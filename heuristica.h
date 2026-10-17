// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algortimos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 12/10/2026
// Archivo: heuristica.h, contiene la declaracion de las funiones heuristicas solicitadas

#ifndef HEURISTICA_H
#define HEURISTICA_H

#include <vector>
#include <random>

#include "entornopfsp.h"

std::vector<int> NEH(const PFSP& instancia);
std::vector<int> BusquedaAleatoria(const PFSP& instancia, int max_evaluaciones, std::mt19937& rng);

#endif
