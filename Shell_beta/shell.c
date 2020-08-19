#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "comando.h"
#include "bitacora.h"
#include "prompt.h"



#define PIPE "|"

void iniciar_bitacora();
void escribir_bitacora(char *str, int tam);
void prompt1(int);	//Funcion para cambiar el prompt 1
void prompt2(int);	//Funcion para cambiar el prompt 1
void ejecutar_comandos(char *entrada);
//int separarcad2(char*);
int separar_pipe(char *cadena);

//void *script(void *args);	//Funcion para generar el script


char  **pars;
char cadena[100], *cad_limpia;
char **comandos_comp;
pthread_t bitacora_hilo;
pthread_t script_hilo;
int no_args,pasadas;


int main(int argc, char const *argv[])
{
	pipe(tub_hilo); //Definida en bitacora.h
	signal(29,prompt1);	//Espera señal 29
	signal(30,prompt2);	//Espera señal 30
	
	printf("\tEl PID del proceso es: %d\n",getpid());

	iniciar_bitacora();
	while(1)
	{
		cad_limpia = (char*)calloc(100,sizeof(char));
		printf("\n%s",prompt);
		fflush(stdout);
		fgets(cadena,100,stdin);
		fflush(stdin);
		escribir_bitacora(cadena,50);
		strncpy(cad_limpia,cadena,strlen(cadena)-1);
		ejecutar_comandos(cad_limpia);
		pasadas++;
	}
}



void mandar_exec(char **comando, int num_argc)
{
	switch(num_argc)
	{
		case 0:
			execlp(comando[0],comando[0],NULL);
			break;
		case 1:
			execlp(comando[0],comando[0],comando[1],NULL);
			break;
		case 2:
			execlp(comando[0],comando[0],comando[1],comando[2],NULL);
			break;
		case 3:
			execlp(comando[0],comando[0], comando[1], comando[2], comando[3], NULL);
		default:
			printf("No juegues son muchos parametros gg :c\n");
	}
}


void ejecutar_comandos(char *entrada)
{
	int **tuberias; //Varibale para los pipes
	int parametros = 0, i = 0;
	int num_comandos;
	int num_pipes = separar_por_pipe(entrada);
	tuberias =  ( int **) malloc(num_pipes*sizeof(int *)); //Memoria para los pipes a utilizar

	for (i = 0; i < num_pipes; i++)
	{
		*(tuberias + i) = (int *) malloc(2*sizeof(int));
		pipe(*(tuberias + i)); //Crea la tubería
	}
	if(num_pipes == 0)
	{
		parametros = sep_params(entrada);
		if(!fork())
			mandar_exec(pars,parametros);
		else
			wait();
	}else if(num_pipes > 0)
	{
		num_comandos = num_pipes + 1;
		for (i = 0; i < num_comandos;i++)
		{

			parametros = sep_params(comandos_comp[i]);
			if(!fork())
			{
				if(i < (num_comandos-1) ) //Cambia la entrada estandar desde el 2do comando ejecutado
				{
					close(tuberias[i][0]);
					dup2(tuberias[i][1], 1);
				}
				if(i > 0)//Cambia la salida estandar excepto al ultimo comando a ejecutar
				{
					close(tuberias[i-1][1]);
					dup2(tuberias[i-1][0], STDIN_FILENO);
				}
				mandar_exec(pars,parametros);
			}
			else{
				if(i < (num_comandos-1) ) 
					close(tuberias[i][1]);
				if(i > 0)
					close(tuberias[i-1][0]);
				wait();
			}
		}
	}
}

int sep_params(char *cadena)
{

	char *token;
	int i=0;
	token = strtok(cadena," ");
	pars = (char **) malloc(sizeof(char *));
	while(token != NULL)
	{
		pars = (char **) realloc(pars, (i+1) * sizeof(char *));
		*(pars + i) = (char *) malloc(strlen(token)*sizeof(char));
		strcpy(*(pars + i),token);
		token = strtok(NULL," ");
		i++;
	}
	return i-1;
}

int separar_por_pipe(char *cadena)
{
	int i = 0;
	char *token;
	comandos_comp = (char **) malloc(sizeof(char *));
	token =  strtok(cadena,PIPE);
	while(token != NULL)
	{
		comandos_comp = (char **) realloc(comandos_comp, (i+1) * sizeof(char *));
		*(comandos_comp + i) = (char *) malloc(strlen(token)*sizeof(char));
		strcpy(*(comandos_comp + i),token);
		i++;
		token =  strtok(NULL,PIPE);
	}
	return i-1;
}




void iniciar_bitacora()
{
	if(pthread_create(&bitacora_hilo,NULL,bitacora,NULL))
	{
			printf("Error al crear el hilo\n");
			exit(0);
	}
}

void escribir_bitacora(char *str, int tam)
{
	write(tub_hilo[1],str,tam);
}

