#!/usr/bin/perl 

use strict;
use uatraits;
use Data::Dumper;

eval {
	my $traits = uatraits->new("../data/browser.xml");
	my $result = $traits->detect("UA Opera/9.7 (WindowsMobile; PPC; Opera Mobi/35267; U; ru; Presto/2.1.1)");
	print Dumper($result);
};

if ($@) {
	print "error occured: $@";
}
