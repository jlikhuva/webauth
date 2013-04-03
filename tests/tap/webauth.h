/*
 * Helper functions for testing WebAuth code.
 *
 * Additional functions that are helpful for testing WebAuth code and have
 * knowledge of WebAuth functions and data structures.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2013
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#ifndef TAP_WEBAUTH_H
#define TAP_WEBAUTH_H 1

#include <config.h>
#include <tests/tap/macros.h>

struct webauth_token_webkdc_factor;

BEGIN_DECLS

/* Compare two webkdc-factor tokens. */
void is_token_webkdc_factor(const struct webauth_token_webkdc_factor *wanted,
                            const struct webauth_token_webkdc_factor *seen,
                            const char *format, ...)
    __attribute__((__format__(printf, 3, 4)));

END_DECLS

#endif /* !TAP_WEBAUTH_H */
