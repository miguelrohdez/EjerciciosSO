#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 100

char salida[4]="exit";

int main(int argc, char const *argv[])
{
	FILE *archivo;
	archivo = fopen(argv[1],"w+");
	/*salida[0]='e';
	salida[1]='x';
	salida[2]='i';
	salida[3]='t';*/
	char buffer[SIZE];
	char *comp;
	if(archivo != NULL)
    {
    	fgets(buffer,100,stdin);
		fprintf(archivo,"%s",buffer);
		do
		{
			comp = (char*)calloc(100,sizeof(char));
			fflush(stdin);
    		fgets(buffer,100,stdin);
    		strncpy(comp,buffer,strlen(buffer)-1);
			fprintf(archivo,"%s",buffer);
			if (!strcmp(comp,salida))
			{
				exit(0);
			}
			fflush(archivo);
		}while(1);
	}
    fclose(archivo);
	return 0;
}

