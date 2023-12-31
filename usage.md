## Running PANDA
### Executable
Once you have [built](#install.md) PANDA, you'll find an executable either in the build directory (using CMake), or in the bin directory (using gnumake).
To get started, simply execute the binary panda in the appropriate directory.
### Input format
At the very least, you have to provide an input file to PANDA. All other parameters are optional. Create a file according to the input file format specification and execute panda data.
### Interpreting output of PANDA
PANDA displays the results of the transformation to the standard output stream. If you wish redirection to a file, use your operating systems facilities (e.g. `panda data > output`).
Additionally, PANDA displays some information that keeps the user informed of the current status of the calculation. You may see output such as
#### "Processing #1 of at least 10 classes"
Only in Adjacency Decomposition (AD): a class (number 1) has been selected for calculation of adjacent classes. There are 10 classes known so far.
#### "Done processing #1"
Only in AD: The class number 1 has been completely processed. Results have been added.
#### "Fourier-Motzkin elimination step 15/30: 2345"
In double-description method, the program is at step 15 of 30 total steps. The system size is currently 2345. Output per thread appears at most once every two seconds.

You can discard all this output on a typical linux or mac system, by redirecting to `/dev/null`:

```
> panda data 2> /dev/null
```
Alternatively, redirect normal output to some file, additional information will remain visible in the console:

```
> panda data > output
Processing #1 of at least 2 classes
```

### Options for running PANDA
For a technical description on how to run PANDA, click here.
#### Help and version information
`-v`, `--v`, `-version`, `--version` will print the version information. Please include this information in a bug report.
`-h`, `--h`, `-help`, `--help`, `?` and running PANDA without any arguments will display the list of valid options.

For detailed help on a specific option, use the --help-command parameter, e.g. `--help-command=threads`.
#### Method
For transformation from V-description to H-description it is possible to use the double description method instead of the adjacency decomposition.
Note that double description method is usually slower than adjacency decomposition, especially if you have symmetry information at hand.
You may select the method with the command line parameter `-m <arg>` / `--method=<arg>`, where `<arg>` is either `adjacency-decomposition` (short form `ad`) or `double-description` (short form `dd`).

Note for adjacency decomposition for vertex enumeration:

As during this process a newly found row can either be a vertex or an extremal ray, the output cannot be separated.
If the last coefficient is zero, then it represents a ray, otherwise a vertex.
E.g. the output `1 0 0 0` represents the ray `1 0 0`. `0 1 1 1` represents the vertex `0 1 1`. If the last coefficient is not equal to one, it represents the common denominator of all coefficients: `1 2 0 4` represents the vertex `1/4 1/2 0`.
#### Integer arithmetic
The user may choose the integer type that is used for any calculation. If no option is used, the system default type `"int"` is used.
Valid arguments are `16`, `32`, `64` for fixed width integer arithmetic (if provided by the system), `safe` for a fixed width 64-bit integer type that forces the program to abort on any overflow, and `inf`, for a arbitrary precision integer type.
Selection of these options is possible with the `-i <arg>` / `--integer-type=<arg>` switch:

```
> panda -i 16
> panda --integer-type=inf
```
It is highly recommended to always run with option `safe` for guarantee of correctness while still using the performance of native fixed width integer arithmetic.
Any result from a run with `16`, `32`, `64` or without such an argument may be influenced by overflows and therefore might be incorrect.
> #### Multi-core parallelization
PANDA uses parallelization. By default it uses the number of cores of the machine as the number of threads it generates.
However, the user may want to adjust this for optimal performance. You may specify the number of threads per processor with the parameter `-t <arg>` / `--threads=<arg>`:

```
> panda -t 5
> panda --threads=20
```

Note that in conjunction with MPI it is advisable to spawn one process per processor only and to use at least as many threads as cores per processor.
#### Input order
Double description method is highly sensitive to input order. By default, the input is taken as present in file. You may choose to alter the order with the parameter `-s <arg>` / `--sorting=<arg>`, where `<arg>` is one of the following options:
```
"lex_asc" / "lexicographic_ascending" or
"lex_desc" / "lexicographic_descending" or
"nz_asc" / "nonzero_ascending" or
"nz_desc" / "nonzero_descending" or
"rev" / "reverse".
```
#### Prior knowledge about polytope structure
When transforming a V-description to an H-description with adjacency decomposition, it is possible to speed up the calculation by inserting prior knowledge about the facial structure of the polytope.
You may do so by providing a file with an inequality section (see format requirements) and pass it via command line parameter `-k <filename>` / `--known-facets=<filename>`.
#### Providing an input file
A string in the list of command line parameters that does not match one of the options above is interpreted as file name. Examples:
```
> panda myproblem.poi
> panda -t 20 myproblem.poi
> panda -i safe myproblem.poi --threads=20
```

#### Checking validity of input
While consistency (i.e. correct dimension of each input row) is always checked, it is very expensive to check whether data is valid. We consider input to be valid if the provided maps indeed are bijections on the vertices, rays or inequalities.

If you would like to verify validity, pass the option `--check` or `-c`. Verification will be done once in the beginning.
Hence, if you see output it means that the validation process succeeded. Note that this check cannot be performed on reduced input.
If maps are invalid and a reduced input is provided, it is likely to either get garbage output or to quickly run out of memory.
