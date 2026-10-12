// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algortimos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 12/10/2026
// Archivo: individuo.h, cada inidividuo es una solucion al problema, se almacenan los datos del mismo

#include <iostream>
#include <vector>
#include <algorithm>

struct Individuo {
  std::vector<int> permutacion; //Secuencia de trabajo (5, 6, 1, 3 ...)
  int makespan; //Valor objetivo a minimizar

  //Constructor
  Individuo(int n) : permutacion(n), makespan(-1) {}

  //Sobrecarga del operador <
  bool operator<(const Individuo& otro) {
    return this->makespan < otro.makespan;
  }
};
