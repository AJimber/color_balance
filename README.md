# Color Balance

Programa de visión artificial desarrollado en C++ utilizando OpenCV.

## Descripción

Esta programa implementa técnicas de balance de color para corregir las dominantes cromáticas de imágenes digitales.

El objetivo es conseguir una representación más equilibrada de los colores mediante el ajuste de los canales de la imagen.

## Funcionamiento

El programa realiza los siguientes pasos:

1. Carga la imagen de entrada utilizando OpenCV.
2. Analiza los canales de color de la imagen.
3. Calcula los ajustes necesarios para equilibrar los colores.
4. Aplica las correcciones sobre los canales correspondientes.
5. Genera la imagen resultante con el balance de color corregido.

```bash

./color_balance <../data/ejemplo.jpg> <salida.jpg>

```

### Modo interactivo

El programa dispone de un modo interactivo que se activa mediante el argumento `-i`.

Este modo permite visualizar los resultados y ajustar los parámetros del balance de color de forma interactiva, facilitando la comparación de los diferentes resultados.

Para activarlo, añadir `-i` a los argumentos de ejecución:

```bash

./color_balance -i <../data/ejemplo.jpg> <salida.jpg>

```

En caso de necesitar una imagen de entrada, se deben proporcionar también los argumentos correspondientes.
## Requisitos

- C++
- OpenCV
- CMake
- Compilador compatible con C++

## Compilación

Desde el directorio principal del proyecto:

```bash
mkdir -p build
cd build
cmake ..
make