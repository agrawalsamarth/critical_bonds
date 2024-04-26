#! /usr/bin/perl

# (c) 18 april 2024 Martin Kroger, ETH Zurich, mk@mat.ethz.ch 

no warnings 'experimental::smartmatch';

sub USAGE { print<<EOF;
NAME
       convert-lammps-data-2-cb.pl

SYNOPSIS
       perl convert-lammps-data-2-cb.pl <lammps.data> [-o <cb-output-filename>] [-v] [-bondtype=..]

DESCRIPTION
       This script converts lammps data format to the critical-bonds input format
       If called without the -o option, the outputfile is <lammps-data-file>.txt
       Note that produced node IDs start at 0 and have no holes after conversion, as this
       is required by the critical_bonds input file format. If the critical_bonds 
       output file is converted back to LAMMPS data format, node IDs start at 1. 
       All bond tables are adjusted accordingly. 
      
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
EOF
exit
};

if ($#ARGV eq -1) { USAGE; };
$datafile=$ARGV[0]; $dimensions=3; 
if (-s "$datafile") { } else { print "missing file $datafile\n"; exit; };
foreach $iarg (0 .. $#ARGV)  { $arg=$ARGV[$iarg]; ($field,$value)=split(/=/,$arg);
   if ($arg eq "-o") { 
      $outputfile=$ARGV[$iarg+1]; 
   } elsif ($field eq "-o") {
      $outputfile=$value; 
   } elsif ($arg eq "-2D") { 
      $dimensions=2; 
   } elsif ($field eq "-bondtype") {
      @use_bondtypes=split(/,/,$value); 
   } elsif ($arg eq "-v") { 
      $verbose=1; 
   }; 
}; 
if (!$outputfile) { $outputfile="$datafile.txt"; }; 

sub strip { chomp $_[0]; $_[0]=~s/^\s+//g; $_[0]=~s/\s+$//; $_[0]=~s/\s+/ /g; $_[0]; };

open(D,"<$datafile"); 
while (!eof(D)) { 
   $line=<D>; $line=strip($line); @vals=split(/ /,$line);
   if       ($line=~/atoms/)  { $atoms=$vals[0]; print "\nstarted conversion\n$atoms atoms ";
   } elsif  ($line=~/bonds/)  { $bonds=$vals[0]; print "$bonds bonds.\n";
   } elsif  ($line=~/xlo/)    { $xlo=$vals[0]; $xhi=$vals[1]; 
   } elsif  ($line=~/ylo/)    { $ylo=$vals[0]; $yhi=$vals[1];
   } elsif  ($line=~/zlo/)    { $zlo=$vals[0]; $zhi=$vals[1];
   } elsif  ($line=~/^Atoms/) { $line=<D>; 
      foreach $iat (0 .. $atoms-1) {
         $line=<D>; $line=strip($line); 
         $ignore = index $line,"\#";
         if ($ignore>0) { $line = substr($line,0,$ignore-1); };  # strip comments
         @vals=split(/ /,$line);
         if (!$noted) { print "lammps data Atoms section has 1+$#vals columns\n"; };
         if      ($#vals eq 9) {
            $id=$vals[0];
            ($dummy,$mol[$id],$type[$id],$q[$id],$x[$id],$y[$id],$z[$id],$ix,$iy,$iz)=split(/ /,$line);
            if (!$noted) { print "assuming lammps Atoms format: id mol type q x y z ix iy iz\n"; $noted=1; };
         } elsif ($#vals eq 6) {
            $id=$vals[0];
            ($dummy,$mol[$id],$type[$id],$q[$id],$x[$id],$y[$id],$z[$id])=split(/ /,$line); 
            if (!$noted) { print "assuming lammps Atoms format: id mol type q x y z\n"; $noted=1; };
         } elsif ($#vals eq 8) {
            $id=$vals[0];
            ($dummy,$mol[$id],$type[$id],$x[$id],$y[$id],$z[$id],$ix,$iy,$iz)=split(/ /,$line);
            if (!$noted) { print "assuming lammps Atoms format: id mol type x y z ix iy iz\n"; };
            if (!$noted) { print "                          OR  id type q x y z ix iy iz\n"; $noted=1; };
         } elsif ($#vals eq 7) {
            $id=$vals[0];
            ($dummy,$type[$id],$x[$id],$y[$id],$z[$id],$ix,$iy,$iz)=split(/ /,$line);
            if (!$noted) { print "assuming lammps Atoms format: id type x y z ix iy iz\n"; $noted=1; };
         } elsif ($#vals eq 5) {
            $id=$vals[0];
            ($dummy,$type[$id],$x[$id],$y[$id],$z[$id])=split(/ /,$line);
            if (!$noted) { print "assuming lammps Atoms format: id type x y z\n"; $noted=1; };
         } else {
            print "\n\nlammps data-file is corrupt or the atom_style is not recognized here. Modify this script here.\n";   
         }; 
         $ID[$iat]=$id;
         if ($dimensions eq 2) { $z[$id]=""; $zlo=""; $zhi=""; }; 
      }; 
   } elsif  ($line=~/^Bonds/) { $line=<D>;
      $usebonds = 0; 
      $ignored_bonds=0;
      foreach $bid (1 .. $bonds) {
         $line=<D>; $line=strip($line); @vals=split(/ /,$line);
         $btype   =$vals[1];
         if ($#use_bondtypes eq -1) { 
            $usebonds+=1;
            $b1[$usebonds]=$vals[2];
            $b2[$usebonds]=$vals[3];
         } elsif ($btype~~@use_bondtypes) { 
            $usebonds+=1; 
            $b1[$usebonds]=$vals[2];
            $b2[$usebonds]=$vals[3];
         } else {
            $ignored_bonds+=1; 
         };
      };
      if ($bonds eq $usebonds) { } else { $bonds=$usebonds; print "Using $usebonds bonds (selected types @use_bondtypes)\n"; }; 
   }; 
};
close(D);
if ($verbose) { print "$ignored_bonds bonds have been ignored due to their type.\n"; }; 

if ($#b1 eq $bonds) { } else { print "data format error in bonds section\n"; }; 

if ($verbose) { print "\n"; foreach $j (0 .. 10) { print "data line $j carries original ID $ID[$j]\n"; }; print "\n"; };

# sort existing IDs 
@newID2ID = sort { $a <=> $b } @ID;    # $newID2ID[0] = smallest lammps ID (sID)

if ($verbose) { foreach $j (0 .. 10) { print "new ID $j -> original ID $newID2ID[$j]\n"; }; print "\n"; };

# reverse lookup table
foreach $j (0 .. $#newID2ID) { $ID2newID[$newID2ID[$j]] = $j; };  # $ID2newID[sID] = 0;

if ($verbose) { foreach $id (1 .. 10) { print "original ID $id -> new ID $ID2newID[$id]\n"; }; print "\n"; }; 

if ($#newID2ID eq $atoms-1) { } else { print "data format error in atoms section\n"; }; 

if ($verbose) { foreach $bid (1 .. 10) { print "bond ID $bid: ($b1[$bid])-($b2[$bid]) -> ($ID2newID[$b1[$bid]],$ID2newID[$b2[$bid]])\n"; }; };

open(OUT,">$outputfile"); 
print OUT<<EOF;
$dimensions
$xlo $xhi $ylo $yhi $zlo $zhi
EOF
print OUT "$atoms\n"; foreach $iat (1 .. $atoms) { $id=$newID2ID[$iat-1]; print OUT "$x[$id] $y[$id] $z[$id]\n"; };
print OUT "$bonds\n"; foreach $bid (1 .. $bonds) { print OUT "$ID2newID[$b1[$bid]] $ID2newID[$b2[$bid]]\n"; };   
close(OUT);
print "\ncreated $outputfile\n";
print "\nobtain critical bonds via:\ncritical_bonds $outputfile <cb-output-filename>\n";
