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
#include <cassert>

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

Individuo AlgoritmoGenetico::seleccionTorneo() {
  std::uniform_int_distribution<int> distribucion(0, parametros_.tam_poblacion_ - 1); 
  int indiceAleatorio = distribucion(rng_); 
  Individuo mejor = poblacion_[indiceAleatorio];
  
  //El primer participante ya esta elegido: faltan K-1
  for(int i = 1; i < parametros_.tam_torneo_; ++i) {
    int candidatoAleatorio = distribucion(rng_);
    const Individuo& candidato = poblacion_[candidatoAleatorio]; //Referencia: solo se compara, no se copia
    if(candidato.makespan < mejor.makespan) {
      mejor = candidato; 
    }
  }
  return mejor; 
}

Individuo AlgoritmoGenetico::generarHijoOX(const Individuo& padreA, const Individuo& padreB, int a, int b) const {
  int tamPadreA = padreA.permutacion.size(); 
  Individuo hijo(tamPadreA); 
  std::vector<bool> usado(tamPadreA, false); 

  //Copiar el segmento del padre A
  for(int i = a; i <= b; ++i) {
    hijo.permutacion[i] = padreA.permutacion[i];
    usado[padreA.permutacion[i]] = true;
  }

  //Rellenar huecos con padreB
  int pos{0}; 
  for(auto trabajo : padreB.permutacion) {
    if(!usado[trabajo]) {
      if(pos == a) pos = b + 1;
      hijo.permutacion[pos] = trabajo;
      pos = pos + 1;
    }
  }
  return hijo;
}

std::pair<Individuo, Individuo> AlgoritmoGenetico::cruceOX(const Individuo& padre1, const Individuo& padre2) {
  std::uniform_int_distribution<int> distribucion(0, static_cast<int>(padre1.permutacion.size()) - 1); //Rango cerrado [0, n-1]
  int indiceAleatorioA = distribucion(rng_);
  int indiceAleatorioB = distribucion(rng_);
  if(indiceAleatorioA > indiceAleatorioB) std::swap(indiceAleatorioA, indiceAleatorioB); //Garantizar a <= b
  Individuo hijoA = generarHijoOX(padre1, padre2, indiceAleatorioA, indiceAleatorioB);
  Individuo hijoB = generarHijoOX(padre2, padre1, indiceAleatorioA, indiceAleatorioB);

  assert(esPermutacionValida(hijoA.permutacion));
  assert(esPermutacionValida(hijoB.permutacion));
  
  return {hijoA, hijoB};
}

bool AlgoritmoGenetico::esPermutacionValida(const std::vector<int>& pi) const {
  if(static_cast<int>(pi.size()) != instancia_.getTrabajos()) return false;
  std::vector<bool> visto(instancia_.getTrabajos(), false);
  for(auto trabajo : pi){
    if(trabajo < 0 || trabajo >= instancia_.getTrabajos()) return false;
    if(visto[trabajo]) return false;
    visto[trabajo] = true;
  }
  return true;
}