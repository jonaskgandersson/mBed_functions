#include <string.h>

// Search for needle in haystack, if match return first position of needle else NULL pointer
void * memmem( const void *haystack, int haystack_len, const void *needle, int needle_len )
{
    // Loop throw haystack and cmp with needle
    for( ; haystack_len >= needle_len; haystack++, haystack_len-- )
    {
        // Check if match
        if( memcmp( haystack, needle, needle_len ) == 0 )
        {
            return (void *)haystack;
        }
    }

    return NULL;
}