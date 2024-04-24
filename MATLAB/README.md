description missing

# critical_bonds.m

## Installation

Install MATLAB, if necessary, from [here](https://ch.mathworks.com/products/matlab.html). No special packages are required. 

## Usage 

We offer a MATLAB function critical_bonds, that can be used from within any other MATLAB function as

    bondlist = critical_bonds(cb_input_filename)

    bondlist = critical_bonds(cb_input_filename,cb_output_filename)

    bondlist = critical_bonds(cb_input_filename,cb_output_filename,true)

where bondlist is a N x 2 matrix carrying a pair of bonds in each row. 
    
It can also be called from the command line via

    critical_bonds(cb_input_filename)

    critical_bonds(cb_input_filename,cb_output_filename)

    critical_bonds(cb_input_filename,cb_output_filename,true)



