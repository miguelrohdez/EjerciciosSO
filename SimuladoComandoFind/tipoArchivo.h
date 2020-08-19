char * tipoArchivo(struct stat datos);

char * tipoArchivo(struct stat datosArchivo)
{
    char *tipo;
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

    return tipo;
}
