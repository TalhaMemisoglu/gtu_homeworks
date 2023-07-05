#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14

typedef struct{
	float arr[3][3];
	double determinant;
}matrix;

typedef struct{
	int x;
	int y;
	int z;
}vector;

typedef struct third_order_polynomialstructure {
    double a3, a2, a1, a0; 
} third_order_polynomialstructure;

typedef struct polynomial_structure {
    char constant;
    double coef[4];
    double value;
} polynomial_structure;

void print_matrix(matrix initial_matrix);
double find_orthogonal(vector vec_1, vector vec_2, vector* output_vec);
polynomial_structure integrate_polynomials(third_order_polynomialstructure poly1, third_order_polynomialstructure poly2, double a, double b);
double evaluate_polynomial(double coef[], double x);
void determinant_of_matrix(matrix* initial_matrix);

int main(int argc, char *argv[]) {
	
	matrix values;
	values.arr[0][0] = 1; values.arr[0][1]=0.9134; values.arr[0][2]=0.2785;
	values.arr[1][0]=0.9058; values.arr[1][1]= 0.6324; values.arr[1][2]=0.5469;
	values.arr[2][0] = 0.1270; values.arr[2][1]=0.0975 ; values.arr[2][2]= 0.9575;
	print_matrix(values);
	determinant_of_matrix(&values);
	
	/*******************************/
	
	vector vec_1,vec_2;
	vector o_vector;
	
	vec_1.x=1;vec_1.y=2;vec_1.z=3;
	vec_2.x=4;vec_2.y=5;vec_2.z=6;
	double theta = find_orthogonal(vec_1,vec_2,&o_vector);
	printf("%d %d %d\n",o_vector.x,o_vector.y,o_vector.z);
	printf("%f\n",theta);
	
	/*******************************/
	
    third_order_polynomialstructure poly1, poly2;
    double a, b;

    printf("Enter the coefficients of the first third-order polynomial (a3, a2, a1, a0): ");
    scanf("%lf %lf %lf %lf", &poly1.a3, &poly1.a2, &poly1.a1, &poly1.a0);
    printf("Enter the coefficients of the second third-order polynomial (a3, a2, a1, a0): ");
    scanf("%lf %lf %lf %lf", &poly2.a3, &poly2.a2, &poly2.a1, &poly2.a0);
    printf("Enter the interval values (a, b): ");
    scanf("%lf %lf", &a, &b);

    polynomial_structure result = integrate_polynomials(poly1, poly2, a, b);

    printf("\nThe coefficients of the integrated polynomial are: %.2lf %+.2lf*x %+.2lf*x^2 %+.2lf*x^3\n",
            result.coef[0], result.coef[1], result.coef[2], result.coef[3]);
	printf("The value of the result: %f",result.value);
	
	return 0;
}

void print_matrix(matrix initial_matrix)
{
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			printf("%.4f ",initial_matrix.arr[i][j]);
		}
		printf("\n");
	}
}

void determinant_of_matrix(matrix* initial_matrix)
{
	initial_matrix->determinant = initial_matrix->arr[0][0] * (initial_matrix->arr[1][1] * initial_matrix->arr[2][2] - initial_matrix->arr[2][1] * initial_matrix->arr[1][2])
        						- initial_matrix->arr[0][1] * (initial_matrix->arr[1][0] * initial_matrix->arr[2][2] - initial_matrix->arr[1][2] * initial_matrix->arr[2][0])
        						+ initial_matrix->arr[0][2] * (initial_matrix->arr[1][0] * initial_matrix->arr[2][1] - initial_matrix->arr[1][1] * initial_matrix->arr[2][0]);
    
	printf("%f",initial_matrix->determinant);    
}
 
double find_orthogonal(vector vec_1, vector vec_2, vector* output_vec)
{
	
	output_vec->x = (vec_1.y) * (vec_2.z) - (vec_1.z) * (vec_2.y);
    output_vec->y = (vec_1.z) * (vec_2.x) - (vec_1.x) * (vec_2.z);
    output_vec->z = (vec_1.x) * (vec_2.y) - (vec_1.y) * (vec_2.x);
	
	double dot=vec_1.x * vec_2.x + vec_1.y * vec_2.y + vec_1.z * vec_2.z;
	double norm_1=sqrt(vec_1.x * vec_1.x + vec_1.y * vec_1.y + vec_1.z * vec_1.z);
	double norm_2=sqrt(vec_2.x * vec_2.x + vec_2.y * vec_2.y + vec_2.z * vec_2.z);
	double cos_theta = dot / (norm_1 * norm_2);
	double theta = acos(cos_theta) * 180 / PI;
	
	return theta;
}

double evaluate_polynomial(double coef[], double x) 
{
    double result = 0;
    int i;
    for (i = 3; i >= 0; i--) 
	{
        result = result * x + coef[i];
    }
    return result;
}

polynomial_structure integrate_polynomials(third_order_polynomialstructure poly1, third_order_polynomialstructure poly2, double a, double b) 
{
    polynomial_structure result;
    result.constant = 'C';

    result.coef[0] = 0; 
    result.coef[1] = (poly1.a3 * poly2.a2 + poly1.a2 * poly2.a3) / 4;
    result.coef[2] = (poly1.a3 * poly2.a1 + poly1.a2 * poly2.a2 + poly1.a1 * poly2.a3) / 3;
    result.coef[3] = (poly1.a3 * poly2.a0 + poly1.a2 * poly2.a1 + poly1.a1 * poly2.a2 + poly1.a0 * poly2.a3) / 2;

    double fa = evaluate_polynomial(result.coef, a);
    double fb = evaluate_polynomial(result.coef, b);
    result.value = fb - fa;

    return result;
}


