/* xfa/helpers/fsys.h
 *
 * Copyright (c) 2024 AnarchySoft
 *
 * License: LGPL-3.0-only
 *
 * Platform independent file system helpers.
 *
 */

#pragma once

// Headers:

	#include "xfa/xfa.h"

// Functions:

	XFA_LOCAL int xfa_fsys_chdir(const char *directory);
	XFA_LOCAL int xfa_fsys_mkdir(const char *directory);
	XFA_LOCAL int xfa_fsys_isdir(const char *path);
	XFA_LOCAL int xfa_fsys_exists(const char *path);
	XFA_LOCAL const char *xfa_fsys_path_base(const char *path);
