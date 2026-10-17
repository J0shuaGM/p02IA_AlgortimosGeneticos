// Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
// Grado en Ingenieria Informatica
// Asignatura: Inteligencia Artificial
// Curso: 3º
// Practica 2: Algoritmos geneticos
// Autor: Joshua Gomez Marrero
// Correo: alu0101477398@ull.edu.es
// Fecha: 17/10/2026
// Archivo: prueba_ag.cc, bateria de pruebas del reemplazo y del bucle ejecutar()
//
// Compilar y ejecutar:  make prueba && ./prueba_ag

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "entornopfsp.h"
#include "heuristica.h"
#include "algoritmogenetico.h"

int fallos = 0;

//Imprime OK/FALLO y lleva la cuenta de fallos
void Comprobar(bool condicion, const std::string& descripcion) {
  std::cout << (condicion ? "  [OK]    " : "  [FALLO] ") << descripcion << std::endl;
  if(!condicion) ++fallos;
}

//Devuelve los makespan de un vector de individuos como texto
std::string Makespans(const std::vector<Individuo>& v) {
  std::string texto;
  for(const Individuo& ind : v) texto += std::to_string(ind.makespan) + " ";
  return texto;
}

//Crea un individuo "falso" con solo el makespan relleno (para probar el reemplazo)
Individuo Falso(int makespan) {
  Individuo ind;
  ind.makespan = makespan;
  return ind;
}

//Comprueba todo lo que debe cumplirse tras una ejecucion completa del AG
void ComprobarEjecucion(const PFSP& instancia, const Parametros& par, AlgoritmoGenetico& ag) {
  Individuo mejor = ag.getMejorSolucion();
  std::vector<std::pair<int, int>> historial = ag.getHistorial();
  std::vector<Individuo> poblacion = ag.getPoblacion();
  double rpd = 100.0 * (mejor.makespan - instancia.getUpperBound()) / instancia.getUpperBound();

  std::cout << "  Mejor: " << mejor.makespan << " (RPD " << rpd << " %), evaluaciones: " << ag.getEvaluaciones()
            << ", mejoras en historial: " << historial.size() << std::endl;

  Comprobar(ag.getEvaluaciones() == par.max_evaluaciones_,
            "Evaluaciones == E (" + std::to_string(ag.getEvaluaciones()) + " vs " + std::to_string(par.max_evaluaciones_) + ")");
  Comprobar(ag.esPermutacionValida(mejor.permutacion), "La mejor solucion es una permutacion valida");
  Comprobar(instancia.Makespan(mejor.permutacion) == mejor.makespan, "El makespan guardado del mejor coincide con recalcularlo");
  Comprobar(static_cast<int>(poblacion.size()) == par.tam_poblacion_,
            "La poblacion final tiene N individuos (" + std::to_string(poblacion.size()) + ")");

  bool todos_validos = true;
  int minimo_poblacion = poblacion.empty() ? -1 : std::min_element(poblacion.begin(), poblacion.end())->makespan;
  for(const Individuo& ind : poblacion) {
    if(!ag.esPermutacionValida(ind.permutacion)) todos_validos = false;
  }
  Comprobar(todos_validos, "Todos los individuos de la poblacion final son validos");
  Comprobar(minimo_poblacion >= mejor.makespan, "Ningun individuo de la poblacion es mejor que el mejor global");

  Comprobar(!historial.empty() && historial[0].first == 1, "El historial empieza en la evaluacion 1 (individuo NEH)");
  bool historial_ok = true;
  for(size_t i = 1; i < historial.size(); ++i) {
    if(historial[i].first <= historial[i - 1].first || historial[i].second >= historial[i - 1].second) historial_ok = false;
  }
  Comprobar(historial_ok, "Historial: evaluaciones crecientes y makespan estrictamente decreciente");
  Comprobar(!historial.empty() && historial.back().second == mejor.makespan, "La ultima entrada del historial es el mejor global");
}

