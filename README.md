# Critical bonds for percolated clusters

We here offer two versions of the critical_bonds software that returns the critical bonds for a 2D or 3D bond-connected network of nodes in the presence of periodic boundary conditions. Both versions operate on a standardized input file format described below. The input file carries information about the box size, coordinates of nodes, and bonded pairs of nodes. We offer converters to this input file format from other data formats like LAMMPS data files. Other converters will be added. 

critical_bonds

I have a few questions: 
1) Can the IDs start at 1 or 5, for example?
2) Can the IDs have holes, can one have IDs 2,13,25 and no others?
3) Do you remove dangling bonds before solving the system?
4) Do you remove nodes with only two attached bonds before solving the system? 
5) Can you add a verbose option to your code like -v ? If -v is given, can you print
some information to stdout like number of nodes, bonds, clusters, if the system is percolated
or not, and the number of critical bonds? 
6) Can I download the latest github version? 
7) Did you finish editing the README.md? 

Answers:
1) For N nodes, the IDs must be between (0,N-1), they can be tabulated in any order.
2) No holes allowed. There must be N nodes with (0,N-1) ids, and no repetitions.
3) No I don't remove the danglinbg ends
4) No I don't remove such nodes as well, and don't see why we should?
5) I can do that now
6) Yes. Some of the exception handling is missing, but if the format of the inout file is correct, it'll produce correct results without a doubt
7) I think I will add these details now, and then you can have a look. I'm not very familiar with .md style, but I can try

We can improve on 3) + 4) later on and make it more robust by relaxing conditions on 1) and 2) in due time. But given my health and the deadline we can proceed with this for now.

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


