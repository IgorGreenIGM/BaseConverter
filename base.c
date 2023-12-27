#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "./base.h"


char upper(char c)
{
    unsigned int ord = (unsigned int)c;
    return ord >= 97 && ord <=122 ? ((char)ord)-32 : c;
}

/**
 * @brief convert a number structure to base 10
 * 
 * @param number input number structure
 * @param limit_size limit the number of digits in the input number
 * @return unsigned int converted number
 */
unsigned int to_base10(Number *number, unsigned int limit_size)
{
    unsigned int result = 0;
    for (unsigned int i = 0; i < limit_size; ++i)
        result += number->digits[i] * pow(number->base, limit_size - i - 1);

    return result;

}

/**
 * @brief convert integer for base 10 to another base
 * 
 * @param number input number
 * @param base destination base
 * @return Number* converted number
 */
Number* base10_to(unsigned int number, unsigned int base)
{
    if (number == 0)
    {
        unsigned int p = 0;
        Number *output = create_number(&p, 1, base);
        return output;
    }

    unsigned int d_counter = 0;
    unsigned int quotient = number;
    unsigned int *d_buffer = (unsigned int*)malloc(MAX_DIGITS * sizeof(unsigned int));

    while (quotient != 0)
    {
        d_buffer[d_counter] = quotient % base;
        quotient = quotient / base;
        ++d_counter;
    }

    for(unsigned int i = 0; i < d_counter / 2; ++i)
    {   
        unsigned int tmp = d_buffer[d_counter - i -1]; 
        d_buffer[d_counter - i -1] = d_buffer[i];
        d_buffer[i] = tmp;
    }

    Number *output = create_number(d_buffer, d_counter, base);
    free(d_buffer);

    return output;
}

/**
 * @brief translate alpha numeric string to integer
 * 
 * @param src input string
 * @param size input sitring size
 * @return unsigned int 
 */
unsigned int translate(const char *src, unsigned int size)
{   
    unsigned int res = 0, p = 0;
    for(int i = size-1; i >= 0; --i)
    {
        unsigned int up = upper(src[i]);
        if (up >= 65 && up <= 90)                           // if it's an alphanumeric value 
        {
            res += ((up - 55) * pow(10, p));
            p += 2;
        }
        else                                                // numeric value
        {
            res += (up - 48) * pow(10, p);
            p += 1;
        }
    }

    return res;
}

/**
 * @brief split input string by specified separators
 * 
 * @param raw_input input string (must be null terminated)
 * @param separator string containing separators (must be null terminated)
 * @param digits_count to output the number of splitted values
 * @return unsigned int* array of splited values
 */
unsigned int *split(const char* raw_input, const char* separator, unsigned int* digits_count)
{
    unsigned int *digits = (unsigned int*)malloc(MAX_DIGITS * sizeof(unsigned int)); // output
    assert(digits != NULL);
    char *token = strtok(raw_input, separator);
    unsigned int cpt = 0;

    while (token != NULL)
    {
        digits[cpt] = translate(token, strlen(token));
        token = strtok(NULL, separator);
        ++cpt;
    }

    *digits_count = cpt;

    digits = (unsigned int*)realloc(digits, cpt * sizeof(unsigned int));
    assert(digits != NULL);
    return digits;
}


/**
 * @brief Create a number object
 * 
 * @param digits array of digits
 * @param digits_count size of the digits array
 * @param base base of the input digits
 * @return Number* 
 */
Number *create_number(unsigned int *digits, unsigned int digits_count, unsigned int base)
{
    Number *number = (Number*)malloc(sizeof(*number));
    
    number->base = base;
    number->digits_count = digits_count;  
    number->digits = (unsigned int*)malloc(digits_count * sizeof(unsigned int));
    memcpy(number->digits, digits, digits_count * sizeof(unsigned int));

    return number;
}

void print_number(const Number *number)
{
    for(unsigned int i = 0; i < number->digits_count; ++i)
        printf("%d|", number->digits[i]);
}

void delete_number(Number *number)
{
    free(number->digits);
    free(number);
}

Number *copy_number(const Number* src)
{
    Number* cpy = (Number*)malloc(sizeof(*src));

    cpy->base = src->base;
    cpy->digits_count = src->digits_count;
    cpy->digits = (unsigned int*)malloc(cpy->digits_count * sizeof(unsigned int));
    memcpy(cpy->digits, src->digits, cpy->digits_count * sizeof(unsigned int));

    return cpy;
}

void reverse_number(Number* number)
{
    unsigned int tmp = 0;
    for(unsigned int i = 0; i < number->digits_count / 2; ++i)
    {
        tmp = number->digits[i];
        number->digits[i] = number->digits[number->digits_count -i -1];
        number->digits[number->digits_count -i -1] = tmp;
    }
}


void strip_zeros(Number *number)
{
    unsigned int zeros = 0;
    while (TRUE) // count zeros at the begining
    {
        if (number->digits[zeros] == 0)
            ++zeros;
        else
            break;
    }

    if (zeros == 0)
        return;

    number->digits_count -= zeros; 
    unsigned int* tmp = (unsigned int*)malloc(number->digits_count * sizeof(unsigned int));
    memcpy(tmp, number->digits + zeros, number->digits_count * sizeof(unsigned int));
    memcpy(number->digits, tmp, number->digits_count * sizeof(unsigned int));
    number->digits = (unsigned int*)realloc(number->digits, number->digits_count * sizeof(unsigned int));
    free(tmp);
}


