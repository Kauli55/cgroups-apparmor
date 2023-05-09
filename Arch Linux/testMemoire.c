#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int arret = 1;
    while ( arret != -1 )
    {
        void *ptr = malloc(arret*1024);
        arret = arret * 10;

        if (ptr == NULL)
        {
            printf("Il n'y a plus de memoire disponible.\n");
            arret = -1;
        }
        else{
            printf("Memoire toujours disponible.\n");
        }
        free(ptr);
    }

    return(0);
}