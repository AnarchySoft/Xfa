/* xfa/helpers/fsys.c (Unix)
 *
 * Copyright (c) 2024 AnarchySoft
 *
 * License: LGPL-3.0-only
 *
 * Platform independent file system helpers.
 *
 */

#define XFA_BUILDING_DLL

// Headers:

	#include "xfa/helpers/fsys.h"

	#include <unistd.h>
	#include <sys/stat.h>

// Functions:

	int xfa_fsys_chdir(const char *directory)
	{
		return chdir(directory);
	}

	int xfa_fsys_mkdir(const char *directory)
	{
		return mkdir(directory, 0777);
	}

	const char *xfa_fsys_path_base(const char *path)
	{
		for (size_t i = strlen(path) - 1; i > 0; i--) {
			if (path[i] == '/') {
				return path + i + 1;
			}
		}

		return path;
	}
