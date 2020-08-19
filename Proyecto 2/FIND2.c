#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include "permisoOct.h"
#include "tipoArchivo.h"

#define PERMISO "-p"
#define USER "-u"
#define NAME "-n"
#define TYPE "-t"
#define SIZEPATH 1024

typedef struct Archivo{
    char nombre[128];
    char file[2];
    char usuario[50];
    int permiso;
    char ruta[1024];
}ARCHIVO;

typedef struct nodo* NODO;
typedef struct nodo* LISTA;

struct nodo {
  ARCHIVO archivo;
  NODO sig;
};

ARCHIVO arc1;


void mostrarDirectorio(char *path);
void busquedaNombre(char *path,char *dato);
void busquedaTipo(char *path,char *dato);
void busquedaPermiso(char *path,char *dato);
void busquedaUsuario(char *path,char *dato);
NODO agregar_elemento (LISTA lista, ARCHIVO elemento);
NODO crear_nodo(ARCHIVO elemento);
void mostrar_lista (LISTA lista);
NODO eliminar_elemento_nombre (LISTA lista, char *elem);
NODO eliminar_elemento_usuario (LISTA lista, char *elem);
NODO eliminar_elemento_permiso (LISTA lista, char *elem);
NODO eliminar_elemento_tipo (LISTA lista, char *elem);

LISTA lista=NULL;

int main(int argc, char *argv[])
{
    int c;
    int nflg=0,uflg=0,tflg=0,pflg=0;
    int i=0;
   // for (i = 0; i < 20; i++) {

        printf("Entra\n" );
        mostrarDirectorio(argv[1]);
        printf("================\n");
        //mostrar_lista (lista);

        //lista = agregar_elemento (lista, ele);
    /*}
    for (i = 0; i < 5; i++) {
            eliminar_elemento (lista, letra + i);
    }
    agregar_elemento (lista, 'z');
    eliminar_elemento (lista, 'n');
    mostrar_lista (lista);
    return 0;

    while((c = getopt(argc,argv,"u:n:p:t:"))!=-1)
    {
        switch(c)
        {
            case 'n':
                printf("Entro nombre con parametro %s\n",optarg );
                busquedaNombre(argv[1],optarg);
                break;
            case 'u':
                printf("Entro usuario con parametro %s\n",optarg );
                busquedaUsuario(argv[1],optarg);
                break;
            case 't':
                busquedaTipo(argv[1],optarg);
                printf("Entro tipo con parametro %s\n",optarg );
                break;
            case 'p':
                printf("Entro permiso con parametro %s\n",optarg );
                busquedaPermiso(argv[1],optarg);
                break;
            case '?':
                printf("Argumento %c no valido\n",optopt );
                break;
            default:
                fprintf(stderr, "getopt");
                break;
        }
    }*/
    for(i=2; i < argc; i++)
    {
        if (!strcmp(argv[i],NAME))
            eliminar_elemento_nombre(lista,argv[i+1]);
        if(!strcmp(argv[i],PERMISO))
            eliminar_elemento_permiso(lista,argv[i+1]);
        if(!strcmp(argv[i],USER))
            eliminar_elemento_usuario(lista,argv[i+1]);
        if(!strcmp(argv[i],TYPE))
            eliminar_elemento_tipo(lista,argv[i+1]);
    }


    //mostrar_lista (lista);
    return 0;
}

