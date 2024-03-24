/* xfa/helpers/io.h
 *
 * Copyright (c) 2024 AnarchySoft
 *
 * License: LGPL-3.0-only
 *
 * Platform independent I/O helper functions.
 *
 */

#define XFA_BUILDING_DLL

// Headers:

	#include "xfa/helpers/io.h"

// Constants:

	const int XFA_ENDIAN_LIL = 1;
	#define XFA_ENDIAN_LIL    (*(char *)&XFA_ENDIAN_LIL)

// Macros:

	#define XFA_SWAP_16(num)  ( \
		(((num) << 010) & 0xff00) | \
		(((num) >> 010) & 0x00ff) \
	)
	#define XFA_SWAP_32(num)  ( \
		(((num) << 030) & 0xff000000) | \
		(((num) << 010) & 0x00ff0000) | \
		(((num) >> 010) & 0x0000ff00) | \
		(((num) >> 030) & 0x000000ff) \
	)
	#define XFA_SWAP_64(num)  ( \
		(((num) << 070) & 0xff00000000000000) | \
		(((num) << 050) & 0x00ff000000000000) | \
		(((num) << 030) & 0x0000ff0000000000) | \
		(((num) << 010) & 0x000000ff00000000) | \
		(((num) >> 010) & 0x00000000ff000000) | \
		(((num) >> 030) & 0x0000000000ff0000) | \
		(((num) >> 050) & 0x000000000000ff00) | \
		(((num) >> 070) & 0x00000000000000ff) \
	)

// Public functions:

#define CONSTRUCT_READ_HELPER
#define CONSTRUCT_WRITE_HELPER
	#define TNAME u8
	#define TYPE  uint8_t
	#undef  SWAP
		#include "io.def.h"
	#undef TNAME
	#undef TYPE
	#undef SWAP

	#define TNAME u16
	#define TYPE  uint16_t
	#define SWAP  XFA_SWAP_16
		#include "io.def.h"
	#undef TNAME
	#undef TYPE
	#undef SWAP

	#define TNAME u32
	#define TYPE  uint32_t
	#define SWAP  XFA_SWAP_32
		#include "io.def.h"
	#undef TNAME
	#undef TYPE
	#undef SWAP

	#define TNAME u64
	#define TYPE  uint64_t
	#define SWAP  XFA_SWAP_64
		#include "io.def.h"
	#undef TNAME
	#undef TYPE
	#undef SWAP
#undef CONSTRUCT_WRITE_HELPER
#undef CONSTRUCT_READ_HELPER

	bool xfa_helper_read_cmp(FILE *file_obj, const char *cmp_bytes, size_t byte_count)
	{
		int current_byte;

		for (size_t cur = 0; cur < byte_count; cur++) {
			current_byte = fgetc(file_obj);

			if (current_byte == EOF || current_byte != cmp_bytes[cur]) {
				return 1;
			}
		}

		return 0;
	}

	bool xfa_helper_read_array(FILE *file_obj, void **dest_array, size_t byte_count)
	{
		if (
			(*dest_array == NULL) &&
			(*dest_array = malloc(byte_count)) == NULL
		) {
			return 1;
		}

		if (!fread(*dest_array, byte_count, 1, file_obj)) {
			free(*dest_array);
			*dest_array = NULL;

			return 1;
		}

		return 0;
	}

	bool xfa_helper_write_array(FILE *file_obj, const void *src_array, size_t byte_count)
	{
		return !fwrite(src_array, byte_count, 1, file_obj);
	}
