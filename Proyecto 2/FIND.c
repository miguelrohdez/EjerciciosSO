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

/*
typedef struct Nodo * NODO;

struct nodo {
	char *nombre;
	char *tipo;
	char *usuario;
	int perm;
	char *ruta;
	struct Nodo *sig;
};

void insertar_nodo (NODO nodo, NODO lista)
{
	lista->sig = nodo;
}

NODO nuevo(char *nombre){
  //Se crea la etiqueta de la estructura
  NODO aprnvo;
  //Se reserva el tamaño de la estructura
  aprnvo = (APNODO) malloc(sizeof(struct nodo));
  //A dato le pasa el contenido de valor
  aprnvo->nombre = nombre;
  //A siguiente le da el valor de NULL
  aprnvo->sig = NULL;
  //Retorna la estructura
  return aprnvo;
}

void eliminar_por_nombre(NODO lista, char *nombre)
{
	NODO tmp;
	tmp  = lista;
	for (; )
}*/
struct passwd *user;
struct group *group;
struct tm *fch;
char buffercwd[SIZEPATH];
struct stat datosArchivo;

int permOct(struct stat datos);
void fpermiso(char *perm,FILE *archtmp);
void fname(char *ruta, char *nombre,FILE *archtmp);
void ftype(char *ruta, char *type,FILE *archtmp);
void fuser(char *user,FILE *archtmp);
void abrirdirectorio(FILE *archTmp,char *path);
void sacarDato(FILE *archTmp);
int main(int argc, char *argv[])
{
	int i;
	//system("clear");
	FILE *archtmp;
	archtmp = fopen("temp","w+");
	abrirdirectorio(archtmp,argv[1]);
	for (i = 2; i <argc; i++)
	{
		if (!strcmp(argv[i],NAME)){
			printf("%s   %s\n",argv[i],argv[i+1] );
			fname(argv[1],argv[i+1],archtmp);
		}
		if(!strcmp(argv[i],PERMISO))
			fpermiso(argv[i+1],archtmp);
		if(!strcmp(argv[i],USER))
			fuser(argv[i+1],archtmp);
		if(!strcmp(argv[i],TYPE)){
			printf("%s %s \n",argv[i],argv[i+1] );
			ftype(argv[1],argv[i+1],archtmp);
		}
	}
	//sacarDato(archtmp);
	
	return 0;
}

void sacarDato(FILE *archTmp)
{
	int i;
	char buffer[1024];
	rewind(archTmp);
	while (!feof(archTmp))
	{
		fscanf(archTmp,"%s",buffer);
		//printf("%s\t\t",buffer );
		if (stat(buffer,&datosArchivo)== -1)
		{
			perror(buffer);
			exit(0);
		}
		else
		{
			//printf("%d\n",datosArchivo.st_ino);
			/*if (*argv[i] == PERMISO)
				fpermiso(argv[i+1]);
			else if(*argv[i] == USER)
				fuser(*argv[i+1]);
			else if(*argv[i] == NAME)
				fname(argv[i+1]);
			else if(*argv[i] == TYPE)
				ftype(argv[i+1]);
			*/	

		}
		
	}
    
    
	
	return;
}

void abrirdirectorio(FILE *archtmp, char *path)
{
	
	DIR *dir;
	struct dirent *directorio;
	dir = opendir(path);
	if (dir == NULL)
		printf("ERROR no se encontro la ruta\n");

	while((directorio = readdir(dir)) != NULL)
	{
		if((strcmp(directorio->d_name,".")!=0) && (strcmp(directorio->d_name,"..")!=0))
		{	
			char cadena[SIZEPATH];
			strcpy(cadena,path);
			strcat(cadena,"/");
			strcat(cadena,directorio->d_name);
			if (stat(cadena,&datosArchivo)== -1)
			{
				perror(cadena);
				exit(0);
			}
			else
			{

				//printf("%s\n",directorio->d_name);
				if(!(S_ISDIR(datosArchivo.st_mode)))
					fprintf(archtmp, "%s\n",cadena );
				if (S_ISDIR(datosArchivo.st_mode))
					abrirdirectorio(archtmp,cadena);
				
			}
		}
	}
	//closedir(dir);
	
}

