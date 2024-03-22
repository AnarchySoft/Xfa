/* xfa/xfa.c
 *
 * Copyright (c) 2024 RandomGuyWithoutY
 *
 * License: LGPL-3.0-only
 *
 * xfa C library.
 *
 */

#define XFA_BUILDING_DLL

// Headers:

	#include <inttypes.h>

	#include "xfa/xfa.h"
	#include "xfa/sha256.h"

	#include "xfa/helpers/fsys.h"
	#include "xfa/helpers/io.h"

// Internal constants:

	#define XFA_FRAGMENT_BUFF_SIZE  0x1000000  // 10 MiB

// Functions:

	XFA_Index *xfa_read_index(const char *file_path, XFA_Index *index)
	{
		void *hash_ptr;
		FILE *file_obj = fopen(file_path, "rb");

		if (file_obj == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));

			return NULL;
		}

		if (
			xfa_helper_read_cmp(file_obj, ".XFA\0", 5) ||
			xfa_helper_read_u8(file_obj, &index->version_major) ||
			xfa_helper_read_u8(file_obj, &index->version_minor)
		) {
			fprintf(stderr, "Error: %s\n", ferror(file_obj) ? "Read operation failed" : feof(file_obj) ? "Unexpected end of file" : "Unknown file format");
			fclose(file_obj);

			return NULL;
		}

		if (index->version_major != 1 || index->version_minor > 0) {
			fprintf(stderr, "Error: Unsupported file version\n");
			fclose(file_obj);

			return NULL;
		}

		// NOTE: initializing `file_name` to NULL is necessary so the helper function allocates the required buffer
		index->file_name = NULL;

		if (
			xfa_helper_read_u16(file_obj, &index->name_length) ||
			// NOTE: `file_name` is not a valid C string at this point and all operations have to use `name_length` to prevent out-of-bounds buffer access
			xfa_helper_read_array(file_obj, (void **)&index->file_name, index->name_length) ||
			xfa_helper_read_u32(file_obj, &index->fragment_count)
		) {
			fprintf(stderr, "Error: %s\n", ferror(file_obj) ? "Read operation failed" : feof(file_obj) ? "Unexpected end of file" : "Insufficient heap memory");
			fclose(file_obj);
			free(index->file_name);

			return NULL;
		}

		if (index->fragment_count <= 0) {
			return index;
		}

		index->fragment_hash = malloc(index->fragment_count * 32);

		if (index->fragment_hash == NULL) {
			fprintf(stderr, "Error: Insufficient heap memory\n");
			fclose(file_obj);
			free(index->file_name);

			return NULL;
		}

		hash_ptr = &index->fragment_hash[0][0];

		if (xfa_helper_read_array(file_obj, &hash_ptr, index->fragment_count * 32)) {
			fprintf(stderr, "Error: %s\n", ferror(file_obj) ? "Read operation failed" : feof(file_obj) ? "Unexpected end of file" : "Undefined failure");
			fclose(file_obj);
			free(index->file_name);
			free(index->fragment_hash);

			return NULL;
		}

		fclose(file_obj);

		return index;
	}


	int xfa_write_index(XFA_Index *index, const char *file_path)
	{
		void *hash_ptr;
		FILE *file_obj = fopen(file_path, "wb");

		if (file_obj == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));

			return 1;
		}

		if (
			xfa_helper_write_array(file_obj, ".XFA\0", 5) ||
			xfa_helper_write_u8(file_obj, index->version_major) ||
			xfa_helper_write_u8(file_obj, index->version_minor) ||
			xfa_helper_write_u16(file_obj, index->name_length) ||
			xfa_helper_write_array(file_obj, index->file_name, index->name_length) ||
			xfa_helper_write_u32(file_obj, index->fragment_count)
		) {
			fprintf(stderr, "Error: Write operation failed\n");
			fclose(file_obj);

			return 1;
		}

		if (index->fragment_count <= 0) {
			fclose(file_obj);

			return 0;
		}

		hash_ptr = &index->fragment_hash[0][0];

		if (xfa_helper_write_array(file_obj, &hash_ptr, index->fragment_count * 32)) {
			fprintf(stderr, "Error: Write operation failed\n");
			fclose(file_obj);

			return 1;
		}

		fclose(file_obj);

		return 0;
	}

	int xfa_fileop_fragment(const char *file_name, size_t fragment_size)
	{
		struct {
			char fragment[XFA_FRAGMENT_BUFF_SIZE];
			XFA_SHA256_Stream sha256;
		} *memory_block = malloc(sizeof(*memory_block));

		FILE *file_obj = fopen(file_name, "rb");
		FILE *idx_file = fopen("Index.xfa", "wb"); // TODO: write the index file to a new directory where all fragments are stored
		FILE *frag_file;
		char  frag_fname[15];
		void *fragment = &memory_block->fragment;

		size_t frag_ptr = 0;
		size_t frag_left = fragment_size;
		size_t read_size;

		uint32_t frag_count = 0;
		fpos_t file_off;

		XFA_SHA256_Stream *sha256 = &memory_block->sha256;
		XFA_Hash hash;

		int err = 0;

		if (
			file_obj == NULL ||
			idx_file == NULL ||
			memory_block == NULL
		) {
			return 1;
		}

		if (
			xfa_helper_write_array(idx_file, ".XFA\0", 5) ||
			xfa_helper_write_u8(idx_file, 1) ||
			xfa_helper_write_u8(idx_file, 0) ||
			xfa_helper_write_u16(idx_file, strlen(file_name)) || // TODO: normalize file name
			xfa_helper_write_array(idx_file, file_name, strlen(file_name)) ||
			fgetpos(idx_file, &file_off) ||
			xfa_helper_write_u32(idx_file, 0)
		) {
			fclose(file_obj);
			fclose(idx_file);
			free(memory_block);

			return 1;
		}

		while ((read_size = fread(fragment, 1, XFA_FRAGMENT_BUFF_SIZE, file_obj))) {
			while (frag_ptr < read_size) {
				size_t next_size = (read_size - frag_ptr < frag_left)
					? (read_size - frag_ptr)
					: frag_left;

				if (frag_left == fragment_size) {
					xfa_init_sha256(sha256);

					// NOTE: this will fail if the fragment count is larger than 8 digits and the buffer
					//       is sized to only fit those digits and the file extension.
					//       The maximum fragment count is 4294967295 (0xFFFFFFFF), which is 10 digits long,
					//       so the true maximum name length would be 10 (count) + 4 (extension) + 1 (null) = 15.
					sprintf(frag_fname, "%08"PRIu32".xfa", frag_count);

					frag_file = fopen(frag_fname, "wb"); // TODO: error handling
				}

				xfa_feed_sha256(sha256, fragment + frag_ptr, next_size);
				xfa_helper_write_array(frag_file, fragment, read_size);  // TODO: error handling

				frag_ptr += next_size;
				frag_left -= next_size;

				if (!frag_left) {
					xfa_close_sha256(sha256, hash);
					fclose(frag_file);

					if (xfa_helper_write_array(idx_file, hash, sizeof(hash))) {
						fclose(file_obj);
						fclose(idx_file);
						free(memory_block);

						return 1;
					}

					frag_count++;
					frag_left = fragment_size;
				}
			}
		}

		// TODO: add check wether EOF or read error occured

		if (frag_left != fragment_size) {
			xfa_close_sha256(sha256, hash);
			fclose(frag_file);

			if (xfa_helper_write_array(idx_file, hash, sizeof(hash))) {
				fclose(file_obj);
				fclose(idx_file);
				free(memory_block);

				return 1;
			}

			frag_count++;
		}

		fflush(idx_file);

		if (
			fsetpos(idx_file, &file_off) ||
			xfa_helper_write_u32(idx_file, frag_count)
		) {
			err = 1;
		}

		fclose(file_obj);
		fclose(idx_file);
		free(memory_block);

		return err;
	}

	int xfa_fileop_defragment(const char *fragment_index);
