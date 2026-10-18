# Práctica 2: Algoritmo genético para el PFSP

**Inteligencia Artificial, 3.º Grado en Ingeniería Informática, Universidad de La Laguna**
Autor: Joshua Gómez Marrero (alu0101477398@ull.edu.es)

Este programa resuelve el *Permutation Flow Shop Scheduling Problem* (PFSP), que consiste en minimizar el makespan (C<sub>max</sub>). Incluye tres algoritmos:

- **`ga`**: algoritmo genético con selección por torneo, cruce OX, mutación por inserción o intercambio y reemplazo generacional con elitismo.
- **`neh`**: heurística constructiva NEH (Nawaz, Enscore y Ham, 1983).
- **`aleatoria`**: búsqueda aleatoria. Sirve como algoritmo de referencia.

---

## 1. Estructura del proyecto

```
├── Makefile
├── README.md
├── cliente.cc               Programa principal: lee instancia y parámetros, ejecuta y genera la fila CSV
├── tools.h / tools.cc       Usage (--help) y lectura del fichero de parámetros
├── entornopfsp.h / .cc      Clase PFSP: lectura de instancias de Taillard y cálculo del makespan
├── individuo.h              Struct Individuo: permutación + makespan
├── heuristica.h / .cc       NEH y búsqueda aleatoria
├── algoritmogenetico.h / .cc  Struct Parametros y clase AlgoritmoGenetico
├── experimentos.sh          Script que ejecuta un conjunto de instancias y construye la tabla
├── instancias/              120 instancias de Taillard (20×5 … 500×20) + ejemplo_4x3.txt del guion
├── parametros/              Ficheros de configuración (pam1.txt, aleatoria.txt, …)
└── resultados/              Tablas resumen y resultados brutos (CSV)
```

---

## 2. Compilación

Hace falta `g++` con soporte para C++17 (probado con g++ 13.3).

```bash
make          # genera el ejecutable ./genetico
make clean    # borra el ejecutable
```

Se compila con `-std=c++17 -Wall -O2`. La opción `-O2` es importante porque reduce mucho el tiempo de las ejecuciones de 50 000 evaluaciones.

La comprobación de factibilidad (T4) usa `assert`, y está **activada por defecto**. Para desactivarla en la experimentación basta con añadir `-DNDEBUG` a `CXXFLAGS`, sin tocar el código.

---

## 3. Ejecución del programa

```bash
./genetico <instancia> <fichero_parametros>
./genetico --help
```

Ejemplo:

```bash
./genetico instancias/tai20_10_0.fsp parametros/pam1.txt
```

El programa ejecuta el algoritmo indicado `ejecuciones` veces. Cada ejecución usa una semilla distinta (`semilla`, `semilla+1`, …).

- **Por la salida estándar** imprime **una línea CSV con el resumen de la instancia**.
- **En el fichero `brutos`**, si se indica, añade **una línea por ejecución**.

Los mensajes de error van por la salida de error, así que no se mezclan con el CSV.

### 3.1. Fichero de parámetros

El fichero contiene pares `clave valor`. Todo lo que va detrás de `#` es un comentario. Si una clave no aparece, toma su valor por defecto. Si una clave es desconocida o tiene un valor no válido, el programa muestra un error y termina, para que una errata no pase desapercibida.

| Clave | Significado | Por defecto |
|---|---|---|
| `algoritmo` | `ga`, `neh` o `aleatoria` | `ga` |
| `N` | Tamaño de la población | 50 |
| `pc` | Probabilidad de cruce | 0.9 |
| `pm` | Probabilidad de mutación (por individuo) | 0.2 |
| `K` | Tamaño del torneo | 2 |
| `E` | Presupuesto de evaluaciones del makespan | 50000 |
| `mutacion` | `insercion` o `intercambio` | `insercion` |
| `semilla` | Semilla de la primera ejecución | 1 |
| `ejecuciones` | Número de ejecuciones independientes | 1 |
| `brutos` | CSV donde se añade una fila por ejecución | *(no se guardan)* |

Ejemplo (`parametros/pam1.txt`, la configuración usada en la experimentación):

