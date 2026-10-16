// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algoritmos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 24/09/2026
// Archivo: tools.cc

#include "tools.h"



/**
 * @brief Funcion que asegura el correcto uso de la linea de ejecucion
 * @param argc Entero que contiene el numero de argumentos pasados por linea de ejecucion
 * @param argv Array que contiene los argumentos pasados por linea de ejecucion
*/
void Usage(int argc, char* argv[]) {
  switch (argc) {
    case 2: {
      std::string parametro = argv[1];
      if (parametro == "--help") {
        std::cout << "El programa se encarga de dar solucion al Permutation Flow Shop Scheduling Problem, mediante el uso de un algortimo genetico" << std::endl; 
        std::cout << "Para ello es necesario que se le pase por linea de comandos un fichero con el entorno a simular para buscar su solucion" << std::endl;
        std::exit(EXIT_SUCCESS);
      }
      break;
    }
    default: {
      std::cerr << argv[0] << ": Modo de empleo: ./genetico entorno.txt" << std::endl;
      std::cout << "Pruebe " << argv[0] << " --help para obtener mas informacion" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}