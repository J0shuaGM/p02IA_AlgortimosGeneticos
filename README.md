# Práctica 2: Algoritmo genético para el PFSP

**Inteligencia Artificial, 3.º Grado en Ingeniería Informática, Universidad de La Laguna**
Autor: Joshua Gómez Marrero (alu0101477398@ull.edu.es)

Este programa resuelve el *Permutation Flow Shop Scheduling Problem* (PFSP), que consiste en minimizar el makespan. Incluye tres algoritmos:

- **`ga`**: algoritmo genético;
- **`neh`**: heurística NEH;
- **`aleatoria`**: búsqueda aleatoria.

---

## 1. Compilación

Hace falta `g++` con soporte para C++17.

```bash
make          # genera el ejecutable ./genetico
make clean    # borra el ejecutable
```

---

## 2. Ejecución

```bash
./genetico <instancia> <fichero_parametros>
./genetico --help
```

Ejemplos:

```bash
./genetico instancias/tai20_10_0.fsp parametros/pam1.txt     # instancia de Taillard
./genetico instancias/modi.txt parametros/pam1.txt           # instancia en formato simple (sin UB)
```

El programa ejecuta el algoritmo `ejecuciones` veces, con las semillas `semilla`, `semilla+1`, … Después imprime **una línea CSV** con el resumen:

```
instancia,n,m,best_cmax,avg_cmax,std_cmax,cmax_neh,rpd_ub,rpd_neh,cpu_time_s
```

- `rpd_ub` = 100 · (best − UB) / UB. Vale `NULL` si la instancia no trae UB.
- `rpd_neh` = 100 · (best − C<sub>max</sub>NEH) / C<sub>max</sub>NEH. Si es negativo, el algoritmo mejora a NEH.
- `cpu_time_s` = tiempo medio de CPU por ejecución, en segundos.

**Formatos de instancia** (se detectan automáticamente):
- **Taillard:** la primera línea es texto (`number of jobs, …`) y trae n, m, semilla, UB y LB.
- **Simple:** la primera línea es `n m` y después viene directamente la matriz de m filas × n columnas.

### Fichero de parámetros

El fichero contiene pares `clave valor`. Lo que va detrás de `#` es un comentario. Las claves que no aparecen toman su valor por defecto.

| Clave | Significado | Por defecto |
|---|---|---|
| `algoritmo` | `ga`, `neh` o `aleatoria` | `ga` |
| `N` | Tamaño de la población | 100 |
| `pc` | Probabilidad de cruce | 0.9 |
| `pm` | Probabilidad de mutación (por individuo) | 0.2 |
| `K` | Tamaño del torneo | 2 |
| `E` | Presupuesto de evaluaciones | 50000 |
| `mutacion` | `insercion` o `intercambio` | `insercion` |
| `semilla` | Semilla de la primera ejecución | 1 |
| `ejecuciones` | Número de ejecuciones independientes | 1 |
| `brutos` | CSV donde se **añade** una fila por ejecución (`instancia,semilla,cmax,rpd_ub,cpu_time_s`) | *(no se guarda)* |

> El fichero de `brutos` **acumula** filas en cada ejecución. Para pruebas sueltas, usa un fichero de parámetros sin esa clave.

Ficheros incluidos en `parametros/`:
- `pam1.txt`: configuración del AG usada en la experimentación;
- `aleatoria.txt`: búsqueda aleatoria de referencia.

---

## 3. Script de experimentación (tablas)

```bash
./experimentos.sh <fichero_parametros> [patron_instancias]
```

El script recorre las instancias de `instancias/` (las `.fsp`) ordenadas por tamaño y, al terminar cada una, añade su fila a `resultados/tabla_<nombre_parametros>.csv`. Si el fichero de parámetros tiene la clave `brutos`, también rellena ese CSV.

```bash
./experimentos.sh parametros/pam1.txt                # las 120 instancias (~6 min)
./experimentos.sh parametros/pam1.txt "tai20_*"      # solo n = 20
./experimentos.sh parametros/pam1.txt "tai50_10_*"   # solo el grupo 50×10
./experimentos.sh parametros/aleatoria.txt           # búsqueda aleatoria (~4,5 min)
```

> La tabla y los brutos se **reescriben desde cero** cada vez que se lanza el script con el mismo fichero de parámetros.

---

## 4. Reproducir los resultados entregados

```bash
make
./experimentos.sh parametros/pam1.txt        # → resultados/tabla_pam1.csv y brutos_pam1.csv
./experimentos.sh parametros/aleatoria.txt   # → resultados/tabla_aleatoria.csv y brutos_aleatoria.csv
./experimentos.sh parametros/pam2.txt        # → resultados/tabla_pam2.csv y brutos_pam2.csv (K = 6)
```

Con la misma semilla se obtiene siempre exactamente el mismo resultado.

---

## 5. Comparación del tamaño del torneo: K = 2 frente a K = 6

`pam1.txt` (K = 2) y `pam2.txt` (K = 6) solo se diferencian en K. Los demás parámetros son N = 100, pc = 0.9, pm = 0.2, mutación por inserción, E = 50000 y 10 semillas, sobre las 120 instancias.

| | K = 2 | K = 6 |
|---|---|---|
| RPD medio del mejor (`rpd_ub`) | **1.37 %** | 1.45 % |
| RPD medio de la media de las 10 ejecuciones | **1.80 %** | 1.87 % |
| Instancias en las que obtiene mejor `best_cmax` | **58** | 31 (31 empates) |
| Instancias con las 10 semillas en el mismo resultado (std = 0) | 5 | 10 |
| Tiempo medio por ejecución | 0.32 s | 0.33 s |

**Conclusión:** K = 2 obtiene mejores resultados. Con N = 100, la probabilidad de que el mejor individuo gane un torneo es 1 − (1 − 1/N)<sup>K</sup>, que pasa de ≈ 2 % con K = 2 a ≈ 5.9 % con K = 6. Ese aumento de la presión selectiva hace que la población pierda diversidad antes (más explotación y menos exploración) y favorece la convergencia prematura: con K = 6 se duplican las instancias en las que todas las semillas acaban en el mismo makespan, y la desventaja es mayor en las instancias pequeñas. El tiempo no cambia, porque el torneo solo compara valores ya calculados. Las diferencias son pequeñas (≈ 0.08 puntos de RPD) y en algunos grupos K = 6 empata o gana, pero la tendencia global favorece al torneo binario (K = 2), que es el que se usa en la configuración final.
