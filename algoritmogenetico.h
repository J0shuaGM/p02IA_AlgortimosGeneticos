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

class AlgoritmoGenetico {
  public: 
    //Constructor
    AlgoritmoGenetico(PFSP& instancia, int N, double pc, double pm, int K, int max_evaluaciones);

    //Getters
    Individuo getMejorSolucion(void) const { return mejor_global_; }

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

    //Ejecutar algortimo
    void ejecutar(void);
  private: 
    PFSP& instancia_;

    //Paramatros configurables
    int tam_poblacion_; //N
    double prob_cruce_; //p_c
    double prob_mutacion_; //p_m
    int tam_torneo_; //Tamaño del torneo K
    int max_evaluaciones_; //E (Presupuesto)
    int evaluaciones_actuales_;

    std::vector<Individuo> poblacion_; 
    Individuo mejor_global_;
};

#endif