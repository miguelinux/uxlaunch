/*
 * This file is part of uxlaunch
 *
 * (C) Copyright 2009 Intel Corporation
 * Authors: 
 *     Auke Kok <auke@linux.intel.com>
 *     Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include <security/pam_appl.h>

#include "uxlaunch.h"

pam_handle_t *ph;
struct pam_conv pc;

void setup_pam_session(void)
{
	char msg[256];
	int err;

	log_string("** Entering setup_pam_session");

	err = pam_start("login", pass->pw_name, &pc, &ph);
	err = pam_open_session(ph, 0);
	if (err != PAM_SUCCESS) {
		sprintf(msg, "pam_open_session returned %d: %s\n", err, pam_strerror(ph, err));
		log_string(msg);
		exit(1);
	}

	err = pam_set_item(ph, PAM_TTY, displaydev);
	if (err != PAM_SUCCESS) {
		sprintf(msg, "pam_set_item returned %d: %s\n", err, pam_strerror(ph, err));
		log_string(msg);
		exit(1);
	}
}

void close_pam_session(void)
{
	char msg[256];
	int err;

	log_string("** Entering close_pam_session");

	err = pam_close_session(ph, 0);
	if (err) {
		sprintf(msg, "pam_close_session returned %d: %s\n", err, pam_strerror(ph, err));
		log_string(msg);
	}
	pam_end(ph, err);
}