```
algoritmo   ga
N           100
pc          0.9
pm          0.2
K           2
E           50000
mutacion    insercion
semilla     1
ejecuciones 10          # semillas 1..10
brutos      resultados/brutos_pam1.csv
```

### 3.2. Formato de salida

**Fila resumen** (una por instancia), en la salida estándar:

```
instancia,n,m,best_cmax,avg_cmax,std_cmax,cmax_neh,rpd_ub,rpd_neh,cpu_time_s
```

| Columna | Cálculo |
|---|---|
| `best_cmax` | Mejor C<sub>max</sub> de las R ejecuciones |
| `avg_cmax` | Media de los R valores de C<sub>max</sub> |
| `std_cmax` | Desviación típica muestral: √(Σ(c<sub>i</sub> − media)² / (R − 1)) |
| `cmax_neh` | C<sub>max</sub> de NEH. Se calcula una vez, porque NEH es determinista |
| `rpd_ub` | 100 · (best − UB) / UB, donde UB es la cota superior que figura en el fichero de Taillard |
| `rpd_neh` | 100 · (best − C<sub>max</sub>NEH) / C<sub>max</sub>NEH. **Si es negativo, el algoritmo mejora a NEH** |
| `cpu_time_s` | Tiempo de CPU medio por ejecución, en segundos (`std::clock`) |

**Fila bruta** (una por ejecución), en el fichero `brutos`:

```
instancia,semilla,cmax,rpd_ub,cpu_time_s
```

---

## 4. Script de experimentación

```bash
./experimentos.sh <fichero_parametros> [patron_instancias]
```

El script compila si hace falta y escribe las cabeceras CSV. Después recorre las instancias ordenadas por tamaño y, **al terminar cada instancia, añade su fila** a `resultados/tabla_<nombre_parametros>.csv`. El progreso se muestra por la salida de error.

Ejemplos:

```bash
./experimentos.sh parametros/pam1.txt                # las 120 instancias
./experimentos.sh parametros/pam1.txt "tai20_*"      # solo n = 20
./experimentos.sh parametros/pam1.txt "tai50_10_*"   # solo el grupo 50×10
./experimentos.sh parametros/aleatoria.txt           # búsqueda aleatoria de referencia
```

> **Atención:** la tabla y el fichero de brutos se **reescriben desde cero** cada vez que se lanza el script con el mismo fichero de parámetros. Para conservar los resultados de una configuración, crea un fichero de parámetros con otro nombre.

**Tiempo aproximado** de una ejecución completa (`pam1.txt`, 120 instancias × 10 ejecuciones): unos 6 minutos el AG y unos 4,5 minutos la búsqueda aleatoria. Una sola ejecución del AG tarda alrededor de 0,07 s en 20×5 y de 2 s en 500×20.

### Reproducir los resultados entregados

```bash
make
./experimentos.sh parametros/pam1.txt        # → resultados/tabla_pam1.csv, resultados/brutos_pam1.csv
./experimentos.sh parametros/aleatoria.txt   # → resultados/tabla_aleatoria.csv, resultados/brutos_aleatoria.csv
```

Todas las ejecuciones son reproducibles: con la misma semilla se obtiene exactamente el mismo resultado.

---

## 5. Decisiones de diseño

### Representación y evaluación
- **Representación:** cada solución es una permutación de los trabajos, indexados internamente de 0 a n−1. Toda permutación es factible.
- **Makespan:** se calcula con la recurrencia (1) del guion usando **un único vector de tamaño m** en lugar de una matriz n×m. Al procesar el trabajo k, `C[i]` todavía contiene C(k−1, i) y `C[i-1]` ya contiene C(k, i−1). El coste en tiempo es O(n·m) y en memoria O(m). Se ha verificado con el ejemplo del guion: (1,2,3,4) → 28 y (3,4,2,1) → 21.
- **Contador de evaluaciones:** `Makespan` solo se llama desde `AlgoritmoGenetico::evaluar()`. Ese método incrementa el contador, actualiza el mejor global y registra cada mejora en el historial (evaluación, C<sub>max</sub>), que sirve para las gráficas de convergencia.

