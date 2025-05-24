# Voronoi Diagram

Voronoi es una partición del plano en regiones donde cada región contiene todos los puntos más cercanos a un punto dado (llamado semilla) que a cualquier otra semilla. En otras palabras, cada semilla tiene un espacio geométrico asociado que representa la región de influencia de ese punto.

## Algoritmo (Fortune's Algorithm):

1. Ordenar todos los puntos por coordenada y
   a. Asegurarse de que los puntos estén en orden ascendente.
2. Inicializar una línea de barrido horizontal
   a. La línea se mueve de arriba hacia abajo.
3. Mantener una estructura de "frente de playa" (beach line) con parábolas
   a. Las parábolas representan las regiones de influencia de las semillas activas.
4. Procesar eventos de sitio (nuevos puntos) y eventos de círculo (intersecciones)
   a. Los eventos de sitio ocurren cuando la línea de barrido alcanza una semilla.
   b. Los eventos de círculo ocurren cuando tres parábolas convergen.
5. Para cada evento de sitio: insertar nueva parábola en el frente
6. Para cada evento de círculo: eliminar parábola y crear vértice de Voronoi
7. Construir el diagrama conectando los vértices generados

# Delaunay Triangulation

Es una triangulación de un conjunto de puntos donde ningún punto está dentro del círculo circunscrito de cualquier triángulo. Es el dual geométrico del diagrama de Voronoi.

## Algoritmo (Bowyer-Watson):

1. Crear un triángulo que contenga todos los puntos
2. Para cada punto p:
   - Encontrar todos los triángulos cuyo círculo circunscrito contiene p
   - Eliminar estos triángulos (creando un polígono con agujero)
   - Conectar p con todos los vértices del borde del agujero
3. Eliminar triángulos que contengan vértices del triángulo inicial

# Relación y Conversión

Como la triangulación de Delaunay es el dual del diagrama de Voronoi, se pueden convertir entre sí.

## De Delaunay a Voronoi:

Suponiendo que ya tenemos la triangulación de Delaunay:

1. Para cada triángulo:
   - Calcular el circumcentro (centro del círculo circunscrito)
   - Los circumcentros se convierten en vértices del diagrama de Voronoi
2. Conectar circumcentros de triángulos adyacentes (que comparten una arista)
   - Las conexiones forman las aristas del diagrama de Voronoi
3. Cada arista de Delaunay es perpendicular a su arista dual de Voronoi
4. Retornar el diagrama de Voronoi resultante

## De Voronoi a Delaunay:

Suponiendo que ya tenemos el diagrama de Voronoi:

1. Para cada región de Voronoi:
   - Tomar los vértices de la región como puntos
   - Conectar los puntos de la región a sus vecinos (vértices adyacentes)
2. Los triángulos formados por los puntos son los triángulos de Delaunay
3. Eliminar triángulos que no cumplen la propiedad de Delaunay (si es necesario)
4. Retornar la triangulación de Delaunay resultante
