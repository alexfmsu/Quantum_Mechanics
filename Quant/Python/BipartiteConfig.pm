package BipartiteConfig;

use Moose;

has [ "capacity", "n", "nt", "precision" ] => (
    is  => "ro",
    isa => "Int");

has [ "wc", "wa", "g", "T", "dt", "init_state", "path"] => (
    is  => "ro",
    isa => "Str");

sub write_to_file($$) {
    my ( $self, $filename ) = @_;

    my $fh;

    open( $fh, ">", $filename );

    print $fh "KHz = 10 ** 3  # 1 KГц" . "\n";
    print $fh "MHz = 10 ** 6  # 1 МГц" . "\n";
    print $fh "GHz = 10 ** 9  # 1 ГГц" . "\n";
    print $fh "\n";
    print $fh "mks = 1e-6  # 1 мкс" . "\n";
    print $fh "ns = 1e-9  # 1 мкс" . "\n";
    print $fh "\n";

    print $fh
        "# BEGIN--------------------------------------------------- CAVITY -----------------------------------------------------"        . "\n";
    print $fh "capacity = " . $self->capacity . "\n";
    print $fh "n = " . $self->n . "\n";
    print $fh "\n";
    print $fh "wc = " . $self->wc . "\n";
    print $fh "wa = " . $self->wa . "\n";
    print $fh "g  = " . $self->g . "\n";
    print $fh
        "# END----------------------------------------------------- CAVITY -----------------------------------------------------"        . "\n";
    print $fh "\n";
    print $fh
        "# ---------------------------------------------------------------------------------------------------------------------"        . "\n";
    print $fh "T = " . $self->T . "\n";
    print $fh "nt = " . $self->nt . "\n";
    print $fh "nt = int(T/mks*nt)" . "\n";
    print $fh "dt = T/nt" . "\n";
    print $fh
        "# ---------------------------------------------------------------------------------------------------------------------"        . "\n";
    print $fh "\n";
    print $fh "init_state = " . $self->init_state . "\n";
    print $fh "\n";
    print $fh "precision = " . $self->precision . "\n";
    print $fh "\n";
    print $fh
        "# ---------------------------------------------------------------------------------------------------------------------"        . "\n";
    print $fh "path = " . $self->path;
    print $fh "\n";
    print $fh "H_csv = path + \"H.csv\"" . "\n";
    print $fh "U_csv = path + \"U.csv\"" . "\n";
    print $fh "\n";
    print $fh "x_csv = path + \"x.csv\"" . "\n";
    print $fh "y_csv = path + \"t.csv\"" . "\n";
    print $fh "z_csv = path + \"z.csv\"" . "\n";
    print $fh "\n";
    print $fh "fid_csv = path + \"fid.csv\"" . "\n";
    print $fh
        "# ---------------------------------------------------------------------------------------------------------------------"        . "\n";

    close($fh);
}

1;

