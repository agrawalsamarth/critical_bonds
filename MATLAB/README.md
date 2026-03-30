# critical_bonds.m

## Installation

Install MATLAB, if necessary, from [here](https://ch.mathworks.com/products/matlab.html). No special packages are required. 

## Usage 

We offer a MATLAB function critical_bonds, that can be used from within any other MATLAB function as

    bondlist = critical_bonds(cb_input_filename)

    bondlist = critical_bonds(cb_input_filename,cb_output_filename)

    bondlist = critical_bonds(cb_input_filename,cb_output_filename,true)

where bondlist is a N x 2 matrix carrying a pair of bonds in each row. The cb-formatted input (and optionally, output) file format is described below. 
    
The code also be called from the linux command line via (code and arguments enclosed in parentheses!)

     matlab -r "critical_bonds 'cb_input_filename' ['cb_output_filename'] [true]"

or from the matlab command line via 

    critical_bonds(cb_input_filename[,cb_output_filename][,true])

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

### Format of the cb_output_filename

    b1 b2                          <- 1st critical bond between node b1 and node b2
    b1 b2                          <- 2nd critical bond between node b1 and node b2
    ...
    b1 b2                          <- last critical bond between node b1 and node b2



