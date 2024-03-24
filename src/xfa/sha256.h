/* xfa/sha256.h
 *
 * Copyright (c) 2024 AnarchySoft
 *
 * License: LGPL-3.0-only
 *
 * Modified SHA-256 implementation from <https://github.com/amosnier/sha-2>.
 *
 */

#pragma once

// Headers:

	#include "xfa/xfa.h"

// Constants:

	#define XFA_SHA_256_CHUNK_SIZE 64

// Data types:

	typedef struct {
		uint8_t  chunk[XFA_SHA_256_CHUNK_SIZE];
		uint8_t *chunk_pos;
		size_t   space_left;
		uint64_t total_len;
		uint32_t h[8];
	} XFA_SHA256_Stream;

// Functions:

	XFA_PUBLIC void xfa_init_sha256(XFA_SHA256_Stream *sha256);
	XFA_PUBLIC void xfa_feed_sha256(XFA_SHA256_Stream *sha256, const void *data, size_t len);
	XFA_PUBLIC void xfa_close_sha256(XFA_SHA256_Stream *sha256, XFA_Hash hash);
