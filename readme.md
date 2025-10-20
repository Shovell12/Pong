# 🎮 Pong Game - SFML 3.0.2

Un clásico juego Pong implementado en C++ con SFML 3.0.2 con características mejoradas.

## 🚀 Características
- **Gráficos pixelados** con SFML 3.0.2
- **Sistema de menús** interactivo
- **Dos jugadores** con controles separados
- **Física mejorada** con aceleración progresiva
- **Sistema de puntuación** con condición de victoria
- **Pantalla de ganador** al finalizar el juego
- **Efectos visuales** como parpadeo de textos

## 🎯 Controles
- **Jugador 1 (Izquierda)**: 
  - `W` - Mover hacia arriba
  - `S` - Mover hacia abajo
  
- **Jugador 2 (Derecha)**:
  - `Flecha Arriba` - Mover hacia arriba  
  - `Flecha Abajo` - Mover hacia abajo

- **Controles generales**:
  - `ESPACIO` - Iniciar juego / Continuar en menús
  - `ESC` - Salir al menú / Salir del juego

## 🎮 Mecánicas de Juego
- **Aceleración progresiva**: La pelota aumenta velocidad con cada rebote
- **Rebotes realistas**: En paletas, techo y suelo
- **Sistema de puntos**: Gana el primero en llegar a 5 puntos
- **Reset automático**: Las posiciones se reinician al comenzar nuevo juego

## 🛠 Compilación

```bash
# Compilar y ejecutar
g++ -o pong src/pong.cpp -lsfml-graphics -lsfml-window -lsfml-system
./pong
