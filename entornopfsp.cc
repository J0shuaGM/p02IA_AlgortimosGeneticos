// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algortimos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 12/10/2026
// Archivo: entornopfsp.cc, definicion de la clase objeto pfsp

#include "entornopfsp.h"

#include <algorithm>
#include <cstdlib>

PFSP::PFSP(const std::string& nombre_fichero) {
  std::ifstream fichero(nombre_fichero);
  if(!fichero.is_open()) {
    std::cerr << "El fichero de entrada no se ha podido abrir" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string linea;
  std::getline(fichero, linea); //Saltar la linea de texto "number of jobs, ..."
  fichero >> numero_trabajos_ >> numero_maquinas_ >> semilla_inicial >> upper_bound_ >> lower_bound_;
  std::getline(fichero, linea); //Consumir el salto de linea que queda tras los numeros
  std::getline(fichero, linea); //Saltar la linea de texto "processing times :"

  //Matriz m x n: tiempo_proceso_[maquina][trabajo]
  tiempo_proceso_.assign(numero_maquinas_, std::vector<int>(numero_trabajos_));
  for(int maquina = 0; maquina < numero_maquinas_; ++maquina) {
    for(int trabajo = 0; trabajo < numero_trabajos_; ++trabajo) {
      fichero >> tiempo_proceso_[maquina][trabajo];
    }
  }
}

int PFSP::Makespan(const std::vector<int>& pi) const {
  //C[i] = instante en que la maquina i termina el ultimo trabajo procesado hasta ahora
  std::vector<int> C(numero_maquinas_, 0);

  for(int trabajo : pi) {
    //Maquina 0: solo espera a que ella misma quede libre
    C[0] += tiempo_proceso_[0][trabajo];
    //Resto: espera a que la maquina quede libre (C[i]) y a que el trabajo salga de la anterior (C[i-1])
    for(int i = 1; i < numero_maquinas_; ++i) {
      C[i] = std::max(C[i], C[i - 1]) + tiempo_proceso_[i][trabajo];
    }
  }

  return C[numero_maquinas_ - 1]; //Fin del ultimo trabajo en la ultima maquina
}
