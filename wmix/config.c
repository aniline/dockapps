/* WMix -- a mixer using the OSS mixer API.
 * Copyright (C) 2014 Christophe CURIS for the WindowMaker Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*
 * config.c: functions related to loading the configuration, both from
 * command line options and from file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <sys/soundcard.h>

#include "include/common.h"
#include "include/config.h"


#define HELP_TEXT	  \
	"WMixer " VERSION " by timecop@japan.co.jp + skunk@mit.edu\n" \
	"usage:\n" \
	"  -d <dsp>  connect to remote X display\n" \
	"  -e <name> exclude channel, can be used many times\n" \
	"  -f <file> parse this config [~/.wmixrc]\n" \
	"  -h        print this help\n" \
	"  -m <dev>  mixer device [/dev/mixer]\n" \
	"  -v        verbose -> id, long name, name\n" \

/* The global configuration */
struct _Config config;


/*
 * Parse Command-Line options
 *
 * Supposed to be called before reading config file, as there's an
 * option to change its name
 */
void parse_cli_options(int argc, char **argv)
{
	int opt;
	int count_exclude = 0;

	config.verbose = false;
	while ((opt = getopt(argc, argv, "d:e:f:hm:v")) != EOF) {
		switch (opt) {
		case 'd':
			if (optarg != NULL)
				config.display_name = strdup(optarg);
			break;

		case 'e':
			if (count_exclude < SOUND_MIXER_NRDEVICES) {
				config.exclude_channel[count_exclude] = strdup(optarg);
				count_exclude++;
			} else
				fprintf(stderr, "Warning: You can't exclude this many channels\n");
			break;

		case 'f':
			if (optarg != NULL)
				if (config.file != NULL)
					free(config.file);
			config.file = strdup(optarg);
			break;

		case 'h':
			fputs(HELP_TEXT, stdout);
			exit(0);
			break;

		case 'm':
			if (optarg != NULL)
				config.mixer_device = strdup(optarg);
			break;

		case 'v':
			config.verbose = true;
			break;

		default:
			break;
		}
	}
	config.exclude_channel[count_exclude] = NULL;
}

/*
 * Read configuration from a file
 *
 * The file name is taken from CLI if available, of falls back to
 * a default name.
 */
void config_read(void)
{
	FILE *fp;
	char buf[512];
	char *ptr;

	if (config.file == NULL)
		return;

	fp = fopen(config.file, "r");
	if (!fp)
		return;

	while (fgets(buf, 512, fp)) {
		if ((ptr = strstr(buf, "mousewheel="))) {
			ptr += 11;
			config.mousewheel = atoi(ptr);
		}
		if ((ptr = strstr(buf, "scrolltext="))) {
			ptr += 11;
			config.scrolltext = atoi(ptr);
		}
		if ((ptr = strstr(buf, "osd="))) {
			ptr += 4;
			config.osd = atoi(ptr);
		}
		if ((ptr = strstr(buf, "osdcolor="))) {
			char *end;
			ptr += 9;
			end = strchr(ptr, '\n');
			ptr[end - ptr] = '\0';
			if (config.osd_color)
				free(config.osd_color);
			config.osd_color = strdup(ptr);
		}
		if ((ptr = strstr(buf, "wheelstep="))) {
			ptr += 10;
			/* detect old style config */
			if (atoi(ptr) > 1)
				config.scrollstep = (float)atoi(ptr) / 100.0;
			else
				config.scrollstep = atof(ptr);
		}
		if ((ptr = strstr(buf, "wheelbtn1="))) {
			ptr += 10;
			config.wheel_button_up = atoi(ptr);
		}
		if ((ptr = strstr(buf, "wheelbtn2="))) {
			ptr += 10;
			config.wheel_button_down = atoi(ptr);
		}
	}
	fclose(fp);
}
