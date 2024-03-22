/* xfa/helpers/fsys.c (Win32)
 *
 * Copyright (c) 2024 RandomGuyWithoutY
 *
 * License: LGPL-3.0-only
 *
 * Platform independent file system helpers.
 *
 */

#define XFA_BUILDING_DLL

// Headers:

	#include "xfa/helpers/fsys.h"

	#include <direct.h>

// Functions:

	int xfa_fsys_chdir(const char *directory)
	{
		return _chdir(directory);
	}

	int xfa_fsys_mkdir(const char *directory)
	{
		return _mkdir(directory);
	}

	// TODO: use a native function from the Win32 API instead of this witchcraft, as said
	//       witchcraft may break down in edgecases. One example of such an edgecase would
	//       be the path `C:file.txt`, where a drive and a file name but no directory
	//       components are present. `xfa_fsys_path_base()` would recognize the entirety
	//       of that string as the file name, which is undesired behaviour.
	const char *xfa_fsys_path_base(const char *path)
	{
		for (size_t i = strlen(path) - 1; i > 0; i--) {
			if (path[i] == '/' || path[i] == '\\') {
				return path + i + 1;
			}
		}

		return path;
	}
