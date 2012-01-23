#!/usr/bin/perl 

use strict;
use uatraits;

eval {
	my $traits = uatraits->new("browser.xml");
	my $result = $traits->detect("");
	for (my ($key, $value) = each(%$result)) {
		print "$key = $value\n";
	}
};

if ($@) {
	print "error occured: $@";
}
