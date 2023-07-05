#include <stdio.h>
#include "util.h"

int main() {

    /* A fractional number: 13/7 */
    int num1 = 13, den1 = 7;
    /* A second fractional number: 13/11 */
    int num2 = 30, den2 = 11;
    /* An unitilized fractional number */
    int num3, den3;

    printf("First number: ");
    fraction_print(num1, den1);
    printf("\n");

    printf("Second number: ");
    fraction_print(num2, den2);
    printf("\n");

    printf("Addition: ");
    fraction_add(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Subtraction: ");
    fraction_sub(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Multiplication: ");
    fraction_mul(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Division: ");
    fraction_div(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");
    
    printf("--------------------------------------------------");
    
    printf("\n***You must enter two fractional numbers***\n");
    printf("Enter numerator of first fractional number: ");
    scanf("%d",&num1);
    printf("Enter denominator of first fractional number: ");
    scanf("%d",&den1);
    printf("\n");
    printf("Enter numerator of second fractional number: ");
    scanf("%d",&num2);
    printf("Enter denominator of second fractional number: ");
    scanf("%d",&den2);
    
    printf("First number: ");
    fraction_print(num1, den1);
    printf("\n");

    printf("Second number: ");
    fraction_print(num2, den2);
    printf("\n");

    printf("Addition: ");
    fraction_add(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Subtraction: ");
    fraction_sub(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Multiplication: ");
    fraction_mul(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");

    printf("Division: ");
    fraction_div(num1, den1, num2, den2, &num3, &den3);
    fraction_print(num3, den3);
    printf("\n");
    
    return(0);
}
