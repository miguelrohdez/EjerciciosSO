#define FORMA1 "=): "
#define FORMA2 "=P: "
#define FORMA_DEFAULT "#: "

void prompt1(int i);
void prompt2(int i);

char prompt[5]=FORMA_DEFAULT; //Prompt

void prompt1(int i){
	strcpy(prompt, FORMA1);
	//printf("\n%s",prompt);
	fflush(stdout);
}
void prompt2(int i){
	strcpy(prompt, FORMA2);
	//printf("\n%s",prompt);
	fflush(stdout);
}