<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Repositorio de (Nombre Apellido) - (Padrón) - (Mail)

- Para compilar:

```bash
línea de compilación
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```
---
##  Funcionamiento


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
- Qué es un diccionario
    Es una coleccion de pares (clave, valor). En el cual cuando se inserta/busca/elimina un elemento se lo hace mediante su clave. Esta clave funciona como un indice del diccinario. Debido a que la clave es unica, no hay duplicacion de entradas.  

- Qué es una función de hash y qué características debe tener
    La funcion de hash codifica la clave y la convierte en otra cosa, por ejemplo en un numero asociado. 

- Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, zona de desborde)
    Una tabla de hash se utiliza para implementar un diccionario. Es una estructura que contiene valores y me permite hallar un valor a partir de una clave. Utilizando la funcion hash en distintas claves pueden que den el mismo valor de "hash", a esto se le llama coliciones. 
