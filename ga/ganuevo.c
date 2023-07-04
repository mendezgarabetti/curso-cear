#include<stdio.h>  // libreria basica para entrada y salida  
#include<stdlib.h> // libreria para el uso de rand()  
#include<time.h>   // libreria para el uso de time()  
#include<string.h>  // Agregado para el uso de atoi()
#include<math.h>
#include "mytime.h"
//#include<omp.h>
/******************************************************************************************************************************************/

// para x = 1,85 el valor de f(x) = 2,85
 //./genetico_secuencial_param 20 1000000 0.5 0.9    1) individuos, 2) generaciones, 3) Prob. cruce 4) prob de mutacion



#define PI 3.141592654
// #define POBLACION 1000   //50     /* population size */ 
//#define MAX_NUM_GEN 150   //150    /* max. number of generations */
#define GEN 22					// cantidad de genes por individuo o sea 22, del tipo:
								// 1111100000111110000011
//#define PXOVER 0.25            /* probability of crossover */
//#define PMUTATION 0.01         /* probability of mutation */

/******************************************************************************************************************************************/
int generacion;
int mutaciones;
int cruzamientos;
int posicion_mejor;

/******************************************************************************************************************************************/



// define una estructura de datos llamada individuo	
//struct individuo /* genotype (GT), a member of the population */
//{
//  int gen[GEN];        		/* a string of variables */
//  double fitness;          	// GT's fitness %lf
//  double decimal;   		/* valor decimal convertido directamente del binario, sin relación con el rango real del problema */
//  double convertido;		/* valor decimal previo convertido al rango del problema */
//  //double upper[NVARS];       /* GT's variables upper bound */
//  //double lower[NVARS];       /* GT's variables lower bound */
//  double rfitness;           /* relative fitness */
//  double cfitness;           /* cumulative fitness */
//};

// crea dos estructuras de datos del tipo "individuo"
//struct individuo population[POBLACION];    /* population */
//struct individuo newpopulation[POBLACION]; /* new population; */
//struct individuo prueba;                   /* replaces the */
                                           /* old generation */

struct individuo
{
  int* gen;        		
  double fitness;
  double decimal;
  double convertido;
  double rfitness;
  double cfitness;
};

//struct individuo *population;  
//struct individuo *newpopulation;
//struct individuo prueba;


                                        
/******************************************************************************************************************************************/
// DECLARACION DE FUNCIONES
void inicializar(struct individuo *population, int POBLACION);
void evaluacion(struct individuo population[], int POBLACION);
void keep_the_best(struct individuo population[], int POBLACION);
void seleccion(struct individuo newpopulation[], struct individuo population[], int POBLACION);
void crossover(struct individuo population[], int POBLACION, double PXOVER);
void Xover(int,int, struct individuo population[]);
void swap(int *, int *);
void mutate(struct individuo population[], int POBLACION, double PMUTATION);
void elitist(struct individuo population[], int POBLACION);

/******************************************************************************************************************************************/
int main(int argc, char *argv[]){  // Se agregan argumentos a main
	
	int POBLACION;   
	int MAX_NUM_GEN;
	//int GEN;
	double PXOVER;
	double PMUTATION;
	
	
	// Se asignan los argumentos a las variables
	// Nota: atoi() y atof() convierten una cadena a int y double respectivamente
	POBLACION = atoi(argv[1]);
	MAX_NUM_GEN = atoi(argv[2]);
	//GEN = atoi(argv[3]);
	PXOVER = atof(argv[3]);
	PMUTATION = atof(argv[4]);
	
// Asignamos memoria para "population" y "newpopulation"
struct individuo *population = (struct individuo *)malloc(POBLACION * sizeof(struct individuo));
struct individuo *newpopulation = (struct individuo *)malloc(POBLACION * sizeof(struct individuo));

// Verificamos que la memoria se haya asignado correctamente.
if(population == NULL || newpopulation == NULL) {
    printf("Error! No se pudo asignar memoria para la población.\n");
    return 1; // Regresa un error.
}

// Ahora asignamos memoria para el gen de cada individuo.
for(int i = 0; i < POBLACION; i++) {
    population[i].gen = (int *)malloc(GEN * sizeof(int));
    newpopulation[i].gen = (int *)malloc(GEN * sizeof(int));
    
    // Verificamos que la memoria se haya asignado correctamente.
    if(population[i].gen == NULL || newpopulation[i].gen == NULL) {
        printf("Error! No se pudo asignar memoria para los genes del individuo.\n");
        return 1; // Regresa un error.
    }
}
	
	double times, demp = 0.0;
    CLOCK_TYPE crono;
    //CLOCK_TYPE crono_w;
    //CLOCK_TYPE crono_w2;
	
	
	CLOCK_Start(crono);
    //sleep(2);
    
	
	// al ultimo elemento del arreglo "population" le coloco en el parametro
	// "fitness" el valor "0" para mantener en esta posición al mejor individuo
	// de la generación anterior
	population[POBLACION-1].fitness=0;
	int i=0;
	// indico que comienzo con la primer generación o sea la "0"
	generacion=0;
	// el primer paso es realizar la inicialiación de la población lo cual lo hacemos
	// con la función "inicializar()"
	inicializar(population, POBLACION);
	evaluacion(population, POBLACION);
	keep_the_best(population, POBLACION);
	
	while(generacion<MAX_NUM_GEN)
	{
	  //while(population[POBLACION].fitness<=2.85){
      generacion++;
      seleccion(newpopulation, population, POBLACION);
      crossover(population, POBLACION, PXOVER);
      mutate(population, POBLACION, PMUTATION);
	  evaluacion(population, POBLACION);
      //elitist();
    }
	
	//printf("EL FITNESS MAS ALTO ES %lf\n",population[POBLACION].fitness);
	//printf("El mejor individuo es el numero %d con los genes:\n",posicion_mejor);
	
	for(i=0;i<GEN;i++)
	{
		printf("%d ",population[POBLACION-1].gen[i]);
	}//fin for 0
	
	printf("Fitness del mejor individuo: %lf\n",population[POBLACION-1].fitness);
	printf("Decimal del mejor individuo: %lf\n",population[POBLACION-1].decimal);
	printf("Cantidad de Cruzamientos:%d y Mutaciones: %d\n",cruzamientos,mutaciones);
	CLOCK_End(crono, times);
	printf("Tiempo total %f", times);
	return 0; // Termina exitosamente.

}

