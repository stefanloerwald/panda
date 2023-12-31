## Writing PANDA input files
PANDA takes data from an input file. The following fields are possible:
Names
Maps
Vertices
Rays
Equations
Inequalities
Reduced Vertices
Reduced Rays
Reduced Inequalities

PANDA also supports PORTA syntax using the keywords

```
DIM
INDEX
CONV_SECTION
CONE_SECTION
INEQUALITY_SECTION
```

There are sample files available [here](samples/).

For a very technical description, start [here](TODO add link).

### Dimension information
You may optionally provide dimension information. This may help you with identifying errors in the input data. Valid format is `DIM=N`, where N is the dimension.
### Names of coordinates
Variables may be named. This allows pretty printing of inequalities (e.g. `x + y <= 1`) and enables the possibility of adding symmetry information. Valid format is:

```
Names
x y z
```

The keywords `Names`, `INDEX`, `INDICES` and `NAMES` may be used interchangeably.
### Symmetry information
PANDA uses symmetry information to speed up the adjacency decomposition and help to remove redundant information from output. Symmetry information is given in the format of affine maps.
To be valid, each affine map must be a bijection on the set of vertices and rays. Example:

```
Maps
y x z
y z x
x+y x z
```


Each line contains exactly one map. The first one swaps the first and second coordinate, the second maps `x` to `y`, `y` to `z` and `z` to `x` and finally the third map maps `x` to `x+y`, `y` to `x` and `z` to `z`.
### Rays and Vertices
A polyhedron `P` may be described by vertices `V` and rays `R`. `P = conv(V) + cone(R) = {x | x = a + b, a in conv(V), b in cone(R)}`.
The keywords for these two sets are `Vertices` and `Rays`. Each row must contain at most one vertex / ray. Coordinates may not contain floating point numbers. Examples:

```
Vertices
0 0 0
0 0 1
0 1 0
0 1 1
1 0 0
1 0 1
1 1 0
1 1 1
```

```
Vertices
0 0 1/2
0 1/2 0
Rays
1 0 0
0 1 0
0 0 1
```

PANDA is compatible with PORTA and supports the `CONV_SECTION` / `CONE_SECTION` syntax:

```
CONV_SECTION
0 0 1/2
0 1/2 0
1/2 0 0
CONE_SECTION
1 0 0
0 1 0
0 0 1
END
```

### Inequalities and equations
A polyhedron may be described through a set of inequalities. It is the intersection of the halfspaces defined by each inequality.
If the coordinates are named, inequalities may be written in the format `-x +2y <= 1`, i.e. variables are separated by at least one space,
the coefficient directly preceeds the variable name without any space in between. Relation operator may be `<=`, `>=` or `=`.
If the coordinates are unnamed, inequalities are expected to be represented as a vector of its coefficients.
Note that the right hand side is negated and that the operator `<=` holds. Example: `-1 2 -1`, which translates to `-x +2y -1 <= 0` / `-x + 2y <= 1`.
Example:

```
Equations
x - y = 0
Inequalities
x + y <= 1
x >= 0
```

PANDA is compatible with PORTA and supports the `INEQUALITY_SECTION` syntax:
All inequalities must be enclosed by an `INEQUALITY_SECTION` and an `END`.

### Reduced input
For convenience, PANDA allows input to only have one representative per vertex / facet class.
E.g. the linear ordering polytope of size 6 with its 720 vertices may be represented by just one vertex representative.
Example input files: lop_6

To mark a set of vertices as reduced, the keyword `Reduced Vertices` must be used. The keywords `Reduced Rays` and `Reduced Inequalities` are defined analogously.

**Warning: using `Reduced Inequalities` requires the full set of equations to be included in the input file. Otherwise expansion of facet classes may fail silently (c.f. lop_6).**
