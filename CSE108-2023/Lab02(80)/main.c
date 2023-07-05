#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14

void surface_and_volume(void);
void BMI_calculator(void);

int main(int argc, char *argv[]) {
	
	surface_and_volume();
	BMI_calculator();
	
	return 0;
}

void surface_and_volume(void)
{
	float surface_area,volume; /*declaration for cube and prism*/
	
	float length_cube;
	printf("Enter the edge length for cube:");
	scanf("%f",&length_cube);
	surface_area=6*length_cube*length_cube;
	volume=length_cube*length_cube*length_cube;
	printf("Surface Area=%.2f, Volume=%.2f\n\n", surface_area, volume);
	
	float length_prism,width_prism,height_prism;
	printf("Enter side length for rectangular prism:");
	scanf("%f",&length_prism);
	printf("Enter side width:");
	scanf("%f",&width_prism);
	printf("Enter side height:");
	scanf("%f",&height_prism);
	surface_area=2*(length_prism*width_prism+width_prism*height_prism+length_prism*height_prism);
	volume=length_prism*width_prism*height_prism;
	printf("Surface Area=%.2f, Volume=%.2f\n\n", surface_area, volume);
	
	float sphere_radius;
	printf("Enter the radius for sphere:");
	scanf("%f",&sphere_radius);
	surface_area=4*PI*sphere_radius*sphere_radius;
	volume=4*(PI*sphere_radius*sphere_radius*sphere_radius)/3;
	printf("Surface Area=%.2f, Volume=%.2f\n\n", surface_area, volume);
	
	float cone_radius,cone_height,b;
	printf("Enter the radius for cone:");
	scanf("%f",&cone_radius);
	printf("Enter the height:");
	scanf("%f",&cone_height);
	b=sqrt(cone_radius*cone_radius+cone_height*cone_height);
	surface_area=PI*cone_radius*(b+cone_radius);
	volume=(PI*cone_radius*cone_radius*cone_height)/3;
	printf("Surface Area=%.2f, Volume=%.2f", surface_area, volume);
}

void BMI_calculator(void)
{
	float height,weight,BMI;
	
	printf("\n\n-----------------\n\n");
	printf("Enter your height(m):");
	scanf("%f",&height);
	printf("Enter your weight(kg):");
	scanf("%f",&weight);
	BMI=weight/(height*2);
	if(BMI<18.5) printf("underweight\n");
	if(BMI>=18.5 && BMI<=24.9) printf("avarage weight\n");
	if(BMI>=25 && BMI<=29.9) printf("overweight\n");
	if(BMI>=30) printf("obese\n");
}
