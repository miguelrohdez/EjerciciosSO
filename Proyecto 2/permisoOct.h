int permOct(struct stat datos);

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
