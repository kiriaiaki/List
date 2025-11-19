#include <stdio.h>

int Print_Pointer (void* Pointer);


int main ()
{
    Print_Pointer ();
    return 0;
}

int Print_Pointer (void* Pointer)
{
    printf ("<FONT COLOR = \"#%06x\"> %p </FONT>", Hash (&Pointer, sizeof (Pointer)) &, Pointer);
    return 0;
}

int Hash (void* Pointer)
{
    

}


