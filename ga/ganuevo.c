#include<stdio.h>  // Librería estándar para entrada y salida
#include<stdlib.h> // Librería para el uso de rand()
#include<time.h>   // Librería para el uso de time()
#include<math.h>
#include "mytime.h"

// Nota: para x = 1,85 el valor de f(x) = 2,85

#define PI 3.141592654
#define POBLACION 20   // Tamaño de la población 
#define MAX_NUM_GEN 15  // Máximo número de generaciones
#define GEN 22          // Cantidad de genes por individuo
#define PXOVER 0.25     // Probabilidad de cruce
#define PMUTATION 0.01  // Probabilidad de mutación

// Variables globales
int generacion;
int mutaciones;
int cruzamientos;
int posicion_mejor;

// Estructura de individuo
struct individuo {
  int gen[GEN];         // String de variables (genotipo)
  double fitness;       // Aptitud del individuo
  double decimal;       // Valor decimal convertido directamente del binario
  double convertido;    // Valor decimal ajustado al rango del problema
  double rfitness;      // Aptitud relativa
  double cfitness;      // Aptitud acumulada
};

// Poblaciones
struct individuo population[POBLACION];    // Población actual
struct individuo newpopulation[POBLACION]; // Nueva población
struct individuo prueba;                   // Individuo de prueba

// Declaración de funciones
void inicializar(void);
void evaluacion(void);
void keep_the_best(void);
void seleccion(void);
void crossover(void);
void Xover(int,int);
void swap(int *, int *);
void mutate(void);
void elitist(void);

void main(void) {
	double times, demp = 0.0;
    CLOCK_TYPE crono;
	CLOCK_Start(crono);

	// Se inicializa el fitness del último elemento de la población a 0
	// para guardar en esta posición al mejor individuo de la generación anterior
	population[POBLACION].fitness = 0;
	int i = 0;
	generacion = 0; // Inicio de la generación 0
	inicializar();
	evaluacion();
	keep_the_best();
	
	while(generacion < MAX_NUM_GEN) {
      generacion++;
      seleccion();
      crossover();
      mutate();
      evaluacion();
      elitist();
    }
	
	// Imprime los genes del mejor individuo
	for(i = 0; i < GEN; i++) {
		printf("%d ", population[POBLACION].gen[i]);
	}
	
	printf("Fitness del mejor individuo: %lf\n", population[POBLACION].fitness);
	printf("Decimal del mejor individuo: %lf\n", population[POBLACION].decimal);
	printf("Cantidad de Cruzamientos: %d y Mutaciones: %d\n", cruzamientos, mutaciones);
	CLOCK_End(crono, times);
	printf("Tiempo total %f\n", times);
}

// Esta función inicializa la población binaria y realiza la conversión a decimal
void inicializar(void) {
  // Declaración de variables  
  int numero, contador = 0, contador1 = 0;  
  int hora = time(NULL);
  int exponente;
  long int suma = 0;
  srand(hora);  // Se inicializa la semilla de rand()

  // Lleva a cero todos los individuos decimales
  for(contador = 0; contador < POBLACION; contador++) {
    population[contador].decimal = 0;
  }

  // Se crea la población binaria inicial de forma aleatoria y se transforma a decimal
  exponente = (GEN - 1);
  for(contador = 0; contador < POBLACION; contador++) {
    for (contador1 = 0; contador1 < GEN; contador1++) {
      numero = rand() % 2; // Genera 0 o 1 de forma aleatoria
      population[contador].gen[contador1] = numero;
      suma = (population[contador].gen[contador1]) * pow(2, exponente);
      exponente = (GEN - 1) - contador1;
      population[contador].decimal = suma + population[contador].decimal;
    }
    // Almacena el valor decimal convertido al rango del problema
    population[contador].convertido = (((double)population[contador].decimal * ((double)3 / (double)4194303)) - (double)1.0);
  }
  
  // Muestra la población inicial
  printf("POBLACION INICIAL:\n");
  for(contador = 0; contador < POBLACION; contador++) {
    printf("INDIVIDUO %d\n", contador);
    for (contador1 = 0; contador1 < GEN; contador1++) {
      printf("%d ", population[contador].gen[contador1]);
    }
    printf("\n");
  } 
} // fin funcion inicializar()

