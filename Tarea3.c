#include<stdio.h>  // libreria basica para entrada y salida
#include<stdlib.h> // libreria para el uso de rand()
//#include<conio.h>  // libreria para el uso de getch()
#include<time.h>   // libreria para el uso de time()
#include<math.h>
#include<mpi.h>

int arreglo[100000],tam=100000;

int Llenar_arreglo(){
    // Declaracion de variables
    int contador,cantidad = 100000;
    int hora = time(NULL);

    // Semilla de rand();
    srand(hora);

    for(contador = 0; contador<cantidad; contador++){
        arreglo[contador] = rand()%100;
               printf("%d ", arreglo[contador]);
        }
}

float calcularPromedio(){
	float suma=0;
	for(int i=0;i<tam;i++){
		suma += arreglo[i];
	}
    return suma/tam;
}

float varianza(){
    float media = calcularPromedio();
    float var = 0;
    for(int i=0; i<tam; i++){
        var+=pow(arreglo[i]-media,2);
    }
    var /= tam;
    return var;
}

float desviacionEstandar(){
    return sqrt(varianza());
}

int main( )
{
    MPI_Init(NULL, NULL); //inicializacion

    int procesador; 
    int tamano; 
    int numero = 0; // variable a enviar y/o recibir
    float numero_promedio = 0.0;
    float numero_varianza = 0.0;
    float numero_dEstandar = 0.0;
    
    MPI_Comm_size(MPI_COMM_WORLD, &tamano); // guardamos el tamaño de procesadores a trabajar
    MPI_Comm_rank(MPI_COMM_WORLD, &procesador); // procesador en tiempo x
    
    /* MPI_Send(&dato_a_enviar,
 		contador(0/1),
		 tipo_de_dato,
		destino,
		 tag(0/1),
		 comunicador)
	MPI_Recv(dato_a_recibir,
		 contador,
		 tipo_de_dato,
		fuente_de_viene,
		tag,
		comunicador,
		status)
	*/
    
    if(procesador == 0){
		numero = Llenar_arreglo();
                
                numero_promedio = calcularPromedio();
                printf("\n\nEl Promedio es: %f \n", numero_promedio);
                
                numero_varianza = varianza();
                printf("\n\nLa Varianza es: %f \n", numero_varianza);
                
                numero_dEstandar = desviacionEstandar();
                printf("\n\nLa Desviacion Estandar es: %f \n", numero_dEstandar);
                
		int procesador_a_enviar = 1;
                int procesador_a_enviar_prom = 2;
                int procesador_a_enviar_vari = 3;
                int procesador_a_enviar_desv = 4;
                
		MPI_Send(&numero, 1, MPI_INT,procesador_a_enviar, 0 , MPI_COMM_WORLD);
                MPI_Send(&numero_promedio, 1, MPI_FLOAT,procesador_a_enviar_prom, 0 , MPI_COMM_WORLD);
                MPI_Send(&numero_varianza, 1, MPI_FLOAT,procesador_a_enviar_vari, 0 , MPI_COMM_WORLD);
                MPI_Send(&numero_dEstandar, 1, MPI_FLOAT,procesador_a_enviar_desv, 0 , MPI_COMM_WORLD);
	}
        if(procesador == 1){
		int procesador_a_recibir = 0;
		MPI_Recv(&numero, 1, MPI_INT,procesador_a_recibir,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if(procesador == 2){
		int procesador_a_recibir = 0;
		MPI_Recv(&numero_promedio, 1, MPI_FLOAT,procesador_a_recibir,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
        if(procesador == 3){
		int procesador_a_recibir = 0;
		MPI_Recv(&numero_varianza, 1, MPI_FLOAT,procesador_a_recibir,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
        if(procesador == 4){
		int procesador_a_recibir = 0;
		MPI_Recv(&numero_dEstandar, 1, MPI_FLOAT,procesador_a_recibir,0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
		
	printf("soy el numero %d, y tbn soy el procesador %d\n", numero, procesador);
	
        MPI_Finalize();

  //Llenar_arreglo();
  //float prom = calcularPromedio();
  //float vari = varianza();
  //float desve = desviacionEstandar();
  //printf("\n\nEl Promedio es: %f \n", prom);
  //printf("\n\nLa Varianza es: %f \n", vari);
  //printf("\n\nLa Desviacion estandar es: %f \n", desve);

}