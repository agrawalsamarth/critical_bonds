# Critical bonds for percolated clusters

node id start at 0 or 1 and are id holes allowed or not? need to mention this here. Must x y z be contained in the periodic box or can they be outside? I would mention.

## Input format 

Node IDs start at 0 and end at number of nodes -1. The table of values can be in randomized order, but all the ids of the nodes must fill all values between 0 to N-1. For 2D configurations the entries in brackets are absent.

    dimensions                     <- space dimension, either 2 or 3
    xlo xhi ylo yhi [zlo zhi]      <- box ranges
    N                              <- number of nodes
    id x y [z]                        <- this is the id followed by the coordinate of node 0
    id x y [z]                        <- this is the id followed by the coordinate of node 1
    ...
    id x y [z]                        <- this is the id followed by the coordinate of node N-1
    number of bonds                <- number of bonded pairs of nodes
    b1 b2                          <- node b1 is bonded to node b2
    b1 b2                          
    ...
    b1 b2                          <- node b1 is bonded to node b2

## Output format

    b1 b2                          <- 1st critical bond between node b1 and node b2
    b1 b2                          <- 2nd critical bond between node b1 and node b2
    ...
    b1 b2                          <- last critical bond between node b1 and node b2


# Installation and Usage

Please install the code using:

## make critical_bonds

Note that Eigen library is required. The library can be used through the command:

## ./bin/critical_bonds <input_filename> <output_filename>
