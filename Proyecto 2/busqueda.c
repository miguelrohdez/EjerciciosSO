#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
#include "trans.h"
#include <dirent.h>
#include <string.h>

#define PERMISO "-p"
#define USER "-u"
#define NAME "-n"
#define TYPE "-t"
#define SIZEPATH 2048



void abrirdirectorio(char *argv[],char *path);

int main(int argc, char *argv[])
{
	
	DIR *dir;
	struct dirent *archivo;
	dir = opendir(argv[1]);
	char *nomArch;
	int modo;
	if (dir == NULL)
		printf("ERROR no se encontro la ruta\n");

	while((archivo = readdir(dir)) != NULL)
	{
		if((strcmp(archivo->d_name,".")!=0) && (strcmp(archivo->d_name,"..")!=0))
		{
			sacarDato(argv[1], archivo->d_name);
		}
	}
	closedir(dir);


	return 0;
	abrirdirectorio(argv[1]);
	return 0;
}

void abrirdirectorio(char *argv[],char *path)
{
	
	DIR *dir;
	struct dirent *directorio;
	struct stat datosArchivo;
	dir = opendir(argv[1]);
	if (dir == NULL)
		printf("ERROR no se encontro la ruta\n");

	while((directorio = readdir(dir)) != NULL)
	{
		if((strcmp(directorio->d_name,".")!=0) && (strcmp(directorio->d_name,"..")!=0))
		{	
		char ruta[SIZEPATH];
			printf("%s\n",directorio->d_name );
			strcpy(ruta,path);
			if(strcmp(ruta,"/"))
			{
				strcat(ruta,"/");
				strcat(ruta,directorio->d_name);
			}
			if(stat(ruta,&datosArchivo) != -1)
			{
				if (S_ISDIR(datosArchivo.st_mode))
					abrirdirectorio(ruta,directorio->d_name);
				else
					printf("%s\n",ruta );
			}
			else
			{
				perror(ruta);
			}
		}
	}
	
	closedir(dir);
	
}