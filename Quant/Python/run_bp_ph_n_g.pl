
use 5.24.0;
use strict;
use warnings;
use utf8;
use DDP;

use JSON::XS;

use lib ".";

use BipartiteNConfig;

use BipartiteConfig;

my $section = "Bipartite_ph_n_g";

my @job = ();

$| = 1;

`rm -r ./$section/Config/* ./$section/out/* 2>/dev/null || true`;

my @n = grep { $_ % 10 == 0 } 20 .. 20;

my @wc = (21.506);    # MHz
my @wa = (21.506);    # MHz

my @g = map { $_ / 50 } 1 .. 50;    # MHz

my @T = (1);                        # mks

my @nt = (20000);

my $out = 'out';

`mkdir -p $section/Config`;
`mkdir -p $section/out/`;

for my $n (@n) {
    for my $capacity (grep { $_ % 5 == 0 } 65 .. $n + 100) {
        for my $wc (@wc) {
            for my $wa (@wa) {
                for my $g (@g) {

                    #     my $config_path = './config';
                    # # my $config_path = "./$section/Config";
                    #     my $out_path = "$n/$capacity\_$g";

                    #     my $nt = '20000';
                    # # my $config_filename = $capacity . "_" . $n;

                    for my $T (@T) {
                        # print $g;

                        my $config_path = './config';

                        # my $out_path = "$capacity\_$n\_$g";
                        my $out_path = "$n/$capacity\_$g";

                        my $nt = '20000';

                        my $init_state = '[' . 0 . ', ' . $n . ']';

                        my $config_filename = "$n\_$capacity\_$g";

                        my $conf = BipartiteNConfig->new(
                            capacity => $capacity,
                            n        => $n,
                            wc       => $wc . ' * GHz',
                            wa       => $wa . ' * GHz',
                            g        => ($g * 21.506 * 1e-2) . ' * GHz',
                            T        => $T . ' * mks',
                            # dt       => $dt,
                            nt         => int($T * $nt),
                            dt         => 'T / ' . int($T * $nt),
                            init_state => $init_state,
                            precision  => 50,
                            path       => '"' . $section . '/' . $out . '/' . $out_path . '/"'
                        );

                        # print($config_path . "/" . $config_filename);

                        $conf->write_to_file($config_path . "/" . $config_filename);
                        # exit(0);
                        push @job, $n;
                    }
                }
            }
        }
    }
}

for (@job) {
    say "-" x 20;

    `cp $section/Config/$_ Bipartite/config.py`;

    system("python3 -O Bipartite.py");

    say "-" x 20;
}
