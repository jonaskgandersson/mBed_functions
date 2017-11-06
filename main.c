#include <stdio.h>
#include "memmem.h"

int main( int argc, char * argv[] )
{
    char string1[] = "myStr\0ing1myString2";
    char string2[] = "String3";

    char *res;

    res = memmem( string1, 19, string2, 7 );

    if( res != NULL)
    {
        fwrite( string1, 1, 19, stdout );
        printf( "\n\r%s\n\r", res );
    }
    else
    {
        printf( "No match\n\r");
    }

    return 0;
}