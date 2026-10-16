// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algoritmos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 24/09/2026
// Archivo: cliente.cc, programa principal

#include <iostream>

#include "tools.h"
#include "individuo.h"
#include "entornopfsp.h"
#include "algoritmogenetico.h"

int main(int argc, char* argv[]) {
  Usage(argc, argv);
  PFSP instancia(argv[1]);

  std::cout << "Trabajos: " << instancia.getTrabajos() << ", Maquinas: " << instancia.getMaquinas() << std::endl;

  //Prueba temporal: makespan de la secuencia 1, 2, ..., n
  std::vector<int> identidad(instancia.getTrabajos());
  for(int i = 0; i < instancia.getTrabajos(); ++i) {
    identidad[i] = i;
  }
  std::cout << "Makespan (1, 2, ..., n): " << instancia.Makespan(identidad) << std::endl;

  return EXIT_SUCCESS;
}
