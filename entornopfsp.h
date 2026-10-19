// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algortimos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 12/10/2026
// Archivo: entornopfsp.h, calse encargada de gestionar el entorno, leera los ficheros de Taillard y proporcina la funcion para calcular el makespan

#ifndef PFSP_H
#define PFSP_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class PFSP {
  public: 
    //Costructor
    PFSP(const std::string& nombre_fichero); //Encargado de leer el fichero

    //Getters
    int getTrabajos(void) const { return numero_trabajos_; }
    int getMaquinas(void) const { return numero_maquinas_; }
    int getTiempo(int trabajo, int maquina) const { return tiempo_proceso_[maquina][trabajo]; }
    int getUpperBound(void) const { return upper_bound_; }
    bool tieneUpperBound(void) const { return upper_bound_ > 0; } //False en el formato simple (sin cotas)
    int getSemilla(void) const { return semilla_inicial; }

    //Metodos
    int Makespan(const std::vector<int>& pi) const;
  private: 
    int numero_trabajos_, numero_maquinas_;
    int semilla_inicial; 
    int upper_bound_, lower_bound_;
    std::vector<std::vector<int>> tiempo_proceso_; //Matriz de tiempo de procesamientos
};

#endif