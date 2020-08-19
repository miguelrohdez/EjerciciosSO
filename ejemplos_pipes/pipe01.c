#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define SIZE 512

int main(void)
{
  pid_t pid;
  int p[2], readbytes;
  char buffer[SIZE];

  pipe( p );

  if ( (pid=fork()) == 0 )
  { 
    close( p[1] ); 

    while( (readbytes=read( p[0], buffer, SIZE )) > 0)
      write( 1, buffer, readbytes );

    close( p[0] );
  }
  else
  { 
    close( p[0] ); 
    strcpy( buffer, "Esto llega a traves de la tuberia\n" );
    write( p[1], buffer, strlen( buffer ) );

    close( p[1] );
  }
  waitpid( pid, NULL, 0 );
  exit( 0 );
}