/******************************************************************************************************************************************/
// esta funcion crea la poblacion inical binaria
void inicializar(struct individuo population[], int POBLACION)
{
        // Declaracion de variables  
         int numero,contador=0,contador1=0;  
         //int hora = time(NULL);
         int hora = 12345;
         int exponente;
         long int suma=0;
         srand(hora);  // la funcion srand() es con la que se le especifica la Semilla de rand()
         
         // con este bucle se lleva a cero todos los indiviuos decimales
    	 for(contador=0; contador<POBLACION; contador++)
		 {
			 population[contador].decimal=0;
			 //printf("Se llevo a cero el arreglo decimal\n\n");
		 }

        //AQUI SE CREA LA POBLACION BINARIA INICIAL CODIFICADA (ALEATORIAMENTE) 
		//Y SE REALIZA LA CONVERSION DE CADA INDIVIDUO BINARIO A DECIMAL PARA
		//LUEGO SER EVALUADOS EN LA FUNCION OBJETIVO
		
		// la posicion 0 representa el gen 1, y la posicion 21 representa al gen 22
		exponente=(GEN-1);
		// GEN es igual a 21
		
		for(contador=0; contador<POBLACION; contador++)
		{
			//printf("Valor de la variable contador %d\n",contador);
			
			for (contador1 = 0; contador1 < GEN; contador1++)
			{
				numero = rand() % 2; // generamos 0 o 1 de forma aleatoria
				printf("%d ", numero);
				population[contador].gen[contador1] = numero;
				exponente = (GEN - 1) - contador1; // actualizamos el exponente antes de usarlo
				suma = (population[contador].gen[contador1]) * pow(2, exponente);
				population[contador].decimal += suma; // acumulamos la suma en decimal
			}
			
			
			
			
			
			//~ for (contador1=0; contador1<GEN; contador1++) // itera de 0 .. 1 .. 2, hasta que contador1=20
			//~ {
				//~ numero = rand()%2; //generamos 0 o 1 de forma aletaoria
				//~ printf("%d ", numero);
				
				//~ // AQUI SE INTRODUCEN LOS VALORES ALEATORIOS "0" Y "1" EN LA POBLACION NUEVA:
				//~ population[contador].gen[contador1]=numero;
				
				//~ // AQUI SE REALIZA LA TRANSFORMACION DE TODOS LOS INDIVIDUOS A
				//~ // DECIMAL para ser almacenado su equivalente en 			 population[contador].decimal
				//~ // el rango decimal va desde 0 a 4.194.303, esto luego se convierte al rango [-1,2]
				//~ // PARA QUE SEAN EVALUADOS PROXIMAMENTE, la conversion debe evaluar bit a bit desde izq a
				//~ // derecha, por lo que usamos la variable "suma" para almacenar los resultados de las potencias de cada 
				//~ // bit
				//~ suma=(population[contador].gen[contador1])*pow(2,exponente);
				//~ // "suma" contiene el valor 
				//~ // ejemplo: Por lo tanto, en la primera iteración, suma será 0 o 2^21 dependiendo del valor del gen, 0 para el primero y 1 para el segundo.
				
				
				//~ // disminuimos el exponente para avanzar de bit
				//~ exponente=(GEN-1)-contador1;
				//~ //printf("el valor del exponente es %d\n", exponente);
				//~ //printf("el valor de la variable suma es: %ld\n",suma);
				
				//~ // luego almacenamos en .decimal el valor total equivalente en decimal del binario generado, lo que seria nuestro
				//~ // "x'" el que luego es convertido a "x"
				//~ population[contador].decimal=suma+population[contador].decimal;
				
				//~ //printf("Creando gen %d del individuo %d con el valor %d\n",contador1,contador,numero);
			//~ }
			// por ultimo en .convertido almacenamos el valor de "x" o sea la conversion final del individuo a decimal dentro del
			// rango del problema
			population[contador].convertido=(((double)population[contador].decimal*((double)3/(double)4194303))-(double)1.0);
			printf("El valor decimal del individuo %d es %lf y el valor convertido es %lf\n", contador, population[contador].decimal,population[contador].convertido);
			
			//printf("\n");
		}
		// mostramos la poblaciobn inicial
		printf("POBLACION INICIAL:\n");
		for(contador=0; contador<POBLACION; contador++)
		{
			printf("INDIVIDUO %d\n",contador);
			for (contador1=0; contador1<GEN; contador1++)
			{
				//printf("INDIVIDUO %d\n",contador);
				// al no colocar "\n" imprime bit a bit uno al lado del otro
				printf("%d ", population[contador].gen[contador1]);
			}
			printf("\n");
		} 
		//getchar();  // genera una pausa en la ejecucion del programa  
} // fin funcion inicialziar()

