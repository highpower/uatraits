#!/usr/bin/perl 

use strict;
use uatraits;

eval {
	my $traits = uatraits->new("browser.xml");
	my $result = $traits->detect("");
};

if ($@) {
	print "error occured: $@";
}
