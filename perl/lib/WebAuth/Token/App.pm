# Perl representation of a WebAuth app token.
#
# Written by Russ Allbery <rra@stanford.edu>
# Copyright 2012
#     The Board of Trustees of the Leland Stanford Junior University
#
# See LICENSE for licensing terms.

package WebAuth::Token::App;

require 5.006;
use strict;
use warnings;

use base qw(WebAuth::Token);

# This version should be increased on any code change to this module.  Always
# use two digits for the minor version with a leading zero if necessary so
# that it will sort properly.
our $VERSION = '1.00';

# Accessor methods.
sub subject         ($;$) { my $t = shift; $t->_attr ('subject',         @_) }
sub last_used       ($;$) { my $t = shift; $t->_attr ('last_used',       @_) }
sub session_key     ($;$) { my $t = shift; $t->_attr ('session_key',     @_) }
sub initial_factors ($;$) { my $t = shift; $t->_attr ('initial_factors', @_) }
sub session_factors ($;$) { my $t = shift; $t->_attr ('session_factors', @_) }
sub loa             ($;$) { my $t = shift; $t->_attr ('loa',             @_) }
sub creation        ($;$) { my $t = shift; $t->_attr ('creation',        @_) }
sub expiration      ($;$) { my $t = shift; $t->_attr ('expiration',      @_) }

1;