int main() {
  PFSP instancia("instancias/tai20_10_0.fsp.txt");
  const int cmax_neh = instancia.Makespan(NEH(instancia));
  const int cmax_aleatoria = 1747; //Busqueda aleatoria, semilla 1, E = 50000 (obtenido ayer)

  //------------------------------------------------------------------
  std::cout << "PRUEBA 1: reemplazo, el mejor anterior es mejor que el peor hijo" << std::endl;
  {
    Parametros par; par.tam_poblacion_ = 4;
    AlgoritmoGenetico ag(instancia, par);
    ag.inicializarPoblacion(); //Contiene al NEH (1680)
    std::vector<Individuo> hijos = {Falso(2000), Falso(2100), Falso(1900), Falso(2200)};
    std::cout << "  Poblacion antes: " << Makespans(ag.getPoblacion()) << "| hijos: " << Makespans(hijos) << std::endl;
    ag.reemplazo(hijos);
    std::vector<Individuo> p = ag.getPoblacion();
    std::cout << "  Poblacion despues: " << Makespans(p) << " (esperado: 2000 2100 1900 1680)" << std::endl;
    Comprobar(p.size() == 4, "Tamano 4");
    Comprobar(p.size() == 4 && p[0].makespan == 2000 && p[1].makespan == 2100 && p[2].makespan == 1900 && p[3].makespan == 1680,
              "El peor hijo (2200) se sustituye por el mejor anterior (1680)");
  }

  //------------------------------------------------------------------
  std::cout << "\nPRUEBA 2: reemplazo, todos los hijos son mejores que el mejor anterior" << std::endl;
  {
    Parametros par; par.tam_poblacion_ = 4;
    AlgoritmoGenetico ag(instancia, par);
    ag.inicializarPoblacion();
    std::vector<Individuo> hijos = {Falso(1500), Falso(1600), Falso(1550), Falso(1650)};
    ag.reemplazo(hijos);
    std::vector<Individuo> p = ag.getPoblacion();
    std::cout << "  Poblacion despues: " << Makespans(p) << " (esperado: 1500 1600 1550 1650)" << std::endl;
    Comprobar(p.size() == 4 && p[0].makespan == 1500 && p[1].makespan == 1600 && p[2].makespan == 1550 && p[3].makespan == 1650,
              "La poblacion son exactamente los hijos (no se mete al elite)");
  }

  //------------------------------------------------------------------
  std::cout << "\nPRUEBA 3: ejecucion completa, insercion (N=50, pc=0.9, pm=0.2, K=2, E=50000, semilla 1)" << std::endl;
  {
    Parametros par(50, 0.9, 0.2, 2, 50000, 1, "insercion");
    AlgoritmoGenetico ag(instancia, par);
    ag.ejecutar();
    ComprobarEjecucion(instancia, par, ag);
    Comprobar(ag.getMejorSolucion().makespan < cmax_neh, "Mejor que NEH (" + std::to_string(cmax_neh) + ")");
    Comprobar(ag.getMejorSolucion().makespan < cmax_aleatoria, "Mejor que la busqueda aleatoria (" + std::to_string(cmax_aleatoria) + ")");
  }

  //------------------------------------------------------------------
  std::cout << "\nPRUEBA 4: ejecucion completa, intercambio (mismos parametros)" << std::endl;
  {
    Parametros par(50, 0.9, 0.2, 2, 50000, 1, "intercambio");
    AlgoritmoGenetico ag(instancia, par);
    ag.ejecutar();
    ComprobarEjecucion(instancia, par, ag);
  }

  //------------------------------------------------------------------
  std::cout << "\nPRUEBA 5: N impar (N=51)" << std::endl;
  {
    Parametros par(51, 0.9, 0.2, 2, 50000, 1, "insercion");
    AlgoritmoGenetico ag(instancia, par);
    ag.ejecutar();
    ComprobarEjecucion(instancia, par, ag);
  }

  //------------------------------------------------------------------
  std::cout << "\nPRUEBA 6: el presupuesto se agota a mitad de generacion (N=50, E=1025)" << std::endl;
  {
    Parametros par(50, 0.9, 0.2, 2, 1025, 1, "insercion");
    AlgoritmoGenetico ag(instancia, par);
    ag.ejecutar();
    ComprobarEjecucion(instancia, par, ag);
  }

  //------------------------------------------------------------------
  std::cout << "\nPRUEBA 7: reproducibilidad (misma semilla => mismo resultado; otra semilla => otra ejecucion)" << std::endl;
  {
    Parametros par1(50, 0.9, 0.2, 2, 50000, 1, "insercion");
    Parametros par2(50, 0.9, 0.2, 2, 50000, 2, "insercion");
    AlgoritmoGenetico a(instancia, par1), b(instancia, par1), c(instancia, par2);
    a.ejecutar(); b.ejecutar(); c.ejecutar();
    std::cout << "  Semilla 1: " << a.getMejorSolucion().makespan << ", semilla 1 otra vez: " << b.getMejorSolucion().makespan
              << ", semilla 2: " << c.getMejorSolucion().makespan << std::endl;
    Comprobar(a.getMejorSolucion().permutacion == b.getMejorSolucion().permutacion, "Semilla 1 dos veces => misma solucion");
    Comprobar(a.getHistorial() != c.getHistorial(), "Semillas distintas => historiales distintos");
  }

  std::cout << "\n==============================" << std::endl;
  std::cout << (fallos == 0 ? "TODAS LAS PRUEBAS SUPERADAS" : "PRUEBAS FALLIDAS: " + std::to_string(fallos)) << std::endl;
  return fallos == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