/******************************************************************************************************************************************/
// evalular poblacion
void evaluacion(struct individuo population[], int POBLACION)
{
	int contador;
	
	for(contador=0; contador<POBLACION; contador++)
	{
		//printf("EVALUANDO INDIVIDUO %d\n",contador);
		double seno=sin((double)10*(double)PI*(double)population[contador].convertido);
		//printf("el seno es %lf",seno);
		population[contador].fitness=((double)population[contador].convertido*seno)+((double)1.0);
		//printf("El valor del fitnes del individuo %d es %lf\n", contador, population[contador].fitness);
		//printf("\n");
	}
//getchar(); 

}// fin funcion evaluacion()

/******************************************************************************************************************************************/
// encontrar al mejor y lo almacena en la ultima posicion
void keep_the_best(struct individuo population[], int POBLACION){
//	printf("ENTRE A KEEP THE BEST\n");
	int mem, i;
	posicion_mejor=0;
	
	for(mem=0;mem<POBLACION;mem++)
	{
		if (population[mem].fitness > population[POBLACION-1].fitness)
		{
			posicion_mejor=mem;
			population[POBLACION-1].fitness=population[mem].fitness;
			population[POBLACION-1].decimal=population[mem].decimal;
			population[POBLACION-1].convertido=population[mem].convertido;
			
		} // fin if 0
	} // fin for 0
	//printf("el mejor individuo de la poblacion inicial es: %d\n",posicion_mejor);
	//getchar();
	for (i=0; i<GEN; i++)
	{
      population[POBLACION-1].gen[i] = population[posicion_mejor].gen[i];       
      // Y ACA ALMACENA EL INDIVIDUO QUE TIENE EL MEJOR FINNESS EN la ultima posicion
	}// fin for 2  

}//fin funcion keep_the_best()

