#ifdef CONSTRUCT_READ_HELPER
#ifndef TNAME
	#error Type name not defined!
#endif
#ifndef TYPE
	#error Data type not defined!
#endif
#define _FNAME_INNER1(_TNAME) xfa_helper_read_##_TNAME
#define _FNAME_INNER2(_TNAME) _FNAME_INNER1(_TNAME)
#define FNAME _FNAME_INNER2(TNAME)
bool FNAME(FILE *file_obj, TYPE *num)
{
	if (!fread(num, sizeof(*num), 1, file_obj)) {
		return 1;
	}

#ifdef SWAP
	if (!XFA_ENDIAN_LIL) {
		*num = SWAP(*num);
	}
#endif

	return 0;
}
#undef _FNAME_INNER1
#undef _FNAME_INNER2
#undef FNAME
#endif
#ifdef CONSTRUCT_WRITE_HELPER
#ifndef TNAME
	#error Type name not defined!
#endif
#ifndef TYPE
	#error Data type not defined!
#endif
#define _FNAME_INNER1(_TNAME) xfa_helper_write_##_TNAME
#define _FNAME_INNER2(_TNAME) _FNAME_INNER1(_TNAME)
#define FNAME _FNAME_INNER2(TNAME)
bool FNAME(FILE *file_obj, TYPE num)
{
#ifdef SWAP
	if (!XFA_ENDIAN_LIL) {
		num = SWAP(num);
	}
#endif

	return !fwrite(&num, sizeof(num), 1, file_obj);
}
#undef _FNAME_INNER1
#undef _FNAME_INNER2
#undef FNAME
#endif