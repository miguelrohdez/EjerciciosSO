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

void mostrarDirectorio(char *path);
void busquedaNombre(char *path,char *dato);
void busquedaTipo(char *path,char *dato);
void busquedaPermiso(char *path,char *dato);
void busquedaUsuario(char *path,char *dato);


int main(int argc, char *argv[])
{
    int i;
    int c;
    int nflg=0,uflg=0,tflg=0,pflg=0;
/*
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
    for(i=2; i<argc; i++)
    {
        if (!strcmp(argv[i],NAME))
            busquedaNombre(argv[1],argv[i+1]);
        if(!strcmp(argv[i],PERMISO))
            busquedaPermiso(argv[1],argv[i+1]);
        if(!strcmp(argv[i],USER))
            busquedaUsuario(argv[1],argv[i+1]);
        if(!strcmp(argv[i],TYPE))
        {
            printf("%s ---- %s\n",argv[1],argv[i+1]);
            busquedaTipo(argv[1],argv[i+1]);

        }

    }
   // mostrarDirectorio(argv[1]);

    return 0;
}

void mostrarDirectorio(char *path)
{
    FILE *arc;
    arc=fopen("tmp","w+");
    DIR *dir;
    dir = opendir(path);
    struct dirent *directorio;
    struct stat datosArchivo;
//    char *pwd;

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


                if(stat(cadruta,&datosArchivo)==-1)
                {
                    perror(cadruta);
                    printf("^ No se pudo sacar los datos\n\n");
                    exit(0);
                }
                else
                {
                   fprintf(arc,"%s\n",cadruta);
                    if(S_ISDIR(datosArchivo.st_mode)){
                        printf("%s\n",cadruta);
                        if(0==fork())
                        {
                            mostrarDirectorio(cadruta);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                    }else{
                        printf("%s\n",cadruta);
                    }

                }

            }


        }

        closedir(dir);
    }


}


void busquedaNombre(char *path,char *dato)
{

    DIR *dir;
    dir = opendir(path);
    struct dirent *directorio;
    struct stat datosArchivo;


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

                if(!strcmp(dato,directorio->d_name))
                {

                    printf("%s\n",cadruta);

                }
                if(stat(cadruta,&datosArchivo)==-1)
                {
                    perror(cadruta);
                    printf("^ No se pudo sacar los datos\n\n");
                    exit(0);
                }
                else
                {
                    //printf("%s\n",cadruta);
                    if(S_ISDIR(datosArchivo.st_mode))
                    {
                        //printf("%s\n",cadruta);
                        if(0==fork())
                        {
                            busquedaNombre(cadruta,dato);
                            exit(0);
                        }else{
                            wait(NULL);
                        }


                    }
                    else
                    {
                        //printf("%s\n",cadruta);
                    }

                }

            }


        }

        closedir(dir);
    }

      return;
}


void busquedaTipo(char *path,char *dato)
{
    //printf("TIPO:\n\nRuta:\n%s\nDato:\n%s",path,dato);
    DIR *dir;
    dir = opendir(path);
    struct dirent *directorio;
    struct stat datosArchivo;
    char *tipo;

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


                if(stat(cadruta,&datosArchivo)==-1)
                {
                    perror(cadruta);
                    printf("^ No se pudo sacar los datos\n\n");
                    exit(0);
                }
                else
                {
                   //printf("%s\n",cadruta);
                tipo = tipoArchivo(datosArchivo);
                    if(!strcmp(dato,tipo))
                        printf("%s\n",cadruta);

                    if(S_ISDIR(datosArchivo.st_mode))
                    {
                        //printf("%s\n",cadruta);
                        if(0==fork())
                        {
                            busquedaTipo(cadruta,dato);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                    }
                    else
                    {
                        //printf("%s\n",cadruta);
                    }

                }

            }


        }

        closedir(dir);
    }
    return;
}
void busquedaPermiso(char *path,char *dato)
{
    //printf("Ruta:\n%s\nDato:\n%s",path,dato);
    DIR *dir;
    dir = opendir(path);
    struct dirent *directorio;
    struct stat datosArchivo;
    char octperm[10];
    int peroct,permdato;


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


                if(stat(cadruta,&datosArchivo)==-1)
                {
                    perror(cadruta);
                    printf("^ No se pudo sacar los datos\n\n");
                    exit(0);
                }
                else
                {
                   // printf("%s\n",cadruta);
                    sprintf(octperm,"%o",permOct(datosArchivo));
                    peroct = atoi(octperm);
                    permdato = atoi(dato);
                    if(peroct == permdato)
                        printf("%s\n",cadruta);

                    if(S_ISDIR(datosArchivo.st_mode))
                    {
                        //printf("%s\n",cadruta);
                        if(0==fork())
                        {
                            busquedaPermiso(cadruta,dato);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                    }
                    else
                    {
                    //    printf("%s\n",cadruta);
                    }

                }

            }


        }

        closedir(dir);
    }
    return;
}
void busquedaUsuario(char *path,char *dato)
{
    //printf("Ruta:\n%s\nDato:\n%s",path,dato);
    DIR *dir;
    dir = opendir(path);
    struct dirent *directorio;
    struct stat datosArchivo;
    struct passwd *user;
    //struct group *group;


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


                if(stat(cadruta,&datosArchivo)==-1)
                {
                    perror(cadruta);
                    printf("^ No se pudo sacar los datos\n\n");
                    exit(0);
                }
                else
                {

                    user = getpwuid(datosArchivo.st_uid);
                    if (!strcmp(dato,user->pw_name))
                        printf("%s\n",cadruta);


                    if(S_ISDIR(datosArchivo.st_mode))
                    {
                        //printf("%s\n",cadruta);
                        if(0==fork())
                        {
                            busquedaUsuario(cadruta,dato);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                    }
                    else
                    {
                        //printf("%s\n",cadruta);
                    }

                }

            }


        }

        closedir(dir);
    }
    return;

}












