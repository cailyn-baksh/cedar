#!/usr/bin/perl

use strict;

sub main() {
	my ($inPath, $outPath) = @ARGV;
	my %symbols;

	open my $inFile,$inPath or die;

	# Find all public symbols
	while (my $line = <$inFile>) {
		$symbols{$1} = 1 if ($line =~ /public\s([^\s]+)\s*$/);
	}

	seek($inFile, 0, 0);  # Seek to beginning of inFile
	open my $outFile,">",$outPath or die;  # Create out file

	while (my $line = <$inFile>) {
		if ($line =~ /extern\s([^\s]+)\s*$/) {
			next if (exists($symbols{$1}));

		}

		print $outFile $line;
	}

	close $outFile;
	close $inFile;
}

&main();