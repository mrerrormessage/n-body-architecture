#!/usr/bin/perl
use strict;
use warnings;



#explanation dialogue
if($ARGV[0] =~ m/--help/){
    print("usage: ./testgen.pl [x-size] [y-size] [timestep] [numobjects] [maxmass]");
    exit 0;
}

#first line defines world size, time-step, and number of objects
print("\[ $ARGV[0], $ARGV[1], $ARGV[2], $ARGV[3]\]\n");

for (my $i = 0; $i < $ARGV[3] ; $i++){

#command-line argument
    my $mass = rand( $ARGV[4] );

#command-line argument
    my $x = rand( $ARGV[0] );
    
#command-line argument
    my $y = rand( $ARGV[1] );
    
#each line is printed here
    printf("%i %i %i\n", $x, $y, $mass);
    
}

#close the file here
