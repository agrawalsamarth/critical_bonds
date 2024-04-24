# Critical bonds for percolated clusters

We here offer two versions of the critical_bonds software that returns the critical bonds for a 2D or 3D bond-connected network of nodes in the presence of periodic boundary conditions: A c++ version and a MATLAB version. Both versions operate on a standardized input file format described below. The input file carries information about the box size, coordinates of nodes, and bonded pairs of nodes. We offer converters to this input file format from other data formats like LAMMPS data files. Other converters will follow. Suggestions for required converters are welcome. The MATLAB version optionally produces graphical output and/or a LAMMPS data file that masks the critical bonds by bond type 2. 

## c++ version

### Installation

Clone this Github repository. Then install the code via make

         git clone https://github.com/agrawalsamarth/critical_bonds;
         cd critical_bonds; 
         make critical_bonds

Note that Eigen library is required. It comes with this repository. If necessary, it can be installed separately via 

          git clone https://gitlab.com/libeigen/eigen.git

### Usage

         ./bin/critical_bonds <input_filename> <output_filename>

### Format of the input_filename 

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

### Format of the output_filename

    b1 b2                          <- 1st critical bond between node b1 and node b2
    b1 b2                          <- 2nd critical bond between node b1 and node b2
    ...
    b1 b2                          <- last critical bond between node b1 and node b2


