#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

typedef struct nodo {
	char *clave;
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	nodo_t **tabla;
	size_t cantidad;
	size_t capacidad;
};

unsigned long funcion_hash(const char *str)
{
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) +
		       (unsigned long)c; /* hash * 33 + c */

	return hash;
}

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));

	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash->capacidad = capacidad;
	hash->cantidad = 0;
	hash->tabla = calloc(1, sizeof(nodo_t) * capacidad);
	return hash;
}

/* A partir de la clave y de un elemento, crea un nodo con una copia de la
 * clave, el elemento y un puntero al nodo siguiente inicializado en NULL
 */
nodo_t *crear_nodo(const char *clave, void *elemento)
{
	char *copia_clave = malloc(strlen(clave) + 1);
	if (!copia_clave)
		return NULL;
	strcpy(copia_clave, clave);

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}

	nuevo_nodo->clave = copia_clave;
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;

	return nuevo_nodo;
}

// Hace un swap entre los hashes
void swap(hash_t *hash, hash_t *nuevo_hash)
{
	hash_t hash_aux = (*hash);

	(*hash) = (*nuevo_hash);
	(*nuevo_hash) = (hash_aux);

	hash_destruir(nuevo_hash);
}

/*  Recibe un hash, crea otro nuevo y vuelve a insertar los elementos del hash
 * recibido poniendolos en su posicion correcta en el nuevo hash creado
 */
hash_t *rehash(hash_t *hash)
{
	hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);

	for (size_t i = 0; i < hash->capacidad; ++i) {
		nodo_t *actual = hash->tabla[i];
		while (actual) {
			hash_insertar(nuevo_hash, actual->clave,
				      actual->elemento, NULL);
			actual = actual->siguiente;
		}
	}
	swap(hash, nuevo_hash);
	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	unsigned long int posicion = funcion_hash(clave) % hash->capacidad;
	// factor de carga
	float factor_carga = (float)(hash->cantidad) / (float)(hash->capacidad);

	// insertar cuando la posicion es null
	if (!hash->tabla[posicion]) {
		nodo_t *nodo = crear_nodo(clave, elemento);
		hash->tabla[posicion] = nodo;
		if (anterior)
			*anterior = NULL;
		hash->cantidad++;
		return hash;
	}

	// si la clave ya existe, la actualizo
	nodo_t *actual = hash->tabla[posicion];
	while (actual) {
		if (strcmp(actual->clave, clave) == 0) {
			if (anterior)
				*anterior = actual->elemento;
			actual->elemento = elemento;
			return hash;
		}
		actual = actual->siguiente;
	}

	// insertar encadenado a otro nodo
	nodo_t *nodo = crear_nodo(clave, elemento);
	nodo->siguiente = hash->tabla[posicion];
	hash->tabla[posicion] = nodo;
	if (anterior)
		*anterior = NULL;
	hash->cantidad++;

	if (factor_carga > FACTOR_CARGA_MAXIMO)
		return rehash(hash);

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	unsigned long int posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *actual = hash->tabla[posicion];
	if (!actual)
		return NULL;

	nodo_t *anterior = NULL;
	while (actual && strcmp(actual->clave, clave) != 0) {
		anterior = actual;
		actual = actual->siguiente;
	}

	void *elemento_borrado = actual->elemento;
	if (anterior) {
		anterior->siguiente = actual->siguiente;
	} else {
		hash->tabla[posicion] = actual->siguiente;
	}

	free(actual->clave);
	free(actual);
	hash->cantidad--;
	return elemento_borrado;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	unsigned long int posicion = funcion_hash(clave) % hash->capacidad;

	if (!hash->tabla[posicion])
		return NULL;

	nodo_t *actual = hash->tabla[posicion];
	while (actual) {
		if (strcmp(actual->clave, clave) == 0) {
			return actual->elemento;
		}
		actual = actual->siguiente;
	}

	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;

	unsigned long int posicion = funcion_hash(clave) % hash->capacidad;

	if (!hash->tabla[posicion])
		return false;

	nodo_t *actual = hash->tabla[posicion];
	while (actual) {
		if (strcmp(actual->clave, clave) == 0) {
			return true;
		}
		actual = actual->siguiente;
	}

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;

	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	for (size_t i = 0; i < hash->capacidad; ++i) {
		nodo_t *actual = hash->tabla[i];
		while (actual) {
			nodo_t *siguiente = actual->siguiente;
			if (destructor)
				destructor(actual->elemento);
			free(actual->clave);
			free(actual);
			actual = siguiente;
		}
	}

	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *elemento,
				     void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;

	for (size_t i = 0; i < hash->capacidad; ++i) {
		nodo_t *actual = hash->tabla[i];
		while (actual) {
			n++;
			if (!f(actual->clave, actual->elemento, aux))
				return n;
			actual = actual->siguiente;
		}
	}

	return n;
}
