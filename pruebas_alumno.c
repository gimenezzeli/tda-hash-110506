#include "./src/hash.h"
#include "pa2m.h"

void crear()
{
	size_t capacidad1 = 2;
	size_t capacidad2 = 5;
	hash_t *hash = hash_crear(capacidad1);
	hash_t *otro_hash = hash_crear(capacidad2);
	pa2m_afirmar(hash != NULL, "Crear con capacidad menor a 3");
	pa2m_afirmar(otro_hash != NULL, "Crear con capacidad mayor a 3");
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "La cantidad de clave/elementos al crear es 0");
	hash_destruir(hash);
	hash_destruir(otro_hash);
}

void insertar()
{
	size_t capacidad = 5;
	hash_t *hash = hash_crear(capacidad);
	hash_insertar(hash, "2303", "Elizabeth", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 1, "Se puede insertar un elemento");
	hash_insertar(hash, "2804", "Zaira", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 2,
		     "Se puede insertar otro elemento");
	hash_insertar(hash, "2606", "Juana", NULL);
	hash_insertar(hash, "3008", "Ocampo", NULL);
	hash_insertar(hash, "1910", "Rossana", NULL);
	hash_insertar(hash, "0504", "Luciana", NULL);
	hash_insertar(hash, "1602", "Antonela", NULL);
	hash_insertar(hash, "1603", "Edgar", NULL);
	hash_insertar(hash, "2211", "Exequiel", NULL);
	hash_insertar(hash, "0911", "Nahuel", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 10,
		     "Se pueden insertar 10 claves/elementos");
	hash_destruir(hash);
}

void buscar_o_contiene()
{
	size_t capacidad = 5;
	hash_t *hash = hash_crear(capacidad);
	hash_insertar(hash, "2303", "Elizabeth", NULL);
	hash_insertar(hash, "2804", "Zaira", NULL);
	pa2m_afirmar(hash_obtener(hash, "2804") != NULL,
		     "Se obtuvo el elemento buscado");
	pa2m_afirmar(hash_contiene(hash, "2303") == true,
		     "Contiene la clave buscada");
	pa2m_afirmar(hash_contiene(hash, "1403") == false,
		     "No contiene la clave buscada");
	hash_destruir(hash);
}

void eliminar()
{
	size_t capacidad = 5;
	hash_t *hash = hash_crear(capacidad);
	hash_insertar(hash, "2303", "Elizabeth", NULL);
	hash_insertar(hash, "2804", "Zaira", NULL);
	hash_insertar(hash, "1403", "Juan", NULL);
	hash_insertar(hash, "2707", "Sofia", NULL);
	hash_quitar(hash, "1403");
	pa2m_afirmar(hash_cantidad(hash) == 3, "Eliminado correctamente");
	pa2m_afirmar(hash_obtener(hash, "1403") == NULL,
		     "La clave eliminada fue buscada pero no se encontro");
	hash_insertar(hash, "1403", "Juan", NULL);
	pa2m_afirmar(hash_contiene(hash, "1403") == true,
		     "Se puede volver a insertar un elemento eliminado");
	hash_destruir(hash);
}

void pruebas_null()
{
	hash_t *hash = NULL;
	hash_insertar(hash, "2303", "Elizabeth", NULL);
	hash_t *hash_nuevo = hash_crear(5);
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "No se puede insertar en un hash nulo");
	pa2m_afirmar(hash_obtener(hash, "2303") == NULL,
		     "No se puede buscar en un hash nulo");
	hash_insertar(hash_nuevo, NULL, "Marlen", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "No se puede insertar con clave nula");
	hash_insertar(hash_nuevo, "2303", "Elizabeth", NULL);
	pa2m_afirmar(hash_obtener(hash_nuevo, NULL) == NULL,
		     "No se puede buscar con clave nula");
	hash_destruir(hash);
	hash_destruir(hash_nuevo);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== MIS PRUEBAS ========================");
	pa2m_nuevo_grupo("\n=== Crear ===");
	crear();
	pa2m_nuevo_grupo("\n=== Insertar ===");
	insertar();
	pa2m_nuevo_grupo("\n=== Buscar ===");
	buscar_o_contiene();
	pa2m_nuevo_grupo("\n=== Eliminar ===");
	eliminar();
	pa2m_nuevo_grupo("\n=== Pruebas null ===");
	pruebas_null();

	return pa2m_mostrar_reporte();
}
