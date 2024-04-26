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
$test_lammps_configuration = "test_configs/config-all-bonds-1.data";
`perl run-critical-bonds $test_lammps_configuration -o .tmp-test.txt`;
$nc = `grep -c . .tmp-test.txt`+0;
if ($nc eq 129) {
   print "successfully tested run-critical-bonds on a test configuration. You are ready to go.\n";
   `rm -f .tmp-test.txt`;
} else {
   print<<EOF;
PROBLEM: The installation was apparently not successful.
perl run-critical-bonds $test_lammps_configuration -o test-1.txt
should return a file test-1.txt with 129 critical bonds.
Please contact us.
EOF
};
