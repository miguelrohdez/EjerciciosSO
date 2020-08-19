#define SIZE 512

void *bitacora(void *args);	//Funcion para generar la bitacora
int tub_hilo[2];



void *bitacora(void *args)
{
	char buffer[SIZE];
	int readbytes;
	FILE *archivo;
	archivo=fopen("bitacora.txt","a+");

	time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    if(archivo != NULL)
    {
		fprintf(archivo,"\n\n\n*****************************************************************************\n");
		fprintf(archivo,"******************************%s******************************\n",output);
		while( (readbytes=read(tub_hilo[0], buffer, SIZE)) > 0)
		{
			fprintf(archivo,"%s",buffer);
			fflush(archivo);
		}
	}
    fclose(archivo);
    printf("Murio\n");
	return NULL;
}