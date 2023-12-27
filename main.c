#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "base.h"


int main(void)
{    
    printf("\n==================~~BASE~CONVERTER~~==================\n");

    // Get the user input in the first base
    unsigned int init_base = 0, dest_base = 0;
    char *input = (char *)malloc(MAX_INPUT_LENGTH * sizeof(char));

    printf("Entrez la base du nombre a convertir : ");
    scanf("%d", &init_base);
    printf("Entrez la base de conversion : ");
    scanf("%d", &dest_base);
    if (init_base < 2 || dest_base < 2)
    {
        printf("Erreur, une base doit etre >= 2\n");
        free(input);
        exit(-1);
    }
    printf("__________________________________________________________________\n");
    printf("entrez les chiffres qui composent le nombre a convertir (separes par des tirets) : \n");
    scanf("%s", input);
    
    // converting : 
    unsigned int count = 0;
    unsigned int *digits = split(input, "-", &count);

    Number *nb = create_number(digits, count, init_base); 
    strip_zeros(nb);
    Number *converted = convert(nb, dest_base);
    printf("\nLe resultat converti est : "); print_number(converted);
    
    delete_number(nb);
    delete_number(converted);
    free(digits);
    free(input);
    //*/
    return 0;
}