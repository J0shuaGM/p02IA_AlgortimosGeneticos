// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algortimos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 12/10/2026
// Archivo: heuristica.cc, contiene la definicion de las funciones de heuristica.h

#include "heuristica.h"

#include <numeric>
#include <algorithm>
#include <limits>



/**
 * @brief Calculo de la heuristica NEH
 * @param instancia entorno sobre el que se va a calcular
 * @return vector con la mejor permutacion
*/
std::vector<int> NEH(const PFSP& instancia) {
  int numero_trabajos = instancia.getTrabajos();
  int numero_maquinas = instancia.getMaquinas();
  std::vector<int> secuencia; 
  //tiempo total de proceso de cada trabajo
  std::vector<int> total(numero_trabajos, 0);
  for(int j = 0; j < numero_trabajos; ++j) {
    for(int i = 0; i < numero_maquinas; ++i) {
      total[j] += instancia.getTiempo(j, i);
    }
  }

  //ordenar los trabajos por tiempo total decreciente
  std::vector<int> orden(numero_trabajos);
  std::iota(orden.begin(), orden.end(), 0); 

  std::stable_sort(orden.begin(), orden.end(), [&](int x, int y){ return total[x] > total[y]; });
  //quedarse con el mejor orden de orden[0] y orden[1]
  int a = orden[0]; 
  int b = orden[1]; 
  std::vector<int> temp = {a , b}; 
  std::vector<int> inverso = {b, a};
  if(instancia.Makespan(temp) <= instancia.Makespan(inverso)) {
    secuencia = temp; 
  } else {  
    secuencia = inverso;
  }

  //insertar el resto de trabajos en su mejor posicion
  for(int k = 2; k < numero_trabajos; ++k) {
    int trabajo = orden[k];
    int mejor_cmax = std::numeric_limits<int>::max();
    int mejor_pos = 0;
    for (int pos = 0; pos <= static_cast<int>(secuencia.size()); ++pos) { 
      std::vector<int> candidata = secuencia; 
      candidata.insert(candidata.begin() + pos, trabajo);
      int cmax = instancia.Makespan(candidata);
      if(cmax < mejor_cmax) {
        mejor_cmax = cmax;
        mejor_pos = pos;
      }
    }
    secuencia.insert(secuencia.begin() + mejor_pos, trabajo); 
  }
  return secuencia;
}



/**
 * @brief Calculo de la busqueda aleatoria
 * @param instancia entorno sobre el que se va a realizar la busqueda
 * @param max_evaluaciones presupuesto de evaluaciones del algoritmo
 * @param rng semilla para la aleatoriedad
 * @return Vector con la mejor permutacion encontrada
*/
std::vector<int> BusquedaAleatoria(const PFSP& instancia, int max_evaluaciones, std::mt19937& rng) {
  int numero_trabajos = instancia.getTrabajos(); 
  std::vector<int> permutacion(numero_trabajos); 
  std::iota(permutacion.begin(), permutacion.end(), 0);
  std::vector<int> mejor = permutacion;
  int mejor_cmax = std::numeric_limits<int>::max();

  //Cada iteracion consume exactamente una evaluacion del presupuesto
  for(int evaluaciones = 0; evaluaciones < max_evaluaciones; ++evaluaciones) {
    std::shuffle(permutacion.begin(), permutacion.end(), rng); //Permutacion uniforme (Fisher-Yates)
    int cmax = instancia.Makespan(permutacion);
    if(cmax < mejor_cmax) {
      mejor_cmax = cmax;
      mejor = permutacion;
    }
  }
  return mejor;
}