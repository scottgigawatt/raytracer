# raytracer

This project contains a raytracer program for creating computer generated graphics.

## Supported Object Types

This project currently supports the following object types:

 - Plane
 - Procedural shaded plane
 - Tiled plane
 - Finite plane
 - Sphere
 - Procedural shaded sphere

## Supported Light Types

This project currently supports the following types of light:

 - Ambient lighting
 - Diffuse reflection
 - Specular reflection

An ambient light source represents an omni-directional, fixed-intensity and fixed-color light source that affects all objects in the scene equally [[1]](https://en.wikipedia.org/wiki/Shading#Ambient_lighting).

Diffuse reflection is the reflection of light from a surface such that an incident ray is reflected at many angles rather than at just one angle as in the case of specular reflection [[2]](https://en.wikipedia.org/wiki/Diffuse_reflection).

Specular reflection, also known as regular reflection is the mirror-like reflection of waves, such as light, from a surface. In this process, each incident ray is reflected, with the reflected ray having the same angle to the surface normal as the incident ray [[3]](https://en.wikipedia.org/wiki/Specular_reflection).

## Input File Examples

Full example files can be found in the [input](input) directory.

### Object Codes

The full list of object codes can be found in [model.h](src/model.h#L19-L40).

```C
#define LIGHT        10
#define SPOTLIGHT    11
#define PROJECTOR    12
#define SPHERE       13
#define PLANE        14
#define FINITE_PLANE 15
#define TILED_PLANE  16
#define TEX_PLANE    17
#define REF_SPHERE   18
#define P_SPHERE     19
#define P_PLANE      20
#define PARABOLOID   21
#define CYLINDER     22
#define CONE         23
#define HYPERBOLOID  24
```

### Scene definitions

```
8 6            World dimensions (x, y)
0 0 3          Viewpoint        (x, y, z)
```

### Procedural plane

```
20             Procedural plane object code
0 10 5         Ambient  (r, g, b)
0 0 0          Diffuse  (r, g, b)
0 0 0          Specular (r, g, b)
0 0 1          Normal   (x, y, z)
-4 0 -10       Center   (x, y, z)
2              Custom shader index
```

### Finite plane
```
15             Finite plane object code
2 2 2          Ambient   (r, g, b)
6 0 6          Diffuse   (r, g, b)
0 0 0          Specular  (r, g, b)
.5  1  1       Normal    (x, y, z)
 4 -1 -2       Center    (x, y, z)
 2  1  1       Direction (x, y, z)
 7  3          Size      (x, y)
```

### Tiled plane

```
16             Tiled plane object code
0.2 0.2 0.2    Ambient  (r, g, b)
0.0 0.0 0.0    Diffuse  (r, g, b)
2.0 2.0 2.0    Specular (r, g, b)
0 1 0          Normal   (x, y, z)
0 -8 0         Center   (x, y, z)
1.0 0.0 1.0    Grid direction 
1.0 1.0        Grid size 
0.2 0.2 0.2    Ambient  (r, g, b)
3.0 3.0 3.0    Diffuse  (r, g, b)
0.0 0.0 0.0    Specular (r, g, b)
```

### Sphere

```
13             Sphere object code
5 1 1          Ambient  (r, g, b)
0 0 0          Diffuse  (r, g, b)
0 0 0          Specular (r, g, b)
1.5 -.34 -6    Center   (x, y, z)
3              Radius
```

### Light

```
10             Light object code
0 5 0          Ambient (r, g, b)
0 2 5          Center  (x, y, z)
```


