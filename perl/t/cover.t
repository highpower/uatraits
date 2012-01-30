#!/usr/bin/perl

use strict;
use Test::More qw(no_plan);
use JSON::XS;

BEGIN { use_ok('uatraits') };

my $run_name = $0;
    my $dir = $run_name;
    $dir =~s/\/[^\/]*$//;
my $browser_path = $dir . "/../../data/browser.xml";
my $cover_path   = $dir . "/../../tests/cover.txt";
my $cover_sh     = $dir . "/../../tests/cover-json.sh";

ok( -f $browser_path , "browser.xml");
ok( -f $cover_path, "cover.txt");

my $ua_detector = uatraits->new( $browser_path );
ok( $ua_detector , "object of uatraits created");


open (TEST_DATA, $cover_sh . "| sed 's/\"true\"/1/g' | sed 's/\"false\"/0/g' |" ) || die 'cannot open '. $cover_path;
while(<TEST_DATA>){
    chomp;
    my $ua = $_;
    #$test_data->{'UA'};

    my $test_data = JSON::XS::decode_json( <TEST_DATA> );

    if( $ua ){
        #delete $test_data->{UA};
        my $resp = $ua_detector->detect( $ua );
        ok( $resp , "UA: $ua has response");
        #is( keys %$resp, keys %$test_data, "keys count equal");
        foreach my $key ( keys %$test_data ){
            is( $resp->{$key}, $test_data->{$key},  "UA: $ua, Field: $key");
        }
        #diag( JSON::XS->new->pretty->encode( { 'UA' => $ua , 'test reference' => $test_data, 'lib response' => $resp } ) );
    }
    #last;
}
close(TEST_DATA);





