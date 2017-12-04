import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import mpl_toolkits.mplot3d.axes3d as orb

#LECTURA DE ARCHIVOS
archivo = np.genfromtxt("RadialVelocities.dat", skip_header=0)
archivoGenerado= np.genfromtxt("ValoresObtenidos.txt")

columna1=archivo[:,0]
columna2=archivo[:,1]
bulbo=archivoGenerado[0]
disco=archivoGenerado[1]
halo=archivoGenerado[2]

#AQUI SON LAS CONSTANTES DADAS
bb=0.2497
bd=5.16
bh=0.3105
ah=64.3

#METODO IGUAL AL DE EL ARCHIVO DE C PARA HALLAR LA VELOCIDAD
def ecuacion(bulbo,disco,halo):

    vector=np.zeros(len(columna1))
    for i in range (0,300):
        bulbo1 = (np.sqrt(bulbo)*columna1[i])/(((columna1[i])**2.0+(bb)**2.0)**(3.0/4.0))
        disco1 = (np.sqrt(disco)*columna1[i])/(((columna1[i])**2.0+(bd+bh)**2.0)**(3.0/4.0))
        halo1 = (np.sqrt(halo)) / (((columna1[i])**2.0+(ah)**2.0)**(1.0/4.0))
        suma = bulbo1 + disco1 + halo1
        vector[i] = suma
    
    return vector

resultado=ecuacion(bulbo,disco,halo)


#GRAFICA
plt.plot(columna1,columna2, c='palevioletred')
plt.plot(columna1,resultado, c='forestgreen')
plt.xlabel('Datos del radio')
plt.ylabel('Datos de velocidad')
plt.title(' Datos dados y ajuste encontrado')
plt.grid()
plt.savefig('Grafica.png')
plt.close()
