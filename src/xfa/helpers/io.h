/* xfa/helpers/io.h
 *
 * Copyright (c) 2024 AnarchySoft
 *
 * License: LGPL-3.0-only
 *
 * Platform independent I/O helper functions.
 *
 */

#pragma once

// Headers:

	#include "xfa/xfa.h"

// Functions:

	// Read helpers:
	XFA_LOCAL bool xfa_helper_read_cmp(FILE *file_obj, const char *cmp_bytes, size_t byte_count);
	XFA_LOCAL bool xfa_helper_read_array(FILE *file_obj, void **dest_array, size_t byte_count);
	XFA_LOCAL bool xfa_helper_read_u8(FILE *file_obj, uint8_t *num);
	XFA_LOCAL bool xfa_helper_read_u16(FILE *file_obj, uint16_t *num);
	XFA_LOCAL bool xfa_helper_read_u32(FILE *file_obj, uint32_t *num);
	XFA_LOCAL bool xfa_helper_read_u64(FILE *file_obj, uint64_t *num);

	// Write helpers:
	XFA_LOCAL bool xfa_helper_write_array(FILE *file_obj, const void *src_array, size_t byte_count);
	XFA_LOCAL bool xfa_helper_write_u8(FILE *file_obj, uint8_t num);
	XFA_LOCAL bool xfa_helper_write_u16(FILE *file_obj, uint16_t num);
	XFA_LOCAL bool xfa_helper_write_u32(FILE *file_obj, uint32_t num);
	XFA_LOCAL bool xfa_helper_write_u64(FILE *file_obj, uint64_t num);
