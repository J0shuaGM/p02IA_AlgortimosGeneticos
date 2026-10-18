// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algoritmos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 24/09/2026
// Archivo: tools.h

#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

#include "algoritmogenetico.h"

void Usage(int argc, char *argv[]);
Parametros LeerParametros(const std::string& nombre_fichero);

#endif