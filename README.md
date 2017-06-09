# BMC-Proyecto1
Proyecto de Introducción a la Biología Molecular Computacional.
# Proposito
 Crear un programa que muestre el crossover existente entre genes.

Tecnológico de Costa Rica IC8022 - Introducción A La Biología Molecular Computacional

Profesor: Dr. Francisco Torres

Estudiantes: Carlos Girón Alas, Amanda Solano Astorga

Programa que crea mapas cromosómicos a partir de las probabilidades de cruces entre los genes dados por el usuario.

Los archivos adjuntos son 
-image.c
-mapas.c
-mapas.h
-cruces.glade
-cruces.glade~
-makefile
*Carpeta "style" con para detalles visuales
*Carpeta "Investigación" la cual contiene el pdf de la investigación y un zip con los archivos fuente: .tex de la investigación y las imágenes utilizadas en la investigación.
*Carpeta "examples" la cual contiene varios archivos de pruebas.

Para el archivo de casos: para cada gen, será dado por una descripción de a lo mucho 4 letras.
Formato de archivo de datos: se basa en una tabla en la cual los datos debajo de la identidad deben estar en blanco, y si no se tiene un dado debe ponerse un "-", no deben haber espacios entre los datos:
	-Primer fila será de los nombres de los genes separados por ";".
	-Probabilidad de cruces entre G1 y los demás genes, separados por ";".
	-Probabilidad de cruces entre el segundo gen y los demás genes, separados por ";".
	...
	-Probabilidad de cruces entre el gen N y los demás genes, separados por ";".

Ejemplo:
A;B;C;D;
-;-;0,35;0,2;
;-;0,3;-;
;;-;-;
;;;-;