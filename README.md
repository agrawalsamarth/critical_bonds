# Critical bonds for percolated clusters

We here offer two versions of the critical_bonds software that return the critical bonds for a 2D or 3D undirected bond-connected network of nodes in the presence of periodic boundary conditions: [c++](#c++) and [MATLAB](#MATLAB) versions. Both versions operate on a standardized input file format "cb-input-filename" described [below](#input) or alternatively, a LAMMPS data file. In each case the input file carries information about the box size, coordinates of nodes, and bonded pairs of nodes. We provide a [runner script](#runscript) that operates on a single input file, produces a list of critical bonds and optionally a new LAMMPS data file where critical bonds are masked by their own bond type. The MATLAB version optionally produces graphical output and accepts cb-formatted configurations in arbitrary dimensions. We offer a number of test configurations in the test-configs subdirectory, and converters to switch between file formats, if necessary.  

This software is part of the Supplemental Information of the following publication: 

S. Agrawal, S. Galmarini, M. Kröger, 
Energy formulation for infinite structures: order parameter for percolation, critical bonds and power-law scaling of contact-based transport,
[Phys. Rev. Lett. 132 (2024) 196101](https://doi.org/PhysRevLett.132.196101).

         @article{critical-bonds,
                  author = {S. Agrawal and S. Galmarini and M. Kr\"oger}, 
                  title = {Energy formulation for infinite structures: order parameter for percolation, 
                  critical bonds and power-law scaling of contact-based transport},
                  journal = {Phys. Rev. Lett.},
                  year = {2024},
                  volume = {132},
                  pages = {196101},
                  note = {Code available at https://github.com/agrawalsamarth/critical_bonds}
                  doi = {10.1103/PhysRevLett.132.196101} 
         }

## c++ version <a name="c++"></a>

### Installation 

Clone this Github repository. Then switch to the new *critical_bonds-main* directory, and download the Eigen library. Finally, compile the code via make

         git clone https://github.com/agrawalsamarth/critical_bonds;
         cd critical_bonds; 
         
         perl ./install.pl

This will create the executable *critical_bonds" in the bin subdirectory. The *install.pl* command installs the eigen library and adds absolute paths to all scripts and tests the installation by applying critical_bonds to a test configuration. 

Copy *critical_bonds" or alternatively the "run-critical-bonds" script to a place where it can be found or where you'll use it.

### Basic Usage

         critical_bonds <cb_input_filename> <cb_output_filename>

This script takes a [cb-formatted](#input) input file *cb-input-filename*, runs critical_bonds on it, and saves the critical bonds in *cb-output-filename*. 

### Runner script<a name="runscript"></a>

This script takes an input file *input-filename* (allowed formats listed below), runs critical_bonds on it, and saves the critical bonds in *cb-output-filename*. If the -L option is given, it creates moreover a LAMMPS data file *lammps-data-filename*, in which non-critical bonds have bond type 1, and critical bonds have bond type 2. Such file can be visualized using vmd, ovito, and many others. If called without the -o option, the outputfile is *input-filename*-cb.txt.

         perl run-critical-bonds <input-filename> [-o <cb-output-filename>] [-L <lammps-data-filename>] [-v]

ALLOWED INPUT FILE FORMATS 

1) [cb-formatted](#input) configuration file
2) [LAMMPS data](#lammpsdata) configuration file

OPTIONS

    -o <cb-output-filename>
       writes the ist of critical bonds to the specified file.
    -L <lammps-data-filename>
       writes the created LAMMPS data file to the specified file
    -v
       creates additional stdout.

### cb-formatted input file format<a name=input></a>

For 2D configurations the entries in brackets are absent. The coordinates of the nodes should be between the specified box sizes (xlo, xhi) etc. Node IDs start at 0 and end at number of nodes-1, ie, the first row of the coordinates table corresponds to the node with id 0, and the last row of this table has id number of nodes - 1. These id values are then used to build the corresponding bond table.

    dimensions                     <- space dimension, either 2 or 3
    xlo xhi ylo yhi [zlo zhi]      <- box ranges
    N                              <- number of nodes
    x y [z]                        <- this is the coordinate of node 0
    x y [z]                        <- this is the coordinate of node 1
    ...
    x y [z]                        <- this is the coordinate of node N-1
    number of bonds                <- number of bonded pairs of nodes
    b1 b2                          <- node b1 is bonded to node b2
    b1 b2                          
    ...
    b1 b2                          <- node b1 is bonded to node b2

### LAMMPS data-formatted input file format<a name="lammpsdata">

    .. atoms
    .. bonds
    .. atom types
    .. bond types

    .. .. xlo xhi

    Atoms 

    id mol type q x y z   OR 
    id type x y z   etc. 

    Bonds 

    bid btype b1 b2 

### Format of the cb_output_filename

    b1 b2                          <- 1st critical bond between node b1 and node b2
    b1 b2                          <- 2nd critical bond between node b1 and node b2
    ...
    b1 b2                          <- last critical bond between node b1 and node b2

## critical_bonds for LAMMPS users <a name="LAMMPS"></a>

LAMMPS users can call critical_bonds from within their LAMMPS script, after saving the configuration via using the LAMMPS shell command. 

          write_data config.data 
          shell perl run-critical-bonds config.data -o critical-bonds.txt

## Converters to and from cb-formatted files <a name="converters"></a>

### Convert from LAMMPS data format to cb_input_filename

         perl convert-lammps-data-2-cb.pl <lammps.data> [-o <cb-output-filename>] [-v] [-bondtype=..]

This script converts lammps data format to the critical-bonds [input format](#input).
If called without the -o option, the outputfile is *lammps-data-file*.txt
Note that produced node IDs start at 0 and have no holes after conversion, as this
is required by the critical_bonds input file format. If the critical_bonds
output file is converted back to LAMMPS data format, node IDs start at 1.
All bond tables are adjusted accordingly. If called without argument, this command returns its command syntax. 

OPTIONS

    -o <outputfile>
       writes the converted file to the specified outputfile
    -2D
       assumes two-dimensionsal data, i.e., ignores the z-coordinate.
       By default, a 3D configuration is assumed.
    -bondtype=type1,[type2,[type3,[...]]
       uses only the listed bond types in the generated critical-bonds input file.
       The comma-separated list should not contain blanks
       This option can be used to check if the critical bonds recognized
       by critical_bonds lead to a de-percolated system. By default the
       bond type of critical bonds is 2, so that one may start this script
       with the option -bondtype=1 to keep only the non-critical bonds.
       Examples: -bondtype=1 or -bondtype=1,4,8
    -v
       creates additional stdout.

### Convert output file generated by critical_bonds to LAMMPS data format

         perl convert-cb-2-lammps-data.pl <cb-input-filename> <cb-output-filename> [-o <outputfile>] [-v]

This script converts a pair of cb-input- and cb-output-files to lammps data format.
In the lammps data file, non-critical bonds have bond type 1, critical bonds have bond type $cbtype.
If called without the -o option, the outputfile is *cb-output-filename*.data

OPTIONS

      -o <outputfile>
         writes the lammps data file (atomstyle: full) to the specified outputfile
         LAMMPS Atoms section format: id mol type $q x y z
         LAMMPS Bonds section format: bid btype b1 b2
         This format can be altered by changing mol, type, and q via -mol=.. -type=.. -q=..
         The default is: -mol=$mol -type=$type -q=$q
      -v
         creates additional stdout

### Convert from mol2 format to cb_input_filename

To come.

## MATLAB version<a name="MATLAB">

The MATLAB version is contained in the MATLAB subdirectory. For its description see [here](MATLAB).
