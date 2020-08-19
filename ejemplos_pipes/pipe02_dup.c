#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define SIZE 512

int main( void )
{
  pid_t pid;
  int a[2], b[2], readbytes;
  char buffer[SIZE];

  pipe( a );
  pipe( b );

  if ( (pid=fork()) == 0 )
  { 
    close( a[1] ); 
	dup2(a[0],0);
    close( b[0] ); 
	
	dup2(b[1],1);

    while( (readbytes=read(b[1], buffer, SIZE ) ) > 0)
      write( a[0], buffer, readbytes );
    close( a[0] );

    strcpy( buffer, "Soy tu hijo hablandote por la otra tuberia.\n" );
    write(1, buffer, strlen( buffer ) );
    close( b[1] );
  }
  else
  { 
    close( a[0] ); 
	dup2(a[1],1);
    close( b[1] ); 
	dup2(b[0],0);

    strcpy( buffer, "Soy tu padre hablandote por una tuberia.\n" );
    write( a[1], buffer, strlen( buffer ) );
    close( a[1]);

    while( (readbytes=read( b[0], buffer, SIZE )) > 0)
      write( 1, buffer, readbytes );
    close( b[0]);
  }
  waitpid( pid, NULL, 0 );
  exit( 0 );
}
