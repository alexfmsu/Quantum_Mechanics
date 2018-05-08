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

`rm -r ./$section/Config/* ./$section/out/* 2>/dev/null || true`;

# for my $capacity (8, 6) {
	for my $n (1, 5, 10, 15, 20, 25) {
        my $capacity = $n;

		my $config_path     = "./$section/Config";
        		
        my $config_filename = $capacity . "_" . $n;

        `mkdir -p $section/Config`;
		`mkdir -p $section/out/`;

		for my $T (0.25) {
		    my $wc = 21.506 . " * GHz";

		    my $wa = 21.506 . ' * GHz';
		   	
		   	my $g;

		    if($n==1){
				$g = (1 * 21.506 * 1e-2) . ' * GHz';
		    }else{
		    	$g = (1 * 21.506 * 1e-2) . ' * GHz';
		    }
		   	# my $g = (1 * 21.506 * 1e-2 / 2/$n**(0)) . ' * GHz';
		   	# my $g = (1 * 21.506 * 1e-2) . ' * GHz';

		    my $init_state = "[" . 0 . ", " . $n . "]";

		    my $conf = BipartiteConfig->new(
		        capacity   => $capacity,
		        n          => $n,
		        wc         => $wc,
		        wa         => $wa,
		        g          => $g,
		        nt         => 20000,
		        T          => $T . " * mks",
		        init_state => $init_state,
		        precision  => 5,
		        path 	   => "\"$section/out/$n/\""		    );

		    $conf->write_to_file($config_path . "/" . $n) or die $!;

            push @job, $n;
		}
	}
# }


for(@job){
    say "-" x 20;

    `cp $section/Config/$_ Bipartite/config.py`;

    system("python3 -O Bipartite.py");

    say "-" x 20;
}