void mostrarDirectorio(char *path)
{
    //fprintf(stderr, "Padre : %d, Hijo: %d\n",getppid(), getpid() );
    DIR *dir;
    dir = opendir(path);
    struct dirent *directorio;
    struct stat datosArchivo;
    struct passwd *user;
    char tipo[2],octperm[10];



    if (dir == NULL)
    {
        perror(path);
        printf("^ Ruta no valida\n\n");

    }
    else
    {
        while((directorio = readdir(dir)) != NULL)
        {
            if((strcmp(directorio->d_name,".") !=0 ) && (strcmp(directorio->d_name,"..")!=0))
            {



                char cadruta[SIZEPATH];
                strcpy(cadruta,path);
                strcat(cadruta,"/");
                strcat(cadruta,directorio->d_name);

                strcpy(arc1.nombre,directorio->d_name);
                strcpy(arc1.ruta,cadruta);

                if(stat(cadruta,&datosArchivo)==-1)
                {
                    perror(cadruta);
                    printf("^ No se pudo sacar los datos\n\n");
                    exit(0);
                }
                else
                {
                    //printf("%s\n",arc1.nombre );
                    //printf("%s\n",arc1.ruta );

                    sprintf(octperm,"%o",permOct(datosArchivo));
                    arc1.permiso=atoi(octperm);;
                    //printf("%d\n",arc1.permiso );

                    user = getpwuid(datosArchivo.st_uid);
                    strcpy(arc1.usuario,user->pw_name);
                    //printf("%s\n",arc1.usuario );

                    strcpy(arc1.file,tipoArchivo(datosArchivo));
                    //printf("%s\n",arc1.file );


                    lista = agregar_elemento (lista, arc1);


                    if(S_ISDIR(datosArchivo.st_mode)){
                        //printf("%s\n",cadruta);
                        if(fork() == 0)
                        {
                            fprintf(stderr, "Hijo ruta: %s\n",cadruta );
                            mostrarDirectorio(cadruta);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                    }else{
                        //printf("%s\n",cadruta);
                    }

                }

            }


        }

        closedir(dir);
    }


}

NODO agregar_elemento (LISTA lista, ARCHIVO nuevo) {
    NODO tmp = lista;
   // printf("Entra agregar_elemento\n" );
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

NODO crear_nodo (ARCHIVO elem) {
    NODO tmp;
   // printf("Entra crear_nodo\n" );
    tmp = (NODO ) malloc (sizeof(struct nodo));
    strcpy(tmp->archivo.nombre, elem.nombre);
    strcpy(tmp->archivo.usuario, elem.usuario);
    tmp->archivo.permiso=elem.permiso;
    strcpy(tmp->archivo.file,elem.file);
    strcpy(tmp->archivo.ruta,elem.ruta);
    tmp->sig = NULL;
    return tmp;
}

void mostrar_lista (LISTA lista) {

    for (; lista != NULL; lista = lista->sig) {
        printf("%s\n", lista->archivo.nombre);
        printf("%s\n", lista->archivo.usuario);
        printf("%s\n", lista->archivo.file);
        printf("%d\n", lista->archivo.permiso);
        printf("%s\n", lista->archivo.ruta);
        printf("------------------------------------------\n");
    }
}

/*
    Hay 3 casos:
    -> cuando el nodo es el inicial
    -> el nodo esta en medio
    -> el nodo es el último.
    Cuando hace free(lista) elimina el nodo, no toda la lista
*/

NODO eliminar_elemento_nombre (LISTA lista, char *nombre) {
    if (lista == NULL)
    {
        fprintf(stderr, "Lista vacía en nombre\n");
        exit(0);
    }
    printf("Entra Nombre\n");
    NODO respaldo = lista;
    NODO anterior = NULL, siguiente = NULL;
    while (lista != NULL)
    {
        siguiente = lista->sig; // Actualiza siguiente
        if (strcmp(nombre,lista->archivo.nombre) != 0) { // Si es el nodo, lo elimina y ajusta
            free (lista);
            if (anterior == NULL) { //Inicial
                lista = siguiente;
                respaldo = lista;
                anterior = NULL;
            } else if (siguiente != NULL) { // En medio
                anterior->sig = siguiente;
                lista = respaldo;
                anterior = NULL;
            } else { //Al final
                anterior->sig  = NULL;
                lista = respaldo;
            }
        } else {
            anterior = lista;
            lista = lista->sig;
            if(lista != NULL)
                siguiente = lista->sig;
        }
    }
    mostrar_lista (respaldo);
}

NODO eliminar_elemento_usuario (LISTA lista, char *elem) {
    if (lista == NULL)
        exit(0);
    printf("Entra USER\n");
    NODO respaldo = lista;
    NODO anterior = NULL, siguiente = NULL;
    for (; lista != NULL; lista = lista->sig) {
        siguiente = lista->sig; // Actualiza siguiente
        if (strcmp( elem,lista->archivo.usuario)) { // Si es el nodo, lo elimina y ajusta
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

NODO eliminar_elemento_permiso (LISTA lista, char *elem) {
    if (lista == NULL)
        exit(0);
    printf("Entra PERMISO\n");
    NODO respaldo = lista;
    NODO anterior = NULL, siguiente = NULL;
    char perm = atoi(elem);
    for (; lista != NULL; lista = lista->sig) {
        siguiente = lista->sig; // Actualiza siguiente
        if (perm != lista->archivo.permiso) { // Si es el nodo, lo elimina y ajusta
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
    printf("Sale\n");
}

NODO eliminar_elemento_tipo (LISTA lista, char *elem) {
    if (lista == NULL)
        exit(0);
    printf("Entra tipo\n");
    NODO respaldo = lista;
    NODO anterior = NULL, siguiente = NULL;
    for (; lista != NULL; lista = lista->sig) {
        siguiente = lista->sig; // Actualiza siguiente
        if (strcmp( elem,lista->archivo.file)) { // Si es el nodo, lo elimina y ajusta
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
