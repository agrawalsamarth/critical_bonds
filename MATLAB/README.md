The description is not yet complete. Use the c++ for the time being. 

# critical_bonds.m

## Installation

Install MATLAB, if necessary, from [here](https://ch.mathworks.com/products/matlab.html). No special packages are required. 

## Usage 

We offer a MATLAB function critical_bonds, that can be used from within any other MATLAB function as

    bondlist = critical_bonds(cb_input_filename)

    bondlist = critical_bonds(cb_input_filename,cb_output_filename)

    bondlist = critical_bonds(cb_input_filename,cb_output_filename,true)

where bondlist is a N x 2 matrix carrying a pair of bonds in each row. 
    
It can also be called from the linux command line via (code and arguments enclosed in parentheses!)

     matlab -r "critical_bonds 'cb_input_filename' ['cb_output_filename'] [true]"

or from the matlab command line via 

    critical_bonds(cb_input_filename[,cb_output_filename][,true])



