use 5.24.0;
use strict;
use warnings;
use utf8;
use DDP;

use JSON::XS;

use lib ".";

use BipartiteConfig;

my $section = "Bipartite";

my @job = ();

$| = 1;

`rm -r ./$section/out/3d/* 2>/dev/null || true`;
# `rm -r ./$section/Config/* ./$section/out/* 2>/dev/null || true`;

# for my $capacity (8, 6) {
# print();
for my $n (1 .. 50) {
    my $capacity = $n;

    my $config_path = "./$section/Config";

    my $config_filename = $capacity . "_" . $n;

    `mkdir -p $section/Config`;
    `mkdir -p $section/out/`;

    for my $T (0.5) {
        for my $g (map {$_ / 20} 1..20) {
            my $wc = 21.506 . " * GHz";
            say $g, "\t", sprintf ("%.10f",  $g * 21.506 * 1e-2);

            my $wa = 21.506 . ' * GHz';

            #   if($n==1){
            # $g = (1 * 21.506 * 1e-2) . ' * GHz';
            #   }else{
            #   	$g = (1 * 21.506 * 1e-2) . ' * GHz';
            #   }
            # my $g = (1 * 21.506 * 1e-2 / 2/$n**(0)) . ' * GHz';
            # my $g = (1 * 21.506 * 1e-2) . ' * GHz';

            my $init_state = "[" . 0 . ", " . $n . "]";

            my $conf = BipartiteConfig->new(
                capacity   => $capacity,
                n          => $n,
                wc         => $wc,
                wa         => $wa,
                g          => sprintf ("%.10f",  $g * 21.506 * 1e-2) . ' * GHz',
                nt         => 20000,
                T          => $T . " * mks",
                init_state => $init_state,
                precision  => 5,
                path       => "\"$section/out/3d/$n\_$g/\""
            );

            $conf->write_to_file($config_path . "/" . $n . '_' . $g) or die $!;

            push @job, "$n\_$g";
        }
    }
}

for (@job) {
    say "-" x 20;

    `cp $section/Config/$_ Bipartite/config.py`;

    system("python3 -O Bipartite.py");

    say "-" x 20;
}
