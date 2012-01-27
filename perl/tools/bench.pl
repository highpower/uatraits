#!/usr/bin/perl

#------------------
use common::sense;
use lib::abs qw(../blib/lib ../blib/arch);
use Benchmark qw(:all);
use JSON::XS;

use uatraits;

my $detector = uatraits->new( lib::abs::path( '../../data/browser.xml' )  );
my $data_file = lib::abs::path( '../../tests/cover.txt');
my $js = JSON::XS->new->ascii->pretty;

my @ua_list;
open( IN, $data_file );
while(<IN>){
    next if (m/^{/); 
    chomp;
    push @ua_list, $_;
}
close( IN );

my $i = $#ua_list;
sub detect {
    #print $js->encode( $detector->detect( $ua_list[$i] ) );
    $detector->detect( $ua_list[$i] );
    if( $i < 0 ){
        $i = $#ua_list;
    }
}
timethis ( 1000000 , sub { detect(); } );

