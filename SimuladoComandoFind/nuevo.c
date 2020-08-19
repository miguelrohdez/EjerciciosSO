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
#include <fnmatch.h>
#include "permisoOct.h"
#include "tipoArchivo.h"

#define PERMISO "-p"
#define USER "-u"
#define NAME "-n"
#define TYPE "-t"
#define EXEC "-e"
#define HELP "-h"
#define SIZEPATH 1024


typedef struct Archivo{
    char nombre[128];
    char filetype[2];
    char usuario[50];
    int permiso;
    char ruta[1024];
}ARCHIVO;

ARCHIVO arc1;

	int c;
    int nflg=0,uflg=0,tflg=0,pflg=0,eflg=0;
    char *Narg, *Uarg, *Targ, *Parg, *Earg;

void mostrarDirectorio(char *path);
int filtroParametros(ARCHIVO archivo);
void Mostrar_ayuda(void);
    
int main(int argc, char *argv[])
{
    int i;
    
    if (argc<2)
    {
        fprintf(stderr,"Falta la ruta de la busqueda");
        Mostrar_ayuda();
    }

    for(i=2; i < argc; ++i)
    {
           
        if(!strcmp(argv[i],HELP))
        {
            printf("Manual de uso del comando FIND");
            Mostrar_ayuda(); 
        }
        if (!strcmp(argv[i],NAME))
        {
            //printf("Entro nombre con parametro %s\n",argv[i+1] );
            Narg = argv[i+1];
            nflg = 1;
        }

        if(!strcmp(argv[i],USER))
        {
            //printf("Entro usuario con parametro %s\n",argv[i+1]);
            Uarg = argv[i+1];
            uflg = 1;
        }

        if(!strcmp(argv[i],TYPE))
        {
            //printf("Entro usuario con parametro %s\n",argv[i+1] );
            Targ = argv[i+1];
            tflg = 1;
        }

        if(!strcmp(argv[i],PERMISO))
        {
            //printf("Entro permiso con parametro %s\n",argv[i+1] );
            Parg = argv[i+1];
            pflg = 1;
        }

        if(!strcmp(argv[i],EXEC))
        {
            eflg=1;
            Earg=argv[i+1];
            if(argv[i+2] == NULL )
                break;
            if (strcmp(argv[i+2],"-")!=0)
            {
                sprintf(Earg,"%s %s",argv[i+1],argv[i+2] );

            }
        }
    }

    


    /*            printf("Entro permiso con parametro %s\n",optarg );
                Earg = optarg;
                eflg = 1;
                printf("Argumento %c no valido\n",optopt );
                fprintf(stderr, "getopt");
        }*/
        printf("Busca en %s\n",argv[1]);
    mostrarDirectorio(argv[1]);
    
	return 0;
}

void mostrarDirectorio(char *path)
{
    DIR *dir;
    dir = opendir(path);
    struct dirent *directorio;
    struct stat datosArchivo;
    struct passwd *user;
    char tipo[2],octperm[10];
    char comando[SIZEPATH];



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

                    strcpy(arc1.filetype,tipoArchivo(datosArchivo));
                    //printf("%s\n",arc1.file );

                	if(filtroParametros(arc1) == 1){
                		//printf("cumple\n");
                		if (eflg==1)
                		{
                			sprintf(comando,"%s %s",Earg,cadruta);
                			system(comando);
                		}
                		printf("%s\n",cadruta );
                	}

                    if(S_ISDIR(datosArchivo.st_mode)){
                        //printf("%s\n",cadruta);
                        //if(fork() == 0)
                        //{
                            //fprintf(stderr, "Hijo ruta: %s\n",cadruta );
                            mostrarDirectorio(cadruta);
                            //exit(0);
                        //}else{
                            wait(NULL);
                        //}
                    }else{
                        //printf("%s\n",cadruta);
                    }

                }

            }


        }

        closedir(dir);
    }

}

int filtroParametros(ARCHIVO archivo)
{	
	int cumple=1;
	if(nflg == 1){
        int flags=0;
		if(fnmatch(Narg,archivo.nombre,flags) != 0){
			return 0;
		}
	}
	if(uflg == 1){
		if(strcmp(Uarg,archivo.usuario) != 0){
			return 0;
		}
	}
	if(tflg == 1){
		if(strcmp(Targ,archivo.filetype) != 0){
			return 0;
		}
	}
	if(pflg == 1){
		if((atoi(Parg)) != archivo.permiso)
			return 0;
	}

	return cumple;
}

void Mostrar_ayuda(void) {

    printf("Uso:\n"
             "myfind [ <Ruta> ] \n"
             "-h                    Muestra la ayuda\n"
             "-u <name>             Busca por usuario\n"
             "-n <name>             Busqueda por nombre de archivo\n"
             "-t [RDCBFLS]          Busqueda por tipo de archivo\n"
             "-p <777>              Busca por permisos que tiene el archivo\n"
             "-e <comando> <arg>    Aplica el comando a los resultados de las busquedas\n");

    exit(0);
  
}