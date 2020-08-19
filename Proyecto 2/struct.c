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


#define SIZEPATH 1024

void mostrarDirectorio(char *path);

typedef struct Archivo{
    char nombre[128];
    char file[2];
    char usuario[50];
    int permiso;
    char ruta[1024];
}ARCHIVO;
ARCHIVO arc1;

int main(int argc, char *argv[])
{
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
                    printf("%s\n",arc1.nombre );
                    printf("%s\n",arc1.ruta );

                    sprintf(octperm,"%o",permOct(datosArchivo));
                    arc1.permiso=atoi(octperm);;
                    printf("%d\n",arc1.permiso );
                    
                    user = getpwuid(datosArchivo.st_uid);
                    strcpy(arc1.usuario,user->pw_name);
                    printf("%s\n",arc1.usuario );
                    
                    strcpy(arc1.file,tipoArchivo(datosArchivo));
                    printf("%s\n",arc1.file );

                    printf("%s\n",arc1.ruta );
                    if(S_ISDIR(datosArchivo.st_mode)){
                        //printf("%s\n",cadruta);
                        if(0==fork())
                        {
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
