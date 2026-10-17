// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algortimos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 12/10/2026
// Archivo: algoritmogenetico.h, definicion de la clase objeto algoritmogenetico

#include "algoritmogenetico.h"

#include <numeric>
#include <algorithm>
#include <limits>

AlgoritmoGenetico::AlgoritmoGenetico(const PFSP& instancia, const Parametros& parametros) 
  : instancia_(instancia), parametros_(parametros), evaluaciones_actuales_(0), rng_(parametros.semilla_) {
  mejor_global_.makespan = std::numeric_limits<int>::max(); //Cualquier primer individuo sera mejor
}

void AlgoritmoGenetico::evaluar(Individuo& individuo) {
  individuo.makespan = instancia_.Makespan(individuo.permutacion); 
  evaluaciones_actuales_ += 1; 
  if(individuo.makespan < mejor_global_.makespan) {
    mejor_global_ = individuo; 
    historial_.push_back({evaluaciones_actuales_, individuo.makespan});
  }
}

void AlgoritmoGenetico::inicializarPoblacion(void) {
  poblacion_.clear();
  //Inicializar un inidividuo NEH
  Individuo individuo;
  individuo.permutacion = NEH(instancia_); 
  evaluar(individuo); 
  poblacion_.push_back(individuo); 

  //Inicializar N-1 inidividuos aleatorios
  while(static_cast<int>(poblacion_.size()) < parametros_.tam_poblacion_) {
    Individuo aleatorio(instancia_.getTrabajos());
    std::iota(aleatorio.permutacion.begin(), aleatorio.permutacion.end(), 0); 
    std::shuffle(aleatorio.permutacion.begin(), aleatorio.permutacion.end(), rng_);
    evaluar(aleatorio);
    poblacion_.push_back(aleatorio);
  }
}