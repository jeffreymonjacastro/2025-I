# Nota PARA TODOS los problemas

Los problemas cuyo prototipo es un template serán testeados con `<int>` y con
`<double>`. En todos los casos se testeará exhaustivamente, incluyendo
condiciones de borde que incluirán valores como los máximos y mínimos
representables, para confirmar ausencia de overflows/underflows.

## PROBLEMA 1

El prototipo de la función debe ser:

```cpp
template <typename T>
bool intersection(T xa, T ya, T xb, T yb, T xc, T yc, T xd, T yd)
```

Será testeado con T siendo `<int>` y siendo `<double>`. Será testeado, entre otros
casos, con muchas condiciones de borde que incluirán casos con coordenadas cuyos
valores serán máximos y/o mínimos representables. Por ejemplo, el caso

```cpp
intersection(INT_MIN, INT_MIN,
             INT_MAX, INT_MAX,
             INT_MIN, INT_MIN+1
             INT_MAX, INT_MAX-1)
```

y casos similares. También se hará lo mismo para double.

> [!NOTE]
>
> - [x] Implementado
> - [x] Testeado con casos esquina

## PROBLEMA 2

El prototipo de la función debe ser:

```cpp
template <typename T>
bool is_convex(vector<vector<T>> const& P)
```

Se garantiza que cada elemento de P será un vector de 2 elementos de tipo T.

`P[0][0]` será la coordenada x del punto p0 \
`P[0][1]` será la coordenada y del punto p0

`P[1][0]` será la coordenada x del punto p1 \
`P[1][1]` será la coordenada y del punto p1

`P[2][0]` será la coordenada x del punto p2 \
`P[2][1]` será la coordenada y del punto p2

...

`P[k][0]` será la coordenada x del punto pk \
`P[k][1]` será la coordenada y del punto pk

Será testeado con T siendo `<int>` y siendo `<double>`. Será testeado
exhaustivamente, con tests que incluirán casos en los que las coordenadas serán
los máximos y/o mínimos valores representables

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 3

El prototipo de la función debe ser:

```cpp
// devuelve la distancia desde P a la recta dada
// por el punto P1 y la direccion D
double distance_point_to_line_3D(
    double p1x, double p1y, double p1z,  // punto P1
    double dx, double dy, double dz, // direccion
    double px, double py, double pz // punto P
    )
```

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 4

El prototipo de la función debe ser:

```cpp
template <typename T>
bool inside_triangle(vector<vector<T>> const& vertices, T px, T py)
```

Se garantiza que `vertices` tiene 3 elementos, cada uno de los cuales será un
vector de 2 elementos de tipo T.

`vertices[0][0]` será la coordenada x del vertice 0 del triangulo
`vertices[0][1]` será la coordenada y del vertice 0 del triangulo

`vertices[1][0]` será la coordenada x del vertice 1 del triangulo
`vertices[1][1]` será la coordenada y del vertice 1 del triangulo

`vertices[2][0]` será la coordenada x del vertice 2 del triangulo
`vertices[2][1]` será la coordenada y del vertice 2 del triangulo

Será testeado con T siendo `<int>` y siendo `<double>`. Será testeado
exhaustivamente, con tests que incluirán casos en los que las coordenadas serán
los máximos y/o mínimos valores representables

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 5

El prototipo de la función debe ser:

```cpp
template <typename T>
bool inside_polygon(vector<vector<T>> const& vertices, T px, T py)
```

Se garantiza que `vertices` describen un poligono no cruzado. Cada uno de los
elementos en `vertices` es un vector de 2 elementos de tipo T que representa las
coordenadas x e y del vértice.

`vertices[0][0]` será la coordenada x del vertice 0 del polígono.\
`vertices[0][1]` será la coordenada y del vertice 0 del polígono.

`vertices[1][0]` será la coordenada x del vertice 1 del polígono.\
`vertices[1][1]` será la coordenada y del vertice 1 del polígono.

`vertices[2][0]` será la coordenada x del vertice 2 del polígono.\
`vertices[2][1]` será la coordenada y del vertice 2 del polígono.

...

`vertices[k][0]` será la coordenada x del vertice k del polígono.\
`vertices[k][1]` será la coordenada y del vertice k del polígono.

Será testeado con T siendo `<int>` y siendo `<double>`. Será testeado
exhaustivamente, con tests que incluirán casos en los que las coordenadas serán los máximos y/o mínimos valores representables

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 6

Los prototipos tienen que ser los siguientes:

```cpp
// Realiza eliminación de puntos interiores en un array de puntos
template <typename T>
vector<vector<T>> interior_points_removal(vector<vector<T>> const& vertices)

// ejecuta jarvis, con o sin elinminación de puntos interiors (especificado vía un parametro)
template <typename T>
vector<vector<T>> jarvis(vector<vector<T>> const& vertices,
    bool withInteriorPointsRemoval)

// ejecuta graham, con o sin elinminación de puntos interiors (especificado vía un parametro)
template <typename T>
vector<vector<T>> graham(vector<vector<T>> const& vertices,
    bool withInteriorPointsRemoval)
```

Como en todos los ejercicios, los vectores que describen puntos tendrán 2
elementos que se corresponderán con las coordenadas del punto siendo
representado.

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 7

> ‼️Hard Problem‼️

El prototipo tiene que ser:

```cpp
template <typename T>
bool exist_intersections(vector<vector<T>> const& segments)
```

Cada elemento de `segments` consistirá de 4 valores de tipo `T:
x1, y1, x2, y2` (en ese orden), que corresponderán a las coordenadas de los
extremos del segmento.

`segments[0][0]` será la coordenada x del extremo 0 del segmento 0 \
`segments[0][1]` será la coordenada y del extremo 0 del segmento 0 \
`segments[0][2]` será la coordenada x del extremo 1 del segmento 0 \
`segments[0][3]` será la coordenada y del extremo 1 del segmento 0

`segments[1][0]` será la coordenada x del extremo 0 del segmento 1 \
`segments[1][1]` será la coordenada y del extremo 0 del segmento 1 \
`segments[1][2]` será la coordenada x del extremo 1 del segmento 1 \
`segments[1][3]` será la coordenada y del extremo 1 del segmento 1

...

`segments[k][0]` será la coordenada x del extremo 0 del segmento k \
`segments[k][1]` será la coordenada y del extremo 0 del segmento k \
`segments[k][2]` será la coordenada x del extremo 1 del segmento k \
`segments[k][3]` será la coordenada y del extremo 1 del segmento k

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 8

> ‼️Hard Problem‼️

El prototipo de la función debe ser:

```cpp
template <typename T>
vector<int> closest_points(vector<vector<T>> const& points)
```

La función tiene que retornar UN VECTOR DE DOS ELEMENTOS, ORDENADO DE MENOR A
MAYOR. Los elementos en este retorno deberán ser LOS ÍNDICES DE LOS 2 PUNTOS QUE
ESTÁN A LA MENOR DISTANCIA, EN EL ARRAY INPUT.

Se garantiza que `points` tendrá al menos dos elementos, (por favor desmerecer
el caso N=1, es simplemente un error de tipeo en el enunciado). Cada uno de los
elementos en `points` es un vector de 2 elementos de tipo T que representa las
coordenadas `x` e `y` del punto.

`points[0][0]` será la coordenada x del punto 0 \
`points[0][1]` será la coordenada y del punto 0

`points[1][0]` será la coordenada x del punto 1 \
`points[1][1]` será la coordenada y del punto 1

`points[2][0]` será la coordenada x del punto 2 \
`points[2][1]` será la coordenada y del punto 2

...

`points[k][0]` será la coordenada x del punto k \
`points[k][1]` será la coordenada y del punto k

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 9

Simplemente adjunten una explicación o bien en .TXT o bien en .PDF

> [!NOTE]
>
> - [x] Escrito

## PROBLEMA 10

El prototipo de la función debe ser:

```cpp
int num_of_triangularions(int n)
```

NOTA: el número puede ser muy grande, al punto de no caber en un int. Para
evitar problemas, devolver el resultado **módulo 1000000007**. Se testeará la
correctitud **módulo 1000000007**.

> [!NOTE]
>
> - [x] Implementado
> - [x] Testeado con casos esquina

## PROBLEMA 11

El prototipo de la función debe ser:

```cpp
template <typename T>
double area(vector<vector<T>> const& vertices)
```

Se garantiza que `vertices` describen un poligono convexo. Cada uno de los
elementos en `vertices` es un vector de 2 elementos de tipo T que representa las
coordenadas x e y del vértice.

`vertices[0][0]` será la coordenada x del vertice 0 del polígono. \
`vertices[0][1]` será la coordenada y del vertice 0 del polígono.

`vertices[1][0]` será la coordenada x del vertice 1 del polígono. \
`vertices[1][1]` será la coordenada y del vertice 1 del polígono.

`vertices[2][0]` será la coordenada x del vertice 2 del polígono. \
`vertices[2][1]` será la coordenada y del vertice 2 del polígono.

...

`vertices[k][0]` será la coordenada x del vertice k del polígono. \
`vertices[k][1]` será la coordenada y del vertice k del polígono.

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 12

> ‼️Hard Problem‼️

El prototipo de la función debe ser:

```cpp
template <typename T>
double area_of_intersection(vector<vector<T>> const& vertices1, vector<vector<T>> const& vertices2)
```

Se garantiza que `vertices1` y `vertices2` describen polígonos convexos. Cada
uno de los elementos en ellos es un vector de 2 elementos de tipo T que
representa las coordenadas `x` e `y` de un vértice.

`vertices1[0][0]` será la coordenada x del vertice 0 del polígono 1. \
`vertices1[0][1]` será la coordenada y del vertice 0 del polígono 1.

`vertices1[1][0]` será la coordenada x del vertice 1 del polígono 1. \
`vertices1[1][1]` será la coordenada y del vertice 1 del polígono 1.

`vertices1[2][0]` será la coordenada x del vertice 2 del polígono 1. \
`vertices1[2][1]` será la coordenada y del vertice 2 del polígono 1.

...

`vertices1[k][0]` será la coordenada x del vertice k del polígono 1. \
`vertices1[k][1]` será la coordenada y del vertice k del polígono 1.

---

`vertices2[0][0]` será la coordenada x del vertice 0 del polígono 2. \
`vertices2[0][1]` será la coordenada y del vertice 0 del polígono 2.

`vertices2[1][0]` será la coordenada x del vertice 1 del polígono 2. \
`vertices2[1][1]` será la coordenada y del vertice 1 del polígono 2.

`vertices2[2][0]` será la coordenada x del vertice 2 del polígono 2. \
`vertices2[2][1]` será la coordenada y del vertice 2 del polígono 2.

...

`vertices2[k][0]` será la coordenada x del vertice k del polígono 2. \
`vertices2[k][1]` será la coordenada y del vertice k del polígono 2.

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 13

El prototipo de la función debe ser:

```cpp
template <typename T>
vector<vector<T>> douglas_peucker(vector<vector<T>> const& vertices,
    double epsilon)
```

Se garantiza que `vertices` describen una polilinea no cruzado. Cada uno de los
elementos en `vertices` es un vector de 2 elementos de tipo T que representa las
coordenadas `x` e `y` del vértice.

`vertices[0][0]` será la coordenada x del vertice 0 de la polilinea. \
`vertices[0][1]` será la coordenada y del vertice 0 de la polilinea.

`vertices[1][0]` será la coordenada x del vertice 1 de la polilinea. \
`vertices[1][1]` será la coordenada y del vertice 1 de la polilinea.

`vertices[2][0]` será la coordenada x del vertice 2 de la polilinea. \
`vertices[2][1]` será la coordenada y del vertice 2 de la polilinea.

...

`vertices[k][0]` será la coordenada x del vertice k de la polilinea. \
`vertices[k][1]` será la coordenada y del vertice k de la polilinea.

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 14

El prototipo de la función debe ser:

```cpp
template <typename T>
vector<vector<T>> triangles_based_simplification(vector<vector<T>> const& vertices,
    int N)
```

`N` es el número de nodos que tiene que tener el resultado.
La decisión de "qué nodo eliminar" debe basarse en el área del triangulo y en el tamaño del mayor de los ángulos del triángulo (triángulos "mas parecidos a una línea" deben eliminarse antes que triangulos "menos parecidos a una linea").

> [!NOTE]
>
> - [x] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 15

> ‼️Hard Problem‼️

El prototipo de la función debe ser:

```cpp
bool can_go_without_crossing_segments(
    vector<vector<double>> const& segments,
    double xa, double ya,
    double xb, double yb)
```

Cada elemento de `segments` consistirá de 4 valores de tipo double:
`x1, y1, x2, y2` (en ese orden), que corresponderán a las coordenadas de los
extremos del segmento.

`segments[0][0]` será la coordenada x del extremo 0 del segmento 0 \
`segments[0][1]` será la coordenada y del extremo 0 del segmento 0 \
`segments[0][2]` será la coordenada x del extremo 1 del segmento 0 \
`segments[0][3]` será la coordenada y del extremo 1 del segmento 0

`segments[1][0]` será la coordenada x del extremo 0 del segmento 1 \
`segments[1][1]` será la coordenada y del extremo 0 del segmento 1 \
`segments[1][2]` será la coordenada x del extremo 1 del segmento 1 \
`segments[1][3]` será la coordenada y del extremo 1 del segmento 1

...

`segments[k][0]` será la coordenada x del extremo 0 del segmento k \
`segments[k][1]` será la coordenada y del extremo 0 del segmento k \
`segments[k][2]` será la coordenada x del extremo 1 del segmento k \
`segments[k][3]` será la coordenada y del extremo 1 del segmento k

> [!NOTE]
>
> - [ ] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 16

> ‼️Hard Problem‼️

El prototipo de la función debe ser:

```cpp
int maximum_intersectable_with_one_line(
    vector<vector<double>> const& segments
)
```

Cada elemento de `segments` consistirá de 4 valores de tipo double:
`x1, y1, x2, y2` (en ese orden), que corresponderán a las coordenadas de los
extremos del segmento.

`segments[0][0]` será la coordenada x del extremo 0 del segmento 0 \
`segments[0][1]` será la coordenada y del extremo 0 del segmento 0 \
`segments[0][2]` será la coordenada x del extremo 1 del segmento 0 \
`segments[0][3]` será la coordenada y del extremo 1 del segmento 0

`segments[1][0]` será la coordenada x del extremo 0 del segmento 1 \
`segments[1][1]` será la coordenada y del extremo 0 del segmento 1 \
`segments[1][2]` será la coordenada x del extremo 1 del segmento 1 \
`segments[1][3]` será la coordenada y del extremo 1 del segmento 1

...

`segments[k][0]` será la coordenada x del extremo 0 del segmento k \
`segments[k][1]` será la coordenada y del extremo 0 del segmento k \
`segments[k][2]` será la coordenada x del extremo 1 del segmento k \
`segments[k][3]` será la coordenada y del extremo 1 del segmento k

> [!NOTE]
>
> - [ ] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 17

> ‼️Hard Problem‼️

El prototipo de la función debe ser:

```cpp
int number_of_regions(
    vector<vector<double>> const& points,
    vector<vector<double>> const& directions,
)
```

Cada elemento de `points` consistirá de 2 valores de tipo double, que
corresponderán a las coordenadas de de un punto. El punto `points[i]` es un punto
por el que pasa la recta `i`.

Cada elemento de `directions` consistirá de 2 valores de tipo double, que
corresponderán a las componentes de un vector de dirección. `directions[i]`
tendrá las componentes del vector de dirección de la recta `i`. Así, la recta `i` será (conceptualmente) el conjunto de puntos que se obtienen con:

```cpp
points[i] + k * directions[i]
```

`points[0][0]` será la coordenada x del punto 0 \
`points[0][1]` será la coordenada y del punto 0

`points[1][0]` será la coordenada x del punto 1 \
`points[1][1]` será la coordenada y del punto 1

...

`points[k][0]` será la coordenada x del punto k \
`points[k][1]` será la coordenada y del punto k

---

`directions[0][0]` será la componente x de la dirección 0 \
`directions[0][1]` será la componente y de la dirección 0

`directions[1][0]` será la componente x de la dirección 1 \
`directions[1][1]` será la componente y de la dirección 1

...

`directions[k][0]` será la componente x de la dirección k \
`directions[k][1]` será la componente y de la dirección k

> [!NOTE]
>
> - [ ] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 18

> ‼️Hard Problem‼️

El prototipo de la función debe ser:

```cpp
int maximum_possible_number_of_regions(int N)
```

Devolver la respuesta **módulo 1000000007**

> [!NOTE]
>
> - [ ] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 19

> ‼️Hard Problem‼️

El prototipo de la función debe ser:

```cpp
template <typename T>
int minimum_covering_area(vector<vector<int>> const& rectangles)
```

`rectangles` tendrá mínimo 1 y máximo 10^9 elementos, cada uno de los cuales consistirá
de 4 enteros, que serán las coordenadas de vértices "inferior izquierdo" y "superior derecho" del rectángulo.

`rectangles[0][0]` será la coordenada x del punto inferior izquierdo del rectangulo 0 \
`rectangles[0][1]` será la coordenada y del punto inferior izquierdo del rectangulo 0 \
`rectangles[0][2]` será la coordenada x del punto superior derecho del rectangulo 0 \
`rectangles[0][3]` será la coordenada y del punto superior derecho del rectangulo 0

`rectangles[1][0]` será la coordenada x del punto inferior izquierdo del rectangulo 1 \
`rectangles[1][1]` será la coordenada y del punto inferior izquierdo del rectangulo 1 \
`rectangles[1][2]` será la coordenada x del punto superior derecho del rectangulo 1 \
`rectangles[1][3]` será la coordenada y del punto superior derecho del rectangulo 1

...

`rectangles[k][0]` será la coordenada x del punto inferior izquierdo del rectangulo k \
`rectangles[k][1]` será la coordenada y del punto inferior izquierdo del rectangulo k \
`rectangles[k][2]` será la coordenada x del punto superior derecho del rectangulo k \
`rectangles[k][3]` será la coordenada y del punto superior derecho del rectangulo k

> [!NOTE]
>
> - [ ] Implementado
> - [ ] Testeado con casos esquina

## PROBLEMA 20

> ‼️Hard Problem‼️

Este ejercicio se corregirá manualmente, por lo que no se establece un prototipo
para el mismo.

> [!NOTE]
>
> - [ ] Implementado
> - [ ] Testeado con casos esquina
