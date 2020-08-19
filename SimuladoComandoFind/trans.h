void transmodo(struct stat datos);


void transmodo(struct stat datos){
	if (S_ISREG(datos.st_mode))
		{
			printf("-");
		}else{
			printf("*");
		}
		//Usuario
		if (datos.st_mode & S_IRUSR)
			printf("r");
		else
			printf("-");
		
		if (datos.st_mode & S_IWUSR)
			printf("w");
		else
			printf("-");

		if (datos.st_mode & S_IXUSR)
			printf("x");
		else
			printf("-");

		//Grupo
		if (datos.st_mode & S_IRGRP)
			printf("r");
		else
			printf("-");
		if (datos.st_mode & S_IWGRP)
			printf("w");
		else
			printf("-");
		if (datos.st_mode & S_IXGRP)
			printf("x");
		else
			printf("-");

		//Otros
		if (datos.st_mode & S_IROTH)
			printf("r");
		else
			printf("-");
		if (datos.st_mode & S_IWOTH)
			printf("w");
		else
			printf("-");
		if (datos.st_mode & S_IXOTH)
			printf("x");
		else
			printf("-");
	return;
}