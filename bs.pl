#!usr/bin/env perl
use strict;
use warnings;

my %generator = (
    '1' => "Ninja",
    '2' => "Unix Makefiles",
    '3' => "Visual Studio 17 2022",
    '4' => "Visual Studio 16 2019",
    '5' => "NMake Makefiles",
    '6' => "Kate - Ninja"
);
unless(-e "./build") {
    mkdir "build";
} else {
    unless (-d "./build") {
        die "Build nie jest folderem";
    }
}
chdir "./build";
my @cmd = (q{cmake});
push @cmd, q{..};
print "Wybierz generator: \n\t1. Ninja\n\t2. Make\n\t3. VS2k22\n\t4. VS2k19\n\t5. NMake\n\t6. Kate\n";
my $num = <STDIN>;
chomp $num;
push @cmd, qq{-G};
push @cmd, $generator{$num};
if ($num eq '4' or $num eq '3') {
    print "Architektura: \n\t1.x64\n\t2.win32\n";
    my $arch = <STDIN>;
    chomp $arch;
    push @cmd, qq{-A};
    if ($arch eq '1') {
        push @cmd, qq{x64};
    } else {
        push @cmd, qq{win32};
    }
}

print "@cmd\n";
system(@cmd);
