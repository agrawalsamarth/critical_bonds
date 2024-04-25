# Critical bonds for percolated clusters

We here offer two versions of the critical_bonds software that return the critical bonds for a 2D or 3D undirected bond-connected network of nodes in the presence of periodic boundary conditions: [c++](#c++) and [MATLAB](#MATLAB) versions. Both versions operate on a standardized input file format "cb-input-filename" described [below](#input). The input file carries information about the box size, coordinates of nodes, and bonded pairs of nodes. We offer [converters](#converters) to this input file format from other data formats like LAMMPS data files. Other converters will follow. Suggestions for required converters are welcome. The MATLAB version optionally produces graphical output and/or a LAMMPS data file that masks the critical bonds by bond type 2. We offer a number of test configurations in the test-configs subdirectory. 

This software is part of the Supplemental Information of the following publication: 

S. Agrawal, S. Galmarini, M. Kröger, 
Energy formulation for infinite structures: order parameter for percolation, critical bonds and power-law scaling of contact-based transport,
Phys. Rev. Lett. (2024) in press since 12 Apr 2024

         @article{critical-bonds,
                  author = {S. Agrawal and S. Galmarini and M. Kr\"oger}, 
                  title = {Energy formulation for infinite structures: order parameter for percolation, 
                  critical bonds and power-law scaling of contact-based transport},
                  journal = {Phys. Rev. Lett.},
                  year = {2024},
                  volume = {XX},
                  pages = {XX},
                  note = {Code available at https://github.com/agrawalsamarth/critical_bonds}
                  doi = {XX} 
         }

## c++ version<a name="c++">

### Installation

Clone this Github repository. Then install the code via make

         git clone https://github.com/agrawalsamarth/critical_bonds;
         cd critical_bonds; 
         make critical_bonds

Note that Eigen library is required. It comes with this repository. If necessary, it can be installed separately via 

          git clone https://gitlab.com/libeigen/eigen.git

### Usage

         ./bin/critical_bonds <cb_input_filename> <cb_output_filename>

### Format of the cb_input_filename<a name=input>

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

#### Example: 

2D Configuration. 5 nodes connected by 6 bonds contained in a periodic box of size [0 1] x [-0.2 1.1]:

    2
    0 1 -0.2 1.1
    5
    0.2 0.3  
    -0.1 0.5 
    0.4 0.2 
    0.8 0.3
    0.7 0.4
    6
    2 3
    0 1
    1 2
    3 1 
    1 4
    4 2

### Format of the cb_output_filename

    b1 b2                          <- 1st critical bond between node b1 and node b2
    b1 b2                          <- 2nd critical bond between node b1 and node b2
    ...
    b1 b2                          <- last critical bond between node b1 and node b2

## LAMMPS helper script

### Run cricital_bonds on a LAMMPS data file

         perl run-critical-bonds-on-LAMMPS-data <lammps-data-filename> [-o <cb-lammps-data-filename>] [-v]

This script takes a LAMMPS data file < lammps-data-filename >, runs critical_bonds on it,
interprets all existing bonds as bond type 1, and saves a new LAMMPS data
file < cb-lammps-data-filename >, in which critical bonds have bond type 2.
If called without the -o option, the outputfile is <lammps-data-filename>-cb.data.
If called without arguments, this command returns its command syntax. 

OPTIONS

    -o <outputfile>
       writes the created LAMMPS data file to the specified outputfile
    -2D
       assumes two-dimensionsal data, i.e., ignores the z-coordinate.
       By default, a 3D configuration is assumed.
    -v
       creates additional stdout.

## Converters<a name="converters">

### Convert from LAMMPS data format to cb_input_filename

         perl convert-lammps-data-2-cb.pl <lammps.data> [-o <cb-output-filename>] [-v] [-bondtype=..]

This script converts lammps data format to the critical-bonds [input format](#input).
If called without the -o option, the outputfile is < lammps-data-file >.txt
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
If called without the -o option, the outputfile is <cb-output-filename>.data

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
