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
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

#include "uxlaunch.h"



static int first_time = 1;

struct timeval start;

#define LOG_MAX 65536
char msglog[LOG_MAX] = "";

void log_string(char *string)
{
	struct timeval current;
	uint64_t secs, usecs;
	char msg[1024];


	if (first_time) {
		first_time = 0;
		gettimeofday(&start, NULL);
	}

	gettimeofday(&current, NULL);

	secs = current.tv_sec - start.tv_sec;
	
	while (current.tv_usec < start.tv_usec) {
		secs --;
		current.tv_usec += 1000000;
	}
	usecs = current.tv_usec - start.tv_usec;

	sprintf(msg, "[%02llu.%06llu] %s", secs, usecs, string);
	if (msg[strlen(msg)] != '\n')
		strcat(msg, "\n");
	fprintf(stderr, msg);

	if ((strlen(msglog) + strlen(msg)) < LOG_MAX)
		strcat(msglog, msg);
}

void close_log(void)
{
	FILE *log;

	log = fopen("/var/log/uxlaunch.log", "w");
	if (log) {
		fputs(msglog, log);
		fclose(log);
	} else {
		log_string("Unable to write log on exit");
	}
}



void log_environment(void)
{
	char **env;

	env = environ;
	
	log_string("Dumping environment");
	log_string("----------------------------------");
	while (env) {
		log_string(*env);
		env++;
	}
	log_string("----------------------------------");
}