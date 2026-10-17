# Universidad de La Laguna
# Escuela Superior de Ingenieria y Tecnologia
# Grado en Ingenieria Informatica
# Asignatura: Inteligencia Artificial
# Curso: 3º
# Practica 2: Algoritmos geneticos
# Autor: Joshua Gomez Marrero
# Correo: alu0101477398@ull.edu.es
# Fecha: 24/09/2026
# Archivo: Makefile

# Nombre del archivo de salida
TARGET = genetico

# Lista de archivos fuente(.cc)
SOURCES = cliente.cc entornopfsp.cc tools.cc heuristica.cc algoritmogenetico.cc

# Dependencias de los archivos fuente 
DEPENDENCIES = tools.h individuo.h entornopfsp.h algoritmogenetico.h heuristica.h

# Opciones de compilación
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Regla para compilar el programa
$(TARGET): $(SOURCES) $(DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Regla para compilar el programa de pruebas del AG (con AddressSanitizer para detectar accesos fuera de rango)
PRUEBA_SOURCES = prueba_ag.cc entornopfsp.cc heuristica.cc algoritmogenetico.cc
prueba: $(PRUEBA_SOURCES) $(DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -g -fsanitize=address -o prueba_ag $(PRUEBA_SOURCES)

# Regla para limpiar los archivos generados
.PHONY: clean prueba
clean:
	rm -f $(TARGET) prueba_ag *.o
