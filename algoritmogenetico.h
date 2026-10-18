// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algortimos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 12/10/2026
// Archivo: algoritmogenetico.h, declaracion de la clase encargada de gestionar la simulacion del algortimo

#ifndef ALGORITMOGENETICO_H
#define ALGORITMOGENETICO_H

#include "entornopfsp.h"
#include "individuo.h"
#include "heuristica.h"

struct Parametros {
  //Constructor
  Parametros() {}
  Parametros(int N, double pc, double pm, int K, int max_evaluaciones, int semilla, const std::string& tipo_mutacion) : 
    tam_poblacion_(N), prob_cruce_(pc), prob_mutacion_(pm), tam_torneo_(K), max_evaluaciones_(max_evaluaciones), semilla_(semilla), tipo_mutacion_(tipo_mutacion) {}

  //Paramatros configurables
  int tam_poblacion_{100}; //N
  double prob_cruce_{0.9}; //p_c
  double prob_mutacion_{0.2}; //p_m
  int tam_torneo_{2}; //Tamaño del torneo K (2 = torneo binario)
  int max_evaluaciones_{50000}; //E (Presupuesto)
  int semilla_{1}; 
  std::string tipo_mutacion_{"insercion"}; //Insercion o intercambio
  std::string algoritmo_{"ga"}; //ga, neh o aleatoria
  int ejecuciones_{1}; //Numero de ejecuciones (semillas semilla_, semilla_+1, ...)
  std::string fichero_brutos_{""}; //CSV donde se anade una fila por ejecucion ("" = no se guardan)
};


class AlgoritmoGenetico {
  public: 
    //Constructor
    AlgoritmoGenetico(const PFSP& instancia, const Parametros& parametros);

    //Getters
    Individuo getMejorSolucion(void) const { return mejor_global_; }
    std::vector<std::pair<int, int>> getHistorial(void) const { return historial_; }
    int getEvaluaciones(void) const { return evaluaciones_actuales_; }
    std::vector<Individuo> getPoblacion(void) const { return poblacion_; }

    //Metodo que cuenta las evaluaciones y actualiza el mejor en un solo sitio
    void evaluar(Individuo& individuo);

    //Metodos para los operadores geneticos
    void inicializarPoblacion(void); //Generar aleatorios y la heuristica NEH
    Individuo seleccionTorneo(void); //Torneo de tamaño K

    //Cruce OX
    Individuo generarHijoOX(const Individuo& padreA, const Individuo& padreB, int a, int b) const; //Funcion auxiliar que recible los cortes por parametros para poder realizar pruebas
    std::pair<Individuo, Individuo> cruceOX(const Individuo& padre1, const Individuo& padre2);

    //Mutaciones
    void mutacionInsercion(Individuo& ind); 
    void mutacionIntercambio(Individuo& ind);
    void mutar(Individuo& ind);

    //Comprobacion de factibiliad
    bool esPermutacionValida(const std::vector<int>& pi) const;

    //Metodo para sustituir padre por hijo
    void reemplazo(const std::vector<Individuo>& hijos);

    //Ejecutar algortimo
    void ejecutar(void);

  private: 
    const PFSP& instancia_;

    Parametros parametros_;
    int evaluaciones_actuales_;

    std::vector<Individuo> poblacion_; 
    Individuo mejor_global_;
    std::mt19937 rng_;

    std::vector<std::pair<int, int>> historial_; //Para las graficas de convergencia en caso de hacerlas
};

#endif