#ifndef __BASE_H_INCLUDED__
#define __BASE_H_INCLUDED__

#define TRUE 1
#define FALSE 0

#define MAX_DIGITS 100
#define MAX_DIGIT_LENGTH 10
#define MAX_INPUT_LENGTH 100

typedef int bool;

typedef struct Number  
{
    unsigned int base;
    unsigned int* digits;
    unsigned int digits_count;
}Number;


void strip_zeros(Number *number);
void delete_number(Number* number);
Number* copy_number(const Number* src);
void print_number(const Number* number);
Number* convert(Number *number, unsigned int base);
Number* base10_to(unsigned int number, unsigned int base);
Number* create_number(unsigned int* digits, unsigned int digits_count, unsigned int base);
void reverse_number(Number* number);

unsigned int translate(const char* src, unsigned int size);
unsigned int to_base10(Number *number, unsigned int limit_size);
unsigned int* split(const char* raw_input, const char* separator, unsigned int* digits_count);

#endif //__BASE_H_INCLUDED__