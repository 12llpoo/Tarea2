#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>   // libreria para el uso de time()
#include<math.h>   // libreria para recursos matematicos

// Declaramos el tamaño del arreglo y un tamaño para utizarlo en algunas funciones.
int arreglo[20],tam=20;

// Inicializamos todas las funciones a utilizar para evitar errores Y/ warning.
int* crear_subVector(int begin, int end, int* origin);
void print(int my_rank, int comm_sz, int n_over_p, int* sub_vec);
float subPromedio(int *sub_vector, int largo_sv);
float varianza(int vector[],float promArreglo);
float desviacionEstandar();
void Llenar_arreglo();

int main(void){

    int comm_sz; // numero de procesos
    int my_rank;

    int* sub_vec = NULL;
    int* total = NULL;
    int n_over_p; 
    float recv_promedio; /* Promedio general de los subvectores recibidos */

    MPI_Init(NULL, NULL); /* Inicializamos MPI */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    

    n_over_p = 20/comm_sz; // cantidad en la que se divide el vector

    if (my_rank != 0) {
	sub_vec = (int*)malloc(n_over_p * sizeof(int));
        MPI_Recv(sub_vec, n_over_p, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        print(my_rank, comm_sz, n_over_p, sub_vec); //Dentro de esta func se calculan los promedios de cada sub_vector

	float recv_promedio = subPromedio(sub_vec,n_over_p); //Se calcula el subpromedio del subvector p[i]
        MPI_Send(&recv_promedio,1,MPI_FLOAT,0,0,MPI_COMM_WORLD);//Se envía el arreglo con los subpromedio al Proce=0 para calculos.
        	

    } else { // Procesador 0 (Promedios de promedio/varianza/desviacion estandar)

	//Llenamos el arreglo ramdom con Llenar_arreglo();
  	Llenar_arreglo();	

        for (int i = 1; i < comm_sz; i++) {
            sub_vec = crear_subVector(i*n_over_p, (i*n_over_p)+n_over_p, arreglo);
            MPI_Send(sub_vec, n_over_p, MPI_INT, i, 0, MPI_COMM_WORLD);  // Enviamos los sub-vectores a los n(i) procesos
        }

        sub_vec = crear_subVector(0, n_over_p, arreglo);
	float subProm=subPromedio(sub_vec,n_over_p);

        for(int i=1; i<comm_sz; i++) {
        	float recibeProm = recv_promedio;
        	MPI_Recv(&recibeProm,1,MPI_FLOAT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); //Recibe los subpromedios con MPI_Recv
        	subProm += recibeProm; //Se suman la totalidad de los diferentes subpromedios
	} 
	
	print(my_rank, comm_sz, n_over_p, sub_vec);

	// Llamamos a las funciones para trabajar con ellas.

	float promTotal=(subProm/comm_sz); //Se calcula el promedio total de todos los subpromedios recibidos en P=0
	printf("Promedio de Promedios: %f\n", promTotal); //Salida por pantalla del promedio total

	printf("Varianza de PdP's: %f\n", varianza(arreglo,promTotal)); //Salida por pantalla de la varianza

	float desvEstandar = sqrt(varianza(arreglo,promTotal));

	printf("La desviacion estandar de PdP's: %f\n", desvEstandar); //Salida por pantalla de la desviacion estandar      
    }

    MPI_Finalize();
    return 0;

}
// Creamos el sub_vector que contendra los  vectores con el largo dividio en la cantidad de procesos
int* crear_subVector(int begin, int end, int* origin){
    int* sub_vec;
    int size;
    int aux = 0;
    size = end - begin;
    sub_vec = (int*)malloc(size * sizeof(int));
    for (int i = begin; i < end; ++i) {
        *(sub_vec+aux) = *(origin+i);
        aux += 1;
    }
    return  sub_vec;
}

// Mostramos lo subvector y los promedios de cada sub vector
void print(int my_rank, int comm_sz, int n_over_p, int* sub_vec){
    printf("Process %d out of %d received sub_vector: [ ", my_rank, comm_sz);
    for (int i = 0; i < n_over_p; ++i)
    {
        printf("%d, ", *(sub_vec+i));	
    }
    printf("]\n");
    float prom = subPromedio(sub_vec,n_over_p);
    printf("Del procesador %d el promedio es %f\n",my_rank,prom);
    printf("\n");

}

// Llenamos el arreglo con 100.000 numeros ramdom
void Llenar_arreglo(){
    int contador,cantidad = 20;
    int hora = time(NULL);
    srand(hora);
    for(contador = 0; contador<cantidad; contador++){
        arreglo[contador] = rand()%100;
        }
}

// Calculamos la varianza del promedio de promedios
float varianza(int vector[],float promArreglo){
    float var;
    for(int i=0; i<=tam; i++){
        var+=pow(vector[i]-promArreglo,2);
	var = var/promArreglo;
    }    
    return var;
}

// Sacamos todos los promedio de los sub_vectores
float subPromedio(int *sub_vector, int largo_sv){
	int *s = sub_vector;
        float promedio;
        float suma_sv;
        for(int i = 0; i < largo_sv; i++) {
                suma_sv += *s;
                s++;
        }
        promedio = suma_sv/largo_sv;

        return promedio;
}