/******************************************************************************************************************************************/
// Estándar de selección proporcional para los problemas de maximización de la 
// incorporación de modelo elitista se asegura de que el miembro más apto sobrevive.
void seleccion(struct individuo newpopulation[], struct individuo population[], int POBLACION)
{
	//	printf("ENTRE A SELECCION\n");
	int mem, i, j;
	double sum = 0;
	double p;
		
	// Calcular el fitness total

	for (mem=0; mem<POBLACION; mem++)
	{
		  sum += population[mem].fitness;
	}
   printf("Fitness total: %lf generacion %d\n",sum,generacion);
	
	for (mem = 0; mem < POBLACION; mem++) // Calcular el fitness relativo para cada individuo
	{
		  population[mem].rfitness =  population[mem].fitness/sum;
		  //printf("Fitness relativo: %lf individuo %d\n",population[mem].rfitness, mem);
		  //printf("fitness %lf individuo %d\n",population[mem].fitness, mem);
	}
	population[0].cfitness = population[0].rfitness;
	
	// Calcular el fitness acumulativo para cada individuo
	
	for (mem=1; mem < POBLACION; mem++)
	{
		  population[mem].cfitness =  population[mem-1].cfitness + population[mem].rfitness;
		  //printf("Fitness acumulativo: %lf individuo %d\n",population[mem].cfitness , mem);
	}

	// Por último, selecciono los sobrevivientes con el fitness acumulativo
	
	for (i=0; i<POBLACION; i++)
	{ 
		  p = rand()%1000/1000.0;
		  //printf("valor p aleatorio: %lf\n",p);
		  if (p<population[0].cfitness)
		  {
				newpopulation[i] = population[0];
				//printf(" asigno a newpopulation i = population 0");
				// Si el numero aleatorio generado es menor que el fitnes
				// acumulativo del primer individuo, copiamos el primer individuo en la 
				// nueva población (de progenitores) ?
		  }
		  else
		  {
				for (j=0; j<POBLACION;j++)
				{
					  if (p>=population[j].cfitness && p<population[j+1].cfitness)
					  {
							newpopulation[i] = population[j+1];
							//printf("newpopulation[%d] = population[%d]\n",i,j+1);
							//getchar();  
					  }
				}
	}
}
	// once a new population is created, copy it back

	for (i=0; i<POBLACION; i++)
	{
		  population[i] = newpopulation[i];
	}
}
/******************************************************************************************************************************************/
// Selecciona dos padres que participan en el cruce. Implementa un punto de cruce individual.
void crossover(struct individuo population[], int POBLACION, double PXOVER){
	//printf("ENTRE A CROSSOVER\n");
	//getchar(); 
		int  mem, uno;
		int primero  =  0; /* count of the number of members chosen */
		double x;

		for (mem = 0; mem < POBLACION; ++mem){
			  x = rand()%1000/1000.0;
			  //printf("valor aleatorio x es igual a %lf\n",x);
			  if (x < PXOVER){
					++primero;
					//printf("el valor de la variable primero es %d\n",primero);
					//getchar(); 
					if (primero % 2 == 0){
						//printf("el valor de la variable uno es %d\n",uno);
						//printf("al Xover se le manda uno %d mem %d",uno,mem);
						Xover(uno, mem, population);
						//getchar(); 
					}
					else{
						  uno = mem;
					}
			  }
		}
}

/******************************************************************************************************************************************/
// lleva a cabo cruce de los dos padres seleccionados.
void Xover(int uno, int dos, struct individuo population[]){
	//	printf("ENTRE A Xover valor de uno: %d dos: %d\n",uno, dos);
	 	int i;
		int puntocruce; /* crossover point */
		puntocruce = (rand() % (GEN - 1)) + 1;
	//printf("punto de cruce aleatorio es %d\n",puntocruce);
	//getchar();
	
		
		// select crossover point
	    for (i = 0; i < puntocruce; i++){
				swap(&population[uno].gen[i], &population[dos].gen[i]);
		}
		
}

/******************************************************************************************************************************************/
// esta funcion realiza el cruze de bits
void swap(int *x, int *y){
		//printf("ENTRE A swap\n");
		double temp;
        cruzamientos++;
		temp = *x;
		*x = *y;
		*y = temp;

}

/******************************************************************************************************************************************/
// Mutación: la mutación aleatoria uniforme. Se selecciona un bit aleatorio para realizar la 
// mutación, el cual se sustituye por un valor aleatorio, al ser binario 0 o 1
void mutate(struct individuo population[], int POBLACION, double PMUTATION){
		//printf("entre a mutar\n");
		//printf("El valor de PMUTATION es: %lf\n", PMUTATION);

		int i, j;
		double x;
		for (i = 0; i < POBLACION; i++){
			  for (j = 0; j < GEN; j++){
					x = rand()%1000/1000.0;
					if (x < PMUTATION){
						  //printf("MUTATE\n");
						  mutaciones++;
						  /* find the bounds on the variable to be mutated */
						  if(population[i].gen[j]==0){  
							population[i].gen[j] = 1;
							//printf("Se ha mutado el bit %d, del individuo %d, de la generacion %d, del valor 0 a 1\n",j,i,generacion);
						  }
						  else{
							population[i].gen[j] = 0;
							//printf("Se ha mutado el bit %d, del individuo %d, de la generacion %d, del valor 1 a 0\n",j,i,generacion);
					      }
					}
			  }
		}
}

/******************************************************************************************************************************************/
//
void elitist(struct individuo population[], int POBLACION){
		int i;
		double best, worst;             /* best and worst fitness values */
		int best_mem, worst_mem; /* indexes of the best and worst member */

		best = population[0].fitness;
		worst = population[0].fitness;
		for (i = 0; i < POBLACION; ++i){
		//for (i = 0; i < POBLACION - 1; ++i){
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
				
		// Si el mejor individuo de la nueva población es mejor que 
		// el mejor individuo de la población anterior, entonces, 
		// copiar el mejor de la nueva población, sino reemplazar el peor
		// individuo de la población actual con el mejor de la generación anterior

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
