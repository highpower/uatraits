#!/usr/bin/perl
# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl uatraits.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';
use strict;
use Test::More tests => 8;
BEGIN { use_ok('uatraits') };

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.
my $run_name = $0;
my $dir = $run_name;
$dir =~s/\/[^\/]*$//;
my $browser_path = $dir . "/../../data/browser.xml";

ok(-f $browser_path, "browser.xml found");
 
my $ok = 1;
my $obj;
eval {
    uatraits->new('');
    $ok = 0;
};

is( $ok , 1 , 'uatraits->new dies, when path to bowser.xml is invalid' );
is( $obj, undef, 'object not created');

eval {
    $obj = uatraits->new($browser_path);
};
ok( $obj , "uatraits object is created");
is( $@, '', "no errors");

#subtest "detect" => sub {
    unless( $obj ){
        fail( 'cannot detect without an object');
#        done_testing();
#        return;
    } else {
        my $resp = $obj->detect('Mozilla/5.0 (Linux; U; Android 2.3; ru-ru; Desire_A8181 Build/FRF91) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1');
        isnt( $resp, undef, "traits->detect returns response");
        is( ref( $resp) , 'HASH', "response is hash");
    }
#    done_testing();
#};

#done_testing();
