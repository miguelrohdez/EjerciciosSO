#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Archivo{
    char nombre[128];
    char tipo;
    char usuario[50];
    int permiso;
    char ruta[1024];
}ARCHIVO;


typedef struct nodo* NODO;
typedef struct nodo* LISTA;

struct nodo {
  char elemento;
  NODO sig;
};
    ARCHIVO arc1;

NODO agregar_elemento (LISTA lista, char elemento);
NODO crear_nodo(char elemento);
void mostrar_lista (LISTA lista);
NODO eliminar_elemento (LISTA lista, char elem);

int main() {
    LISTA lista;
    strcpy(arc1.nombre,"Miguel");
    arc1.tipo='R';
    strcpy(arc1.usuario,"marh");
    arc1.permiso=644;
    strcpy(arc1.ruta,"/home/miguel/");
    printf("%s\n",arc1.nombre );


    char letra = 'a';
    int i=0;
    for (i = 0; i < 20; i++) {

        printf("Entra\n" );
        lista = agregar_elemento (lista, letra + i);
    }
    mostrar_lista (lista);
    printf("================\n");
    for (i = 0; i < 5; i++) {
            eliminar_elemento (lista, letra + i);
    }
    agregar_elemento (lista, 'z');
    eliminar_elemento (lista, 'n');
    mostrar_lista (lista);
    return 0;
}

NODO agregar_elemento (LISTA lista, char nuevo) {
    NODO tmp = lista;
    printf("Entra agregar_elemento\n" );
    if (lista == NULL) {
        return crear_nodo (nuevo);
    }
    else {
        for (; lista != NULL; lista = lista->sig) {
            if (lista->sig == NULL) {
                lista->sig = crear_nodo (nuevo);
                break;
            }
        }
        lista = tmp;
        return lista;
    }
}

NODO crear_nodo (char elem) {
    NODO tmp;
    printf("Entra crear_nodo\n" );
    tmp = (NODO ) malloc (sizeof(struct nodo));
    tmp->elemento = elem;
    tmp->sig = NULL;
    return tmp;
}

void mostrar_lista (LISTA lista) {
    for (; lista != NULL; lista = lista->sig) {
        printf("%c\n", lista->elemento);
    }
}

/*
    Hay 3 casos:
    -> cuando el nodo es el inicial
    -> el nodo esta en medio
    -> el nodo es el último.
    Cuando hace free(lista) elimina el nodo, no toda la lista
*/
NODO eliminar_elemento (LISTA lista, char elem) {
    NODO respaldo = lista;
    NODO anterior = NULL, siguiente = NULL;
    for (; lista != NULL; lista = lista->sig) {
        siguiente = lista->sig; // Actualiza siguiente
        if (elem == lista->elemento) { // Si es el nodo, lo elimina y ajusta
            free (lista);
            if (anterior == NULL) { //Inicial
                lista = siguiente;
                return siguiente;
            } else if (siguiente == NULL) { // En medio
                anterior->sig = NULL;
                return respaldo;
            } else { //Al final
                anterior->sig = siguiente;
                return respaldo;
            }
        }
        anterior = lista; // Actualiza el anterior
    }
}