#! /usr/bin/perl

sub USAGE { print<<EOF;
NAME
       convert-lammps-data-2-cb.pl

SYNOPSIS
       perl convert-lammps-data-2-cb.pl <lammps.data> [-o <outputfile>]

DESCRIPTION
       This script converts lammps data format to the critical-bonds data format .cb
       If called without the -o option, the outputfile is <lammps-data-file>.cb
      
OPTIONS
      -o <outputfile> 
         writes the converted file to the specified outputfile      
      -2D
         assumes two-dimensionsal data, i.e., ignores the z-coordinate
EOF
exit
};

if ($#ARGV eq -1) { USAGE; };
$datafile=$ARGV[0]; $dimensions=3; 
if (-s "$datafile") { } else { print "missing file $dtafile\n"; exit; };
foreach $iarg (0 .. $#ARGV)  { $arg=$ARGV[$iarg]; 
   if ($arg eq "-o") { 
      $outputfile=$ARGV[$iarg+1]; 
   } elsif ($arg eq "-2D") { 
      $dimensions=2; 
   }; 
}; 
if (!$outputfile) { $outputfile="$datafile.cb"; }; 

sub strip { chomp $_[0]; $_[0]=~s/^\s+//g; $_[0]=~s/\s+$//; $_[0]=~s/\s+/ /g; $_[0]; };

open(D,"<$datafile"); 
while (!eof(D)) { 
   $line=<D>; $line=strip($line); @vals=split(/ /,$line);
   if       ($line=~/atoms/)  { $atoms=$vals[0]; print "$atoms atoms ";
   } elsif  ($line=~/bonds/)  { $bonds=$vals[0]; print "$bonds bonds\n";
   } elsif  ($line=~/xlo/)    { $xlo=$vals[0]; $xhi=$vals[1]; 
   } elsif  ($line=~/ylo/)    { $ylo=$vals[0]; $yhi=$vals[1];
   } elsif  ($line=~/zlo/)    { $zlo=$vals[0]; $zhi=$vals[1];
   } elsif  ($line=~/^Atoms/) { $line=<D>; 
      foreach $iat (1 .. $atoms) {
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
            if (!$noted) { print "assuming lammps Atoms format: id mol type x y z ix iy iz OR \n"; };
            if (!$noted) { print "                              id type q x y z ix iy iz\n"; $noted=1; };
         } elsif ($#vals eq 7) {
            $id=$vals[0];
            ($dummy,$type[$id],$x[$id],$y[$id],$z[$id],$ix,$iy,$iz)=split(/ /,$line);
            if (!$noted) { print "assuming lammps Atoms format: id type x y z ix iy iz\n"; $noted=1; };
         } elsif ($#vals eq 5) {
            $id=$vals[0];
            ($dummy,$type[$id],$x[$id],$y[$id],$z[$id])=split(/ /,$line);
            if (!$noted) { print "assuming lammps Atoms format: id type x y z\n"; $noted=1; };
         } else {
            print "lammps data-file is corrupt. Edit this script.\n";   
         }; 
         $ID[$iat]=$id;
         print "DEBUG ID[$iat] = $id;\n";
         if ($dimensions eq 2) { $z[$id]=""; $zlo=""; $zhi=""; }; 
      }; 
   } elsif  ($line=~/^Bonds/) { $line=<D>;
      foreach $bid (1 .. $bonds) {
         $line=<D>; $line=strip($line); @vals=split(/ /,$line);
         $b1[$bid]=$vals[2];
         $b2[$bid]=$vals[3];
      };
   }; 
};
close(D);

if ($#b1 eq $bonds) { } else { print "data format error in bonds section\n"; }; 

@IDsorted = sort { $a <=> $b } @ID; 

open(A,">$outputfile"); 
print A<<EOF;
$dimensions
$xlo $xhi $ylo $yhi $zlo $zhi
EOF
print A "$atoms\n"; foreach $iat (1 .. $atoms) { $id=$IDsorted[$iat]; print A "$id $x[$id] $y[$id] $z[$id]\n"; };
print A "$bonds\n"; foreach $bid (1 .. $bonds) { print A "$id $b1[$bid] $b2[$bid]\n"; };   
close(A);
print "created $outputfile\n";
