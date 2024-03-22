/* main.c
 *
 * Copyright (c) 2024 RandomGuyWithoutY
 *
 * License: GPL-3.0-only
 *
 * xfa CLI utility.
 *
 */

// Headers:

	#include <stdio.h>

	#include "xfa/xfa.h"

// Functions:

	int main(int argc, char **argv)
	{
		char *file_name;

		if (argc < 2) {
			printf(
				"Xfa 1.1 -- Back in C\n"
				"Copyright (c) 2024 RandomGuyWithoutY\n"
				"Released into the public domain.\n"
				"\n"
				"Usage: xfa <operation> <file>\n"
				"\n"
				"Operations:\n"
				"   d - defragment file\n"
				"   f - fragment file\n"
				"  zd - defragment and unzip file\n"
				"  zf - zip and fragment file\n"
				"\n"
			);

			file_name = NULL;
		} else if (argc < 3) {
			file_name = NULL;
		} else {
			file_name = argv[2];
		}

		printf("File: %s\n", file_name);

		return 0;
	}