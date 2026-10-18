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
  if(argc == 2 && std::string(argv[1]) == "--help") {
    std::cout << "El programa resuelve el Permutation Flow Shop Scheduling Problem (PFSP) con un algoritmo genetico," << std::endl;
    std::cout << "la heuristica NEH o una busqueda aleatoria." << std::endl;
    std::cout << "Modo de empleo: " << argv[0] << " <instancia> <fichero_parametros>" << std::endl;
    std::cout << "  instancia:          fichero de Taillard (p.ej. instancias/tai20_10_0.fsp.txt)" << std::endl;
    std::cout << "  fichero_parametros: pares 'clave valor' (p.ej. parametros/pam1.txt). Claves:" << std::endl;
    std::cout << "    algoritmo (ga|neh|aleatoria), N, pc, pm, K, E, mutacion (insercion|intercambio), semilla, ejecuciones, brutos (ruta CSV)" << std::endl;
    std::cout << "    Lo que sigue a '#' es un comentario. Las claves omitidas toman su valor por defecto." << std::endl;
    std::exit(EXIT_SUCCESS);
  }
  if(argc != 3) {
    std::cerr << argv[0] << ": Modo de empleo: " << argv[0] << " <instancia> <fichero_parametros>" << std::endl;
    std::cerr << "Pruebe " << argv[0] << " --help para obtener mas informacion" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

/**
 * @brief Lee un fichero de parametros con pares "clave valor" (lo que sigue a '#' es comentario)
 * @param nombre_fichero Ruta del fichero de parametros
 * @return Parametros leidos; las claves que no aparecen conservan su valor por defecto
*/
Parametros LeerParametros(const std::string& nombre_fichero) {
  Parametros param; //Sin parentesis: "Parametros param();" declararia una funcion
  std::ifstream fichero(nombre_fichero);
  if(!fichero.is_open()) {
    std::cerr << "El fichero de parametros " << nombre_fichero << " no se ha podido abrir" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string clave, resto_linea;
  while(fichero >> clave) {
    if(clave[0] == '#') { //Comentario: se descarta el resto de la linea
      std::getline(fichero, resto_linea);
      continue;
    }
    if(clave == "algoritmo") fichero >> param.algoritmo_;
    else if(clave == "N") fichero >> param.tam_poblacion_;
    else if(clave == "pc") fichero >> param.prob_cruce_;
    else if(clave == "pm") fichero >> param.prob_mutacion_;
    else if(clave == "K") fichero >> param.tam_torneo_;
    else if(clave == "E") fichero >> param.max_evaluaciones_;
    else if(clave == "mutacion") fichero >> param.tipo_mutacion_;
    else if(clave == "semilla") fichero >> param.semilla_;
    else if(clave == "ejecuciones") fichero >> param.ejecuciones_;
    else if(clave == "brutos") fichero >> param.fichero_brutos_;
    else {
      std::cerr << "Clave desconocida en " << nombre_fichero << ": " << clave << std::endl;
      std::exit(EXIT_FAILURE);
    }
    if(fichero.fail()) { //Por ejemplo "N cincuenta"
      std::cerr << "Valor no valido para la clave " << clave << " en " << nombre_fichero << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
  return param;
}