// Función para evaluar la población
void evaluacion(void) {
	int contador;
	
	for(contador=0; contador<POBLACION; contador++) {
		double seno = sin((double)10 * (double)PI * (double)population[contador].convertido);
		population[contador].fitness = ((double)population[contador].convertido * seno) + ((double)1.0);
	}
}

// Función para mantener al mejor individuo en la última posición
void keep_the_best(void){
	int mem, i;
	posicion_mejor = 0;
	
	for(mem=0; mem<POBLACION; mem++) {
		if (population[mem].fitness > population[POBLACION].fitness) {
			posicion_mejor = mem;
			population[POBLACION].fitness = population[mem].fitness;
			population[POBLACION].decimal = population[mem].decimal;
			population[POBLACION].convertido = population[mem].convertido;
		}
	}
	
	for (i=0; i<GEN; i++) {
		population[POBLACION].gen[i] = population[posicion_mejor].gen[i];
	}  
}

// Función para la selección proporcional estándar en problemas de maximización
void seleccion() {
	int mem, i, j;
	double sum = 0;
	double p;
	
	// Cálculo del fitness total
	for (mem=0; mem<POBLACION; mem++) {
		sum += population[mem].fitness;
	}
	
	// Cálculo del fitness relativo para cada individuo
	for (mem = 0; mem < POBLACION; mem++) {
		population[mem].rfitness =  population[mem].fitness / sum;
	}
	
	// Cálculo del fitness acumulativo para cada individuo
	population[0].cfitness = population[0].rfitness;
	for (mem=1; mem < POBLACION; mem++) {
		population[mem].cfitness =  population[mem-1].cfitness + population[mem].rfitness;
	}
	
	// Selección de sobrevivientes basados en fitness acumulativo
	for (i=0; i<POBLACION; i++) {
		p = rand()%1000/1000.0;
		if (p < population[0].cfitness) {
			newpopulation[i] = population[0];
		}
		else {
			for (j=0; j<POBLACION; j++) {
				if (p >= population[j].cfitness && p < population[j+1].cfitness) {
					newpopulation[i] = population[j+1];
				}
			}
		}
	}
	
	// Copia de la nueva población
	for (i=0; i<POBLACION; i++) {
		population[i] = newpopulation[i];
	}
}

// Función para realizar cruce entre dos padres
void crossover(void) {
	int mem, uno;
	int primero  =  0;
	double x;

	for (mem = 0; mem < POBLACION; ++mem) {
		x = rand()%1000/1000.0;
		if (x < PXOVER) {
			++primero;
			if (primero % 2 == 0) {
				Xover(uno, mem);
			}
			else {
				uno = mem;
			}
		}
	}
}

// Función para realizar cruce de bits entre dos individuos
void Xover(int uno, int dos) {
	int i;
	int puntocruce = (rand() % (GEN - 1)) + 1;

	// Intercambio de genes hasta el punto de cruce
	for (i = 0; i < puntocruce; i++) {
		swap(&population[uno].gen[i], &population[dos].gen[i]);
	}
}

// Función para intercambiar dos valores
void swap(int *x, int *y) {
	double temp;
	cruzamientos++;
	temp = *x;
	*x = *y;
	*y = temp;
}

// Función para realizar mutaciones
void mutate(void) {
	int i, j;
	double x;
	for (i = 0; i < POBLACION; i++) {
		for (j = 0; j < GEN; j++) {
			x = rand()%1000/1000.0;
			if (x < PMUTATION) {
				mutaciones++;
				if(population[i].gen[j] == 0) {  
					population[i].gen[j] = 1;
				}
				else {
					population[i].gen[j] = 0;
				}
			}
		}
	}
}

// Función para mantener el mejor individuo entre generaciones
void elitist() {
	int i;
	double best, worst;             /* best and worst fitness values */
	int best_mem, worst_mem; /* indexes of the best and worst member */

	best = population[0].fitness;
	worst = population[0].fitness;

	// Find best and worst individual
	for (i = 0; i < POBLACION; ++i) {
		// logic to find best and worst
	}

	// Replace worst individual with best if best of current generation is better than best of previous
	if (best >= population[POBLACION].fitness) {
		// replace with best
	}
	else {
		// replace worst with best from previous generation
	}
}

