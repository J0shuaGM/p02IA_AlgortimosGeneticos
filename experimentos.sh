#!/bin/bash
# Universidad de La Laguna Escuela Superior de Ingenieria y Tecnologia
# Grado en Ingenieria Informatica
# Asignatura: Inteligencia Artificial
# Practica 2: Algoritmos geneticos
# Autor: Joshua Gomez Marrero
# Archivo: experimentos.sh
#
# Ejecuta el programa sobre un conjunto de instancias con un fichero de parametros
# y va anadiendo a la tabla una fila por instancia en cuanto esta termina.
#
# Uso:   ./experimentos.sh <fichero_parametros> [patron_instancias]
# Ej.:   ./experimentos.sh parametros/pam1.txt                 (todas las instancias)
#        ./experimentos.sh parametros/pam1.txt "tai20_*"       (solo las de n = 20)
#
# Salida: resultados/tabla_<nombre_parametros>.csv  (una fila por instancia)
#         el fichero indicado en la clave 'brutos'   (una fila por ejecucion)
# Ambos ficheros se REESCRIBEN desde cero en cada ejecucion del script.

if [ $# -lt 1 ]; then
  echo "Uso: $0 <fichero_parametros> [patron_instancias]" >&2
  exit 1
fi

PARAMETROS=$1
PATRON=${2:-"tai*"}
NOMBRE=$(basename "$PARAMETROS" .txt)
TABLA=resultados/tabla_${NOMBRE}.csv
BRUTOS=$(awk '$1 == "brutos" {print $2}' "$PARAMETROS") # Ruta de la clave 'brutos' (si existe)

make -s || exit 1
mkdir -p resultados

# Cabeceras
echo "instancia,n,m,best_cmax,avg_cmax,std_cmax,cmax_neh,rpd_ub,rpd_neh,cpu_time_s" > "$TABLA"
if [ -n "$BRUTOS" ]; then
  echo "instancia,semilla,cmax,rpd_ub,cpu_time_s" > "$BRUTOS"
fi

# Instancias ordenadas por tamano (sort -V: tai20 < tai50 < tai100 ...)
for INSTANCIA in $(ls instancias/${PATRON}.fsp | sort -V); do
  echo "[$(date +%H:%M:%S)] $INSTANCIA" >&2
  ./genetico "$INSTANCIA" "$PARAMETROS" >> "$TABLA" || exit 1
done

echo "Tabla guardada en $TABLA" >&2
[ -n "$BRUTOS" ] && echo "Brutos guardados en $BRUTOS" >&2
