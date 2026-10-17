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
#include "heuristica.h"

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

  //Prueba de NEH
  std::vector<int> secuencia = NEH(instancia);
  std::cout << "NEH: ";
  for(int trabajo : secuencia) {
    std::cout << trabajo + 1 << " ";
  }
  int cmax = instancia.Makespan(secuencia);
  double rpd = 100.0 * (cmax - instancia.getUpperBound()) / instancia.getUpperBound();
  std::cout << "-> Cmax = " << cmax << ", RPD = " << rpd << " %" << std::endl;

  //Primera prueba AG iniciar poblacion
  Parametros param;
  AlgoritmoGenetico prueba(instancia, param);
  prueba.inicializarPoblacion();
  int contador{0};
  for(int i = 0; i < 10000; ++i) {
    Individuo torneo = prueba.seleccionTorneo();
    if(torneo.makespan == 1680) ++contador;
  }
  std::cout << contador << std::endl;
  std::vector<Individuo> poblacion = prueba.getPoblacion(); 
  for(Individuo indi : poblacion) {
    std::cout << indi.makespan << " ";
  }
  std::cout << std::endl;
  std::cout << "Evaluaciones totales: " << prueba.getEvaluaciones() << std::endl;
  std::cout << "Mejor global: " << prueba.getMejorSolucion().makespan << std::endl;
  std::vector<std::pair<int, int>> historial = prueba.getHistorial(); 
  for(auto entrada : historial) {
    std::cout << entrada.first << "/" << entrada.second << ", ";
  }
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
