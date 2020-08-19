#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define COMMAND1 "/bin/cat"
#define COMMAND2 "/usr/bin/wc"

int main( int argc, char **argv )
{
  pid_t pid;
  int p[2];

  pipe(p);

  if ( (pid=fork()) == 0 )
  { 
    close(p[0]); /* cerramos el lado de lectura del pipe */
    dup2(p[1], 1); /* STDOUT = extremo de salida del pipe */
    close(p[1]); /* cerramos el descriptor de archivo que sobra
                         tras el dup2 */

    execlp(COMMAND1, COMMAND1, argv[1], NULL);

  }
  else
  { 
    close(p[1]); /* cerramos el lado de escritura del pipe */
    dup2(p[0], 0); /* STDIN = extremo de entrada del pipe */
    close(p[0]); /* cerramos el descriptor de fichero que sobra
                         tras el dup2 */

    execlp(COMMAND2, COMMAND2, NULL);

  }

  return 0;
}
