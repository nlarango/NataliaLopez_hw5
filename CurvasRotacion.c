#include <stdio.h>
#include <math.h>
#include <stdlib.h>


//AQUI DEFINO VARIABLES USADAS ANTES DEL MAIN
#define datosObservados 300 
int i;


//DEFINICION DE METODOS PARA LLAMARLOS EN EL MAIN, METODO COMO EN EL REPOSITORIO
long double repositorio(double Valoresiniciales[datosObservados], double Valoresmodelo[datosObservados])
{
  double chi = 0.0;
  for( i=0;i<datosObservados;i++)
  {
    chi = chi + pow((Valoresiniciales[i]-Valoresmodelo[i]) / 100.0,2.0);
  }
  return exp(-chi/2.0);
}


//AQUI SE HACE UNA DISTRIBUCION NORMAL CON NUMEROS ALEATORIOS.ENCONTRADO EN INTERNET COMO EL METODO DE MULLER

double DistribucionNAleatoria(double miu, double a)
{
  double aleatorio1 = drand48();
  double aleatorio2 = drand48();
  double distribucionNormal = sqrt(-2.0*log(aleatorio1))*cos(2*(3.1415)*aleatorio2);

  return ((a*distribucionNormal) + miu);
}


// AQUI SE REEMPLAZAN LOS VALORES HALLADOS EN LA ECUACION DADA EN EL ENUNCIADO



double * velValoresmodelo(double R[datosObservados], double masaBulbo, double masaDisco, double masaHalo)
{

//VALORES DADOS, ASOCIADOS A LAS DIMENSIONES DE BULBO DISCO Y HALO
double bbulbo = 0.2497;
double bdisco = 5.16;
double adisco = 0.3105;
double ahalo = 64.3;
double * velModelo = malloc(datosObservados*sizeof(double));


//AQUI HAGO LA FORMULA DADA, SE REEMPLAZAN LOS VALORES PARA HALLAR CADA TERMINO Y SUMO PARA HALLAR LA VELOCIDAD CIRCULAR

for(i=0; i<datosObservados; i++)
{
    	double bulbo = sqrt(masaBulbo) * R[i] / pow((pow(R[i],2.0)+pow(bbulbo,2.0)), 0.75);
    	double disco = sqrt(masaDisco) * R[i] / pow((pow(R[i],2.0)+pow(bdisco+adisco,2.0)), 0.75);
    	double halo = sqrt(masaHalo) / pow((pow(R[i],2.0)+pow(ahalo,2.0)), 0.25);
    	velModelo[i] = bulbo + disco + halo;
}
    
  return velModelo;
}


