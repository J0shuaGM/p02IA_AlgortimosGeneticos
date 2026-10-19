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
#include <fstream>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <algorithm>

#include "tools.h"
#include "individuo.h"
#include "entornopfsp.h"
#include "algoritmogenetico.h"
#include "heuristica.h"

int main(int argc, char* argv[]) {
  Usage(argc, argv);
  PFSP instancia(argv[1]);
  Parametros param = LeerParametros(argv[2]);
  //Nombre de la instancia sin ruta ni extension: "instancias/tai20_5_0.fsp" -> "tai20_5_0"
  std::string nombre_fichero = argv[1];
  nombre_fichero = nombre_fichero.substr(nombre_fichero.find_last_of('/') + 1);
  nombre_fichero = nombre_fichero.substr(0, nombre_fichero.find('.'));
  
  int UB = instancia.getUpperBound(); 
  int cmax_neh = instancia.Makespan(NEH(instancia)); 

  std::ofstream brutos; //Solo se abre si se ha indicado un fichero de brutos
  if(!param.fichero_brutos_.empty()) brutos.open(param.fichero_brutos_, std::ios::app);
  brutos << std::fixed << std::setprecision(2);
  std::vector<int> cmaxs; 
  double tiempo_total{0};
  std::vector<int> solucion;

  for(int r = 0; r < param.ejecuciones_; ++r) {
    Parametros param_r = param; 
    param_r.semilla_ = param_r.semilla_ + r;
    double tiempo_cpu_inicio = std::clock(); 

    if(param_r.algoritmo_ == "ga") {
      AlgoritmoGenetico ag(instancia, param_r); 
      ag.ejecutar();
      solucion = ag.getMejorSolucion().permutacion;
    } else if(param_r.algoritmo_ == "neh") {
      solucion = NEH(instancia); 
    } else if(param_r.algoritmo_ == "aleatoria") {
      std::mt19937 rng(param_r.semilla_);
      solucion = BusquedaAleatoria(instancia, param_r.max_evaluaciones_, rng);
    } else {
      std::cerr << "Algoritmo desconocido: " << param_r.algoritmo_ << std::endl; 
      std::exit(EXIT_FAILURE);
    }

    double tiempo = (std::clock() - tiempo_cpu_inicio) / CLOCKS_PER_SEC; //Ticks de reloj -> segundos
    int cmax = instancia.Makespan(solucion);
    cmaxs.push_back(cmax); 
    tiempo_total += tiempo;
    if(brutos.is_open()) { //CSV: instancia,semilla,cmax,rpd_ub,cpu_time_s
      brutos << nombre_fichero << "," << param_r.semilla_ << "," << cmax << ",";
      if(instancia.tieneUpperBound()) brutos << 100.0 * (cmax - UB) / UB;
      else brutos << "NULL"; //Instancia sin UB: el RPD no se puede calcular
      brutos << "," << tiempo << std::endl;
    }
  }
  //Calculo del mejor cmax
  int best_cmax = *std::min_element(cmaxs.begin(), cmaxs.end());
  //Calculo de la media
  double media{0}; 
  for(int cmax : cmaxs) {
    media += cmax;
  }
  media = media/param.ejecuciones_;
  //Calculo de la desviacion tipica
  double desviacion_tipica{0};
  if (param.ejecuciones_ != 1) { //Desviacion tipica muestral (R - 1)
    double suma_cuadrados{0};
    for(int cmax : cmaxs) {
      suma_cuadrados += (cmax - media) * (cmax - media);
    }
    desviacion_tipica = std::sqrt(suma_cuadrados / (param.ejecuciones_ - 1));
  }

  //Fila de la tabla (CSV): instancia,n,m,best_cmax,avg_cmax,std_cmax,cmax_neh,rpd_ub,rpd_neh,cpu_time_s
  double rpd_neh = 100.0 * (best_cmax - cmax_neh) / cmax_neh; //Negativo = mejora a NEH
  std::cout << std::fixed << std::setprecision(2);
  std::cout << nombre_fichero << "," << instancia.getTrabajos() << "," << instancia.getMaquinas() << ","
            << best_cmax << "," << media << "," << desviacion_tipica << "," << cmax_neh << ",";
  if(instancia.tieneUpperBound()) std::cout << 100.0 * (best_cmax - UB) / UB;
  else std::cout << "NULL"; //Instancia sin UB: el RPD no se puede calcular
  std::cout << "," << rpd_neh << "," << tiempo_total / param.ejecuciones_ << std::endl;
  return EXIT_SUCCESS;
}