void fpermiso(char *perm,FILE *archtmp)
{
	int i;
	char octperm[80];
	int intoct,intperm;
	char buffer[SIZEPATH];
	rewind(archtmp);
	while (!feof(archtmp))
	{
		fscanf(archtmp,"%s",buffer);
		//printf("%s\t\t",buffer );
		if (stat(buffer,&datosArchivo)== -1)
		{
			perror(buffer);
			exit(0);
		}
		else
		{
			sprintf(octperm,"%o",permOct(datosArchivo));
			intoct = atoi(octperm);
			intperm = atoi(perm);
			if(intoct == intperm)
				printf("%s-- %d ->%d\n",buffer,intperm,intoct );			
		}
	}
}

void fname(char *ruta, char *nombre,FILE *archtmp)
{
	DIR *dir;
	struct dirent *directorio;
	dir = opendir(ruta);
	if (dir == NULL)
		printf("ERROR no se encontro la ruta\n");

	while((directorio = readdir(dir)) != NULL)
	{
		if((strcmp(directorio->d_name,".")!=0) && (strcmp(directorio->d_name,"..")!=0))
		{	
			char cadena[SIZEPATH];
			strcpy(cadena,ruta);
			strcat(cadena,"/");
			strcat(cadena,directorio->d_name);
			//if(!strcmp(directorio->d_name,nombre))
			if (stat(cadena,&datosArchivo)== -1)
			{
				perror(cadena);
				exit(0);
			}
			else
			{

				//printf("%s\n",directorio->d_name);
				if(!strcmp(directorio->d_name,nombre) && !(S_ISDIR(datosArchivo.st_mode)))
				{
					fprintf(archtmp, "%s\n",cadena );
					printf("Coincide %s\n", cadena);
				}
				if (S_ISDIR(datosArchivo.st_mode))
					fname(cadena,nombre,archtmp);
				
			}
		}
	}
return;
}

void ftype(char *ruta,char *type,FILE *archtmp)
{
	DIR *dir;
	struct dirent *directorio;
	dir = opendir(ruta);
	char buffer[SIZEPATH];
	char *tipo;
	if (dir == NULL)
		printf("ERROR no se encontro la ruta\n");

	while((directorio = readdir(dir)) != NULL)
	{
		if((strcmp(directorio->d_name,".")!=0) && (strcmp(directorio->d_name,"..")!=0))
		{	
			char cadena[SIZEPATH];
			strcpy(cadena,ruta);
			strcat(cadena,"/");
			strcat(cadena,directorio->d_name);
			if (stat(cadena,&datosArchivo)== -1)
			{
				perror(cadena);
				exit(0);
			}
			else
			{
	
				if (S_ISREG(datosArchivo.st_mode))
					tipo = "R";
				else if (S_ISDIR(datosArchivo.st_mode))
					tipo = "D";
				else if (S_ISCHR(datosArchivo.st_mode))
					tipo = "C";
				else if (S_ISBLK(datosArchivo.st_mode))
					tipo = "B";
				else if (S_ISFIFO(datosArchivo.st_mode))
					tipo = "F";
				else if (S_ISLNK(datosArchivo.st_mode))
					tipo = "L";
				else if (S_ISSOCK(datosArchivo.st_mode))
					tipo = "S";
				//printf("%s\t\t %s\n",cadena,tipo );
				if (!strcmp(type,tipo))
				{
					printf("%s \n",cadena);
				}
				if (S_ISDIR(datosArchivo.st_mode))
						ftype(cadena,type,archtmp);
			}
		}
	}

}

void fuser(char *user, FILE *archtmp)
{
	int i;
	char buffer[SIZEPATH];
	char *tipo;
	struct passwd *stuser;
	rewind(archtmp);
	while (!feof(archtmp))
	{
		fscanf(archtmp,"%s",buffer);
		if (stat(buffer,&datosArchivo)== -1)
		{
			perror(buffer);
			exit(0);
		}
		else
		{
		stuser = getpwuid(datosArchivo.st_uid);

			if (!strcmp(user,stuser->pw_name))
			{
					printf("%s %s %s\n",buffer,user,stuser->pw_name);
			}

		}
	}

}

int permOct(struct stat datos)
{
	int user;
	int group;
	int others;
	int perms;
	user = datos.st_mode & (S_IRWXU);
	//printf("U%d\n",user );
	group = datos.st_mode & (S_IRWXG);
	//printf("G%d\n",group );
	others = datos.st_mode & (S_IRWXO);
	//printf("O%d\n", others);
 	perms = user + group + others;

 	return perms;
}

