int sep_cad(char *, char arg[][50]);

int sep_cad(char *cadena, char arg[][50])
{
	char *token;
	int i=0;
	token = strtok(cadena," ");
	while(token != NULL)
	{	
		strcpy(arg[i],token);
		token = strtok(NULL," ");
		i++;
	}
	return i;
}