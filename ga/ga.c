/*
 *
 * Algoritmo Genetico Simple, 
 * Compilacion GCC: gcc ga.c -o ga -lm
 * Compilacion ICC: icc ga.c -o ga
 * Ejecucion: ./ga
 *
 * Varias fuentes, GNU
 * Adaptacion Miguel Mendez-Garabetti
 * mendezgarabetti@fosshlab.org
 *
 *


#include<stdio.h>  
#include<stdlib.h> 
#include<time.h>   
#include<math.h>
#include "mytime.h"

#define PI 3.141592654
#define POBLACION 10000   
#define MAX_NUM_GEN 150   
#define GEN 22					

#define PXOVER 0.25            
#define PMUTATION 0.01         

int generacion;
int mutaciones;
int cruzamientos;
int posicion_mejor;

struct individuo 
{
  int gen[GEN];        		
  double fitness;          	
  double decimal;
  double convertido;

  double rfitness;           
  double cfitness;           
};

struct individuo population[POBLACION];    
struct individuo newpopulation[POBLACION]; 
struct individuo prueba;                   

void inicializar(void);
void evaluacion(void);
void keep_the_best(void);
void seleccion(void);
void crossover(void);
void Xover(int,int);
void swap(int *, int *);
void mutate(void);
void elitist(void);

void main(void){

	double times, demp = 0.0;
    CLOCK_TYPE crono;

	CLOCK_Start(crono);

	population[POBLACION].fitness=0;
	int i=0;

	generacion=0;

	inicializar();
	evaluacion();
	keep_the_best();

	while(generacion<MAX_NUM_GEN)
	{

      generacion++;
      seleccion();
      crossover();
      mutate();

      evaluacion();
      elitist();

    }

	for(i=0;i<GEN;i++)
	{
		printf("%d ",population[POBLACION].gen[i]);
	}

	printf("Fitness del mejor individuo: %lf\n",population[POBLACION].fitness);
	printf("Decimal del mejor individuo: %lf\n",population[POBLACION].decimal);
	printf("Cantidad de Cruzamientos:%d y Mutaciones: %d\n",cruzamientos,mutaciones);
	CLOCK_End(crono, times);
	printf("Tiempo total %f", times);
}

void inicializar(void)
{

         int numero,contador=0,contador1=0;  
         int hora = time(NULL);
         int exponente;
         long int suma=0;
         srand(hora);  

    	 for(contador=0; contador<POBLACION; contador++)
		 {
			 population[contador].decimal=0;

		 }

		exponente=(GEN-1);

		for(contador=0; contador<POBLACION; contador++)
		{

			for (contador1=0; contador1<GEN; contador1++)
			{
				numero = rand()%2;
				printf("%d ", numero);

				population[contador].gen[contador1]=numero;

				suma=(population[contador].gen[contador1])*pow(2,exponente);

				exponente=(GEN-1)-contador1;

				population[contador].decimal=suma+population[contador].decimal;

			}

			population[contador].convertido=(((double)population[contador].decimal*((double)3/(double)4194303))-(double)1.0);

		}

		printf("POBLACION INICIAL:\n");
		for(contador=0; contador<POBLACION; contador++)
		{
			printf("INDIVIDUO %d\n",contador);
			for (contador1=0; contador1<GEN; contador1++)
			{

				printf("%d ", population[contador].gen[contador1]);
			}
			printf("\n");
		} 

} 

void evaluacion(void)
{
	int contador;

	for(contador=0; contador<POBLACION; contador++)
	{

		double seno=sin((double)10*(double)PI*(double)population[contador].convertido);

		population[contador].fitness=((double)population[contador].convertido*seno)+((double)1.0);

	}

}

void keep_the_best(void){

	int mem, i;
	posicion_mejor=0;

	for(mem=0;mem<POBLACION;mem++)
	{
		if (population[mem].fitness > population[POBLACION].fitness)
		{
			posicion_mejor=mem;
			population[POBLACION].fitness=population[mem].fitness;
			population[POBLACION].decimal=population[mem].decimal;
			population[POBLACION].convertido=population[mem].convertido;

		} 
	} 

	for (i=0; i<GEN; i++)
	{
      population[POBLACION].gen[i] = population[posicion_mejor].gen[i];       

	}

}

void seleccion()
{

	int mem, i, j;
	double sum = 0;
	double p;

	for (mem=0; mem<POBLACION; mem++)
	{
		  sum += population[mem].fitness;
	}
   printf("Fitness total: %lf generacion %d\n",sum,generacion);

	for (mem = 0; mem < POBLACION; mem++)
	{
		  population[mem].rfitness =  population[mem].fitness/sum;

	}
	population[0].cfitness = population[0].rfitness;

	for (mem=1; mem < POBLACION; mem++)
	{
		  population[mem].cfitness =  population[mem-1].cfitness + population[mem].rfitness;

	}

	for (i=0; i<POBLACION; i++)
	{ 
		  p = rand()%1000/1000.0;

		  if (p<population[0].cfitness)
		  {
				newpopulation[i] = population[0];

		  }
		  else
		  {
				for (j=0; j<POBLACION;j++)
				{
					  if (p>=population[j].cfitness && p<population[j+1].cfitness)
					  {
							newpopulation[i] = population[j+1];

					  }
				}
	}
}

	for (i=0; i<POBLACION; i++)
	{
		  population[i] = newpopulation[i];
	}

}

void crossover(void){

		int  mem, uno;
		int primero  =  0; 
		double x;

		for (mem = 0; mem < POBLACION; ++mem){
			  x = rand()%1000/1000.0;

			  if (x < PXOVER){
					++primero;

					if (primero % 2 == 0){

						Xover(uno, mem);

					}
					else{
						  uno = mem;
					}
			  }
		}
}

void Xover(int uno, int dos){

	 	int i;
		int puntocruce; 
		puntocruce = (rand() % (GEN - 1)) + 1;

	    for (i = 0; i < puntocruce; i++){
				swap(&population[uno].gen[i], &population[dos].gen[i]);
		}

}

void swap(int *x, int *y){

		double temp;
        cruzamientos++;
		temp = *x;
		*x = *y;
		*y = temp;

}

void mutate(void){
		int i, j;
		double x;
		for (i = 0; i < POBLACION; i++){
			  for (j = 0; j < GEN; j++){
					x = rand()%1000/1000.0;
					if (x < PMUTATION){
						  mutaciones++;

						  if(population[i].gen[j]==0){  
							population[i].gen[j] = 1;

						  }
						  else{
							population[i].gen[j] = 0;

					      }
					}
			  }
		}
}

void elitist(){
		int i;
		double best, worst;             
		int best_mem, worst_mem; 

		best = population[0].fitness;
		worst = population[0].fitness;
		for (i = 0; i < POBLACION; ++i){

			  if(population[i].fitness > population[i+1].fitness){      
					if (population[i].fitness >= best){
						  best = population[i].fitness;
						  best_mem = i;
					}
					if (population[i+1].fitness <= worst){
						  worst = population[i+1].fitness;
						  worst_mem = i + 1;
					}
			  }
			  else{
					if (population[i].fitness <= worst){
						  worst = population[i].fitness;
						  worst_mem = i;
					}
					if (population[i+1].fitness >= best){
						  best = population[i+1].fitness;
						  best_mem = i + 1;
					}
			  }
		}

		if (best >= population[POBLACION].fitness){
			for (i = 0; i < GEN; i++){
			   population[POBLACION].gen[i] = population[best_mem].gen[i];
			}
			population[POBLACION].fitness = population[best_mem].fitness;
		}
		else{
			for (i = 0; i < GEN; i++){
			   population[worst_mem].gen[i] = population[POBLACION].gen[i];
			}
			population[worst_mem].fitness = population[POBLACION].fitness;
			} 
}