### Población inicial
- **1 individuo NEH + (N − 1) permutaciones aleatorias** (barajadas con Fisher-Yates mediante `std::shuffle`). El individuo NEH da un buen punto de partida y los aleatorios aportan la diversidad necesaria para evitar la convergencia prematura.
- **Evaluaciones de NEH:** el individuo NEH cuenta **una** evaluación del presupuesto, la de su makespan final. Las ≈ n²/2 evaluaciones internas de la construcción no se cuentan. NEH es un paso previo determinista y, si se contaran, en las instancias de n = 500 (≈125 000 evaluaciones) agotarían el presupuesto de 50 000 antes de empezar. Su tiempo sí está incluido en `cpu_time_s`.
- **NEH:** se ordenan los trabajos por tiempo total decreciente con `std::stable_sort`, de modo que en caso de empate se respeta el índice y el resultado es determinista. En la inserción, si dos posiciones empatan, se elige la primera.

### Operadores
- **Selección por torneo de tamaño K (con reemplazo):** se escogen K individuos al azar y gana el de menor makespan. Se ha preferido a la ruleta porque trabaja directamente con minimización, no depende de la escala de los valores de makespan y la presión selectiva se controla con K. Se comprobó empíricamente que la probabilidad de elegir al mejor individuo coincide con 1 − (1 − 1/N)<sup>K</sup>.
- **Cruce OX con relleno de izquierda a derecha:** se copia el segmento `p1[a..b]` en las mismas posiciones y los huecos se rellenan, de izquierda a derecha, con los trabajos que faltan en el orden en que aparecen en p2. h2 se obtiene intercambiando los papeles de los padres y usando los mismos cortes.
  *Justificación:* es la variante que describe el guion, es más sencilla de implementar y de comprobar a mano, y conserva igual que la versión circular de Davis las dos propiedades útiles para el PFSP: el bloque de trabajos consecutivos de p1 y el orden relativo del resto según p2. Se ha validado con el ejemplo de la figura 3 del guion.
- **Mutación por inserción (por defecto) o por intercambio**, que se elige con la clave `mutacion`. Las posiciones i y j se sortean garantizando i ≠ j, para que la mutación nunca sea nula. La inserción se usa por defecto porque altera menos el orden relativo y los bloques de trabajos, y es el movimiento en el que se basa NEH.
- **Interpretación de p<sub>m</sub>:** es la probabilidad **por individuo**. Cada hijo se muta una vez con probabilidad p<sub>m</sub>, y el sorteo es independiente para h1 y h2, siguiendo el paso 8 del algoritmo 1.

### Reemplazo y bucle principal
- **Reemplazo generacional con elitismo:** la nueva población está formada por los hijos. Si el mejor individuo de la generación anterior es mejor que el peor hijo, lo sustituye. Así el mejor de la población nunca empeora y sigue reproduciéndose.
- **Presupuesto exacto:** el presupuesto se comprueba antes de cada evaluación, y no solo al inicio de cada generación. Así cada ejecución realiza **exactamente E evaluaciones**, igual que la búsqueda aleatoria, y la comparación es justa. Si el presupuesto se agota a mitad de una generación, el algoritmo termina sin hacer el reemplazo. No se pierde información, porque `evaluar()` ya ha actualizado el mejor global.
- **N impar:** el segundo hijo solo se añade si todavía cabe en la población.

### Factibilidad (T4)
- `esPermutacionValida()` comprueba que la solución tiene longitud n, que todos los valores están en [0, n−1] y que no hay repetidos. Con eso queda garantizado que están todos los trabajos. Se invoca mediante `assert` tras cada cruce y cada mutación.

### Aleatoriedad y reproducibilidad
- Se usa un único `std::mt19937` por ejecución, inicializado con la semilla de esa ejecución y pasado por referencia. No se usa `rand()`.
- La semilla que figura en los ficheros de Taillard es la que se empleó para **generar la instancia**. No se utiliza como semilla del algoritmo.

### Simplificaciones asumidas
- La lectura de instancias supone que los ficheros siguen el formato de Taillard que se ha proporcionado, así que no valida su contenido. Solo comprueba que el fichero se puede abrir.
