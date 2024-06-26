#! /usr/bin/perl

foreach $arg (@ARGV) { 

   if ($ARGV[0] eq "eigen") { 
      print "added Eigen/Sparse path to post.h\n";
      $pwd=`pwd`; chomp $pwd;	
      $Sparse = `find ./ -name "Sparse" -print | grep -i Eigen`; chomp $Sparse; 
      open(INC,"<include/post.h"); @INC=<INC>; close(INC); 
      open(INC,">include/post.h"); 
      foreach $inc (@INC) { 
         if ($inc=~/Eigen\/Sparse/i) { $inc="#include <$pwd/$Sparse>\n"; };
         print INC $inc;
      }; 
      close(INC); 
   }; 

   if ($ARGV[0] eq "scripts") { 
      print "added absolute paths to scripts\n";
      $pwd=`pwd`; chomp $pwd; $DIR="DIR";
      foreach $script (`ls -1 *.pl run*`) { chomp $script;
         open(S,"<$script"); @S=<S>; close(S);
         foreach $line (0 .. 10) {
            if ($S[$line]=~/INSTALL$DIR/) { $S[$line]="\$INSTALL$DIR=\"$pwd\";\n"; };
         };
         open(S,">$script"); print S @S; close(S);
      };
   }; 

   if ($ARGV[0] eq "benchmark") { 
      print "benchmark testing\n";
      $test_lammps_configuration = "test_configs/config-all-bonds-1.data"; $NC = 129;
      $test_output = "install-test.txt";
      $COMMAND = "perl run-critical-bonds $test_lammps_configuration -o $test_output";
      print "Now executing:\n$COMMAND\n";
      `$COMMAND`;
      $nc = `grep -c . $test_output`+0;
      if ($nc eq $NC) {
         print "successfully tested run-critical-bonds on a test configuration with $NC critical bonds.\nYou are ready to go.\n";
         `rm -f $test_output`;
      } else {
         print<<EOF;
      PROBLEM: The installation was apparently not successful.
      The above command should have returned a file $test_output with $NC critical bonds.
      Please contact us.
EOF
      };
   };

}; 
