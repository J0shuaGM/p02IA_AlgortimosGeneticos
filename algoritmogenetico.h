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
  int tam_poblacion_{10}; //N
  double prob_cruce_{1}; //p_c
  double prob_mutacion_{1}; //p_m
  int tam_torneo_{2}; //Tamaño del torneo K (2 = torneo binario)
  int max_evaluaciones_{50000}; //E (Presupuesto)
  int semilla_{1}; 
  std::string tipo_mutacion_{"intercambio"}; //Insercion o intercambio
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
    std::pair<Individuo, Individuo> cruceOX(const Individuo& p1, const Individuo& p2);

    //Mutaciones
    void mutacionInsercion(Individuo& ind); 
    void mutacionIntercambio(Individuo& ind);

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