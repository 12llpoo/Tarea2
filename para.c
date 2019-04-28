#include<stdio.h>  // libreria basica para entrada y salida
#include<stdlib.h> // libreria para el uso de rand()
//#include<conio.h>  // libreria para el uso de getch()
#include<time.h>   // libreria para el uso de time()
#include<math.h>

int arreglo[100000],tam=100000;

void Llenar_arreglo(){
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

  Llenar_arreglo();
  float prom = calcularPromedio();
  float vari = varianza();
  float desve = desviacionEstandar();
  printf("\n\nEl Promedio es: %f \n", prom);
  printf("\n\nLa Varianza es: %f \n", vari);
  printf("\n\nLa Desviacion estandar es: %f \n", desve);

}


