#! /usr/bin/perl

$pwd=`pwd`; chomp $pwd; $DIR="DIR";

# add absolute path of the installation directory to scripts
foreach $script (`ls -1 *.pl run*`) { chomp $script;
   open(S,"<$script"); @S=<S>; close(S);
   foreach $line (0 .. 10) {
      if ($S[$line]=~/INSTALL$DIR/) {
         $S[$line]="\$INSTALL$DIR=\"$pwd\";\n";
      };
   };
   open(S,">$script"); print S @S; close(S);
};

# test benchmark configuration
# $test_lammps_configuration = "test_configs/config-all-bonds-1.data";
# `perl run-critical-bonds-on-LAMMPS-data $test_lammps_configuration -o test-1.txt`;
# `bin/critical_bonds $test_configuration test-result.txt`;