int main(void)
{

	//AQUI LEO EL ARCHIVO CON LOS DATOS DADOS
	FILE * archivo1; 
	archivo1 = fopen("RadialVelocities.dat","r");
	fscanf(archivo1, "%*[^\n]");  

	//AQUI HAGO EL ARCHIVO QUE SE VA A GENERAR
	FILE * archivo2; 
	archivo2 = fopen("ValoresObtenidos.txt","w");	

	//AQUI DEFINO VARIABLES USADAS
	int j;
  	int iteraciones = 40000; 
  

 	 //AQUI DEFINO VECTORES USADOS TAMAÑO DATOS OBSERVADOS

  	double * velinicial = malloc(datosObservados*sizeof(double));
  	double * velprime = malloc(datosObservados*sizeof(double));   
  	double * rinicial = malloc(datosObservados*sizeof(double));
  	double * velValoresiniciales = malloc(datosObservados*sizeof(double));


 	 //AQUI DEFINO VECTORES USADOS TAMAÑO ITERACIONES

  	double * paso = malloc(iteraciones*sizeof(double));
  	double * masaBulboPaso = malloc(iteraciones*sizeof(double));
  	double * masaDiscoPaso = malloc(iteraciones*sizeof(double));
	double * masaHaloPaso = malloc(iteraciones*sizeof(double));

   	
  	for (j=0; j<datosObservados; j++) 
    	{
      	fscanf(archivo1,"%lf %lf\n", &rinicial[j],&velValoresiniciales[j]);
    	}


	// EL PASO EN CADA VARIABLE SE ASIGNA CON NUMEROS ALEATORIOS
  	
	masaBulboPaso[0] = drand48();
  	masaDiscoPaso[0] = drand48();
  	masaHaloPaso[0] = drand48();


	//AQUI INICIALIZO EL PRIMER VALOR DE LA FUNCION QUE ES EL MODELO DE APROXIMACION
	
 	velinicial = velValoresmodelo(rinicial, masaBulboPaso[0], masaDiscoPaso[0], masaHaloPaso[0]);
  	paso[0] = repositorio(velValoresiniciales, velinicial);

  	for(j=0; j<iteraciones; j++)
	{
	
	//AQUI INICIALIZO EL RESTO DE VALORES DE LA FUNCION QUE ES EL MODELO DE APROXIMACION
    	velinicial = velValoresmodelo(rinicial, masaBulboPaso[j], masaDiscoPaso[j], masaHaloPaso[j]);


	//AQUI TENGO NUMEROS ALEATORIOS CON DISTRIBUCION NORMAL, LOS NUMEROS DE LA DERECHA SON LAS 		DESVIACIONES ESTANDAR QUE SE HALLA PROBANDO NUMEROS 

    	double masaBulboA = DistribucionNAleatoria(masaBulboPaso[j], 1000.0);
    	double masaDiscoA = DistribucionNAleatoria(masaDiscoPaso[j], 1000.0);
    	double masaHalosA = DistribucionNAleatoria(masaHaloPaso[j], 1000.0);


	//AQUI APROXIMO LA FUNCION MODELO PERO CON VALORES ALEATORIOS DE LA DISTRIBUCION NORMAL
	
    	velprime = velValoresmodelo(rinicial, masaBulboA, masaDiscoA, masaHalosA);

 	//AQUI INICIALIZO LA FUNCION LIKELIHOOD Y SE OBTENGO VALORES CON LOS DEL MODELO USADO

    	long double paso1 = repositorio(velValoresiniciales, velprime);
	long double paso0 = repositorio(velValoresiniciales, velinicial);	
    	double sirveOno = paso1 / paso0;

  

    	if(sirveOno >= 1.0)
	{
      	masaBulboPaso[j+1] = masaBulboA;
      	masaDiscoPaso[j+1] = masaDiscoA;
      	masaHaloPaso[j+1] = masaHalosA;
      	paso[j+1] = paso1;
    	}

 	else 
	{
      	double beta = drand48();
      	if(beta <= sirveOno)
      	{
	masaBulboPaso[j+1] = masaBulboA;
	masaDiscoPaso[j+1] = masaDiscoA;
	masaHaloPaso[j+1] = masaHalosA;
	paso[j+1] = paso1;
      	} 
      	else 
      	{
	masaBulboPaso[j+1] = masaBulboPaso[j];
	masaDiscoPaso[j+1] = masaDiscoPaso[j];
	masaHaloPaso[j+1] = masaHaloPaso[j];
	paso[j+1] = paso[j];
      	}
    	}      
  	}

	//AQUI SE SABE CUALES VALORES SIRVEN Y CUALES NO SEGUN LOS MAXIMOS Y MINIMOS DE LA FUNCION 		REPOSITORIO

 	int k = 0;
  	double maximo = paso[0];
  	for(j=1; j<iteraciones; j++)
   	{
    	if(paso[j] >= maximo)
    	{
     	maximo = paso[j];
      	k = j;
    	}
 	}

 
	//AQUI SE PONEN LOS RESULTADOS EN LA TERMINAL Y EL ARCHIVO A GRAFICAR EN PYTHON

  	printf("Mb: %lf\n", masaBulboPaso[k]);
  	printf("Md: %lf\n", masaDiscoPaso[k]);
  	printf("Mh: %lf\n", masaHaloPaso[k]);
	fprintf(archivo2, "%f %f %f", masaBulboPaso[k], masaDiscoPaso[k], masaHaloPaso[k]);
 
}