/**
 * @brief convert an input number to a specified base
 * 
 * @param number input number
 * @param base destination base
 * @return Number* converted number
 */
Number* convert(Number *number, unsigned int base)
{
    // Create an array to store the remainder values
    unsigned int rest_cpt = 0;
    unsigned int* rest_buf = (unsigned int*)malloc(sizeof(unsigned int) * MAX_DIGITS);
    Number* rests = create_number(rest_buf, MAX_DIGITS, number->base);

    // Copy and assign the input number as quotient    
    Number *quotient = copy_number(number);    
    unsigned int quotient_base10 = to_base10(quotient, quotient->digits_count);
    
    unsigned int div_cpt = 0;
    // Loop until the quotient becomes 0
    while (quotient_base10 != 0)
    {
        printf("====DIVISION %d=====\n", ++div_cpt);
        printf("QUOTIENT : "); print_number(quotient); printf("\n");

        bool found_not_null = FALSE;
        unsigned int* tmp = (unsigned int*)calloc(MAX_DIGITS, sizeof(unsigned int));
        Number* next_quotient = create_number(tmp, MAX_DIGITS, base);
        free(tmp);
        unsigned int rest = 0, next_quotient_cpt = 0;
        unsigned int number_size = quotient->digits_count, chunk_value = 0;

        // Process each digit of the quotient
        for(unsigned int digit_counter = 0; digit_counter < number_size; ++digit_counter)
        {
            // Get the base-10 value of the current chunk
            chunk_value = to_base10(quotient, digit_counter + 1);

            // Calculate the quotient an store it
            printf("n_cpt : %d", next_quotient_cpt);
            unsigned int q = chunk_value / base;
            Number *nb_q = base10_to(q, number->base);
            memcpy(next_quotient->digits + next_quotient_cpt, nb_q->digits, nb_q->digits_count);
            next_quotient_cpt += nb_q->digits_count;
            printf("| pushed :  "); print_number(nb_q); printf("|\n");
            delete_number(nb_q);    

            if (chunk_value < base)
                continue;
            
            found_not_null = TRUE;
            // Calculate the remainder 
            rest = chunk_value % base;
            printf("passed at : | dc : %d | cv : %d | rest : %d\n", digit_counter, chunk_value, rest);

            // Reset all the digits before the last non-zero digit
            if (rest == 0)
            {
                printf("INSIDE IF\n");
                for(unsigned int i = 0; i <= digit_counter; ++i)
                    quotient->digits[i] = 0;

                printf("QUOTIENT : "); print_number(quotient); printf("\n");
                digit_counter = -1;
                strip_zeros(quotient); // here replace strip_szeros by trim
                printf("QUOTIENT : "); print_number(quotient); printf("\n");
                number_size = quotient->digits_count;
                printf("nb_size : %d\n", number_size);
            }
            else
            {
                printf("inside else\n");
                Number *tmp = copy_number(quotient);
                Number *nb_r = base10_to(rest, number->base);
                memcpy(quotient->digits, nb_r->digits, nb_r->digits_count * sizeof(unsigned int));
                memcpy(quotient->digits + nb_r->digits_count, tmp->digits + digit_counter + 1, (number_size - digit_counter - 1) * sizeof(unsigned int));
                number_size = nb_r->digits_count + number_size - digit_counter - 1;
                quotient->digits_count = number_size;
                quotient->base = nb_r->base;
                printf("QUOTIENT : "); print_number(quotient); printf("\n");
                delete_number(nb_r); delete_number(tmp);

                digit_counter = -1;
                strip_zeros(quotient);
                // next_quotient_cpt = 0;
            }
        }
        // Store the current remainder in the array
        // unsigned int quotient10 = to_base10(quotient, quotient->digits_count); // !! convert here to output base and copy 
        // if (quotient10 == 0)
            // break;

        Number *nb_rest = base10_to(rest, base);
        printf("\n\nresult : "); print_number(nb_rest); printf("\n\n");
        memcpy(rests->digits + rest_cpt, nb_rest->digits, nb_rest->digits_count * sizeof(unsigned int));
        rest_cpt += nb_rest->digits_count;
        delete_number(nb_rest);

        // Update the next quotient and continue the loop
        next_quotient->digits_count = next_quotient_cpt; // ?~~?
        if (found_not_null)
            strip_zeros(next_quotient);
        delete_number(quotient);
        quotient = copy_number(next_quotient);
        delete_number(next_quotient);

        printf("==== NEXT QUOTIENT : "); print_number(quotient); printf("\n");
        printf("====cv : %d\n", chunk_value);

        // Update the base-10 value of the quotient for the next iteration 
        quotient_base10 = to_base10(quotient, quotient->digits_count);
    }
    delete_number(quotient);

    // Reverse the array of remainders and return it
    rests->digits_count = rest_cpt;
    reverse_number(rests);
    
    return rests;
}