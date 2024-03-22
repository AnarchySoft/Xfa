/* xfa/xfa.h
 *
 * Copyright (c) 2024 RandomGuyWithoutY
 *
 * License: LGPL-3.0-only
 *
 * xfa C library.
 *
 */

#pragma once

// Headers:

	#include <stdlib.h>
	#include <stdio.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <string.h>

// Macros:

	#if defined _WIN32 || defined __CYGWIN__
		#ifdef XFA_BUILDING_DLL
			#ifdef __GNUC__
				#define XFA_PUBLIC __attribute__ ((dllexport))
			#else
				#define XFA_PUBLIC __declspec(dllexport)
			#endif
		#else
			#ifdef __GNUC__
				#define XFA_PUBLIC __attribute__ ((dllimport))
			#else
				#define XFA_PUBLIC __declspec(dllimport)
			#endif
		#endif
		#define XFA_LOCAL
	#else
		#if __GNUC__ >= 4
			#define XFA_PUBLIC __attribute__ ((visibility ("default")))
			#define XFA_LOCAL  __attribute__ ((visibility ("hidden")))
		#else
			#define XFA_PUBLIC
			#define XFA_LOCAL
		#endif
	#endif

// Data types:

	typedef uint8_t  XFA_Hash[32];

	typedef struct {
		uint8_t   version_major;
		uint8_t   version_minor;
		uint16_t  name_length;
		char     *file_name;  // NOTE: deallocated in the main function
		uint32_t  fragment_count;
		XFA_Hash *fragment_hash;  // NOTE: deallocated in the main function
	} XFA_Index;

// Functions:

	XFA_PUBLIC XFA_Index *xfa_read_index(const char *file_path, XFA_Index *index);
	XFA_PUBLIC int xfa_write_index(XFA_Index *index, const char *file_path);
	XFA_PUBLIC int xfa_fileop_fragment(const char *file_name, size_t fragment_size);
	XFA_PUBLIC int xfa_fileop_defragment(const char *fragment_index);