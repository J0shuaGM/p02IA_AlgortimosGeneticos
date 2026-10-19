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

/**
 * @brief Constructor, inicializa el algoritmo con la instancia y los parametros configurados
 * @param instancia entorno PFSP sobre el que se ejecuta el algoritmo
 * @param parametros parametros del AG (N, pc, pm, K, E, semilla, tipo de mutacion...)
*/
AlgoritmoGenetico::AlgoritmoGenetico(const PFSP& instancia, const Parametros& parametros) 
  : instancia_(instancia), parametros_(parametros), evaluaciones_actuales_(0), rng_(parametros.semilla_) {
  mejor_global_.makespan = std::numeric_limits<int>::max(); //Cualquier primer individuo sera mejor
}



/**
 * @brief Metodo para evaluar un Individuo y actualizar el mejor en caso de ser necesario
 * @param individuo individuo a ser evaluado
*/
void AlgoritmoGenetico::evaluar(Individuo& individuo) {
  individuo.makespan = instancia_.Makespan(individuo.permutacion); 
  evaluaciones_actuales_ += 1; 
  if(individuo.makespan < mejor_global_.makespan) {
    mejor_global_ = individuo; 
    historial_.push_back({evaluaciones_actuales_, individuo.makespan});
  }
}



/**
 * @brief Metodo encargado de inicializar la poblacion, 1 NEH n -1 aleatorias
*/
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



/**
 * @brief metodo para elejir un participante al azar y k - 1 con reemplazo
 * @return Devuelve el individuo con menor makespan
*/
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



/**
 * @brief metodo encargado de realizar el cruce
 * @param padreA, padreB padres entre los que se va ralizar el cruce
 * @param a, b segemento de corte
 * @return Individuo resultante del cruce
*/
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



/**
 * @brief Metodo encargado de sortear el corte para el cruceOX
 * @param padre1, padre2 Individuos padres sobre los que se va a realizar el corte
 * @return par de Individuos resultantes del cruce
*/
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



/**
 * @brief metodo encargado de que comprobar si una permutacion es valida
 * @param pi permutacion a comprobar
 * @return verdadero si permuatacion corecta, falso si permutacion false
*/
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



/**
 * @brief Metodo encargado de la mutacion por insercion
 * @param ind individuo a mutar
*/
void AlgoritmoGenetico::mutacionInsercion(Individuo& ind) {
  //Sortear dos posiciones distintas i y j
  std::uniform_int_distribution<int> distribucion(0, static_cast<int>(ind.permutacion.size()) - 1);
  int posAleatoriaI = distribucion(rng_);
  int posAleatoriaJ;
  do { //Se sortea al menos una vez y se repite mientras coincida con I
    posAleatoriaJ = distribucion(rng_);
  } while(posAleatoriaJ == posAleatoriaI);
  //Extraer el trabajo de la posicion i e insertarlo en la j (los intermedios se desplazan)
  int trabajo = ind.permutacion[posAleatoriaI]; 
  ind.permutacion.erase(ind.permutacion.begin() + posAleatoriaI);
  ind.permutacion.insert(ind.permutacion.begin() + posAleatoriaJ, trabajo);
  assert(esPermutacionValida(ind.permutacion));
}



/**
 * @brief Metodo encargado de la mutacion por intercambio
 * @param ind individuo a mutar
*/
void AlgoritmoGenetico::mutacionIntercambio(Individuo& ind) {
  //Sortear dos posiciones distintas i y j
  std::uniform_int_distribution<int> distribucion(0, static_cast<int>(ind.permutacion.size()) - 1);
  int posAleatoriaI = distribucion(rng_);
  int posAleatoriaJ;
  do { //Se sortea al menos una vez y se repite mientras coincida con I
    posAleatoriaJ = distribucion(rng_);
  } while(posAleatoriaJ == posAleatoriaI);
  //Intercambiar los trabajos de ambas posiciones
  std::swap(ind.permutacion[posAleatoriaI], ind.permutacion[posAleatoriaJ]);
  assert(esPermutacionValida(ind.permutacion));
}



/**
 * @brief Metodo que aplica la mutacion elegida en los parametros (insercion o intercambio)
 * @param ind individuo a mutar
*/
void AlgoritmoGenetico::mutar(Individuo& ind) {
  //Elegir la mutacion segun el parametro; si el tipo es desconocido se termina con error
  if(parametros_.tipo_mutacion_ == "insercion" || parametros_.tipo_mutacion_ == "INSERCION") mutacionInsercion(ind);
  else if(parametros_.tipo_mutacion_ == "intercambio" || parametros_.tipo_mutacion_ == "INTERCAMBIO") mutacionIntercambio(ind);
  else {
    std::cerr << "Tipo de mutacion desconocido: " << parametros_.tipo_mutacion_ << std::endl;
    std::exit(EXIT_FAILURE);
  }
}



/**
 * @brief Metodo de reemplazo generacional con elitismo: la nueva poblacion son los hijos,
 *        pero el mejor de la generacion anterior sustituye al peor hijo si es mejor que el
 * @param hijos individuos generados en la generacion actual (ya evaluados)
*/
void AlgoritmoGenetico::reemplazo(const std::vector<Individuo>& hijos) {
  //Localizar el mejor individuo de la poblacion anterior y el peor hijo
  std::vector<Individuo> copia = hijos;
  auto pos_mejor_anterior = std::min_element(poblacion_.begin(), poblacion_.end());
  auto pos_peor_hijo = std::max_element(copia.begin(), copia.end());
  Individuo mejor_anterior = poblacion_[pos_mejor_anterior - poblacion_.begin()];
  //Elitismo: el mejor anterior sustituye al peor hijo si es mejor que el
  if(mejor_anterior.makespan < copia[pos_peor_hijo - copia.begin()].makespan) {
    copia[pos_peor_hijo - copia.begin()] = mejor_anterior;
  }
  poblacion_ = copia;
}



/**
 * @brief Metodo que ejecuta el algoritmo genetico completo (algoritmo 1 del guion) hasta agotar
 *        el presupuesto de evaluaciones. El resultado se consulta con getMejorSolucion()
*/
void AlgoritmoGenetico::ejecutar() {
  inicializarPoblacion();
  //Bucle de generaciones: se repite mientras quede presupuesto
  while(evaluaciones_actuales_ < parametros_.max_evaluaciones_) {
    std::vector<Individuo> hijos; 
    //Generar hijos hasta completar N o agotar el presupuesto
    while(static_cast<int>(hijos.size()) < parametros_.tam_poblacion_ && evaluaciones_actuales_ < parametros_.max_evaluaciones_) {

      //Seleccion de los dos padres por torneo
      Individuo permutacion1 = seleccionTorneo(); 
      Individuo permutacion2 = seleccionTorneo();
      Individuo hijo1, hijo2;
      std::pair<Individuo, Individuo> cruce;

      std::uniform_real_distribution<double> azar(0.0, 1.0); 

      //Cruce OX con probabilidad pc; si no, los hijos son copias de los padres
      if(azar(rng_) < parametros_.prob_cruce_) {
        cruce = cruceOX(permutacion1, permutacion2);
        hijo1 = cruce.first;
        hijo2 = cruce.second;
      } else {
        hijo1 = permutacion1; 
        hijo2 = permutacion2;
      }

      //Mutacion de cada hijo con probabilidad pm (sorteo independiente)
      if(azar(rng_) < parametros_.prob_mutacion_) mutar(hijo1);
      if(azar(rng_) < parametros_.prob_mutacion_) mutar(hijo2);

      //Evaluar y anadir los hijos; el segundo solo si cabe en la poblacion y queda presupuesto
      evaluar(hijo1);
      hijos.push_back(hijo1);
      if(static_cast<int>(hijos.size()) < parametros_.tam_poblacion_ && evaluaciones_actuales_ < parametros_.max_evaluaciones_) {
        evaluar(hijo2); 
        hijos.push_back(hijo2);
      }
    }
    //Reemplazo solo si la generacion esta completa (si se agoto el presupuesto a mitad, se termina)
    if(static_cast<int>(hijos.size())== parametros_.tam_poblacion_) reemplazo(hijos);
  }
}