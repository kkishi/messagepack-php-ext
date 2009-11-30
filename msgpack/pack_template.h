/*
 * MessagePack packing routine template
 *
 * Copyright (C) 2008-2009 FURUHASHI Sadayuki
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define __LITTLE_ENDIAN__
#elif __BYTE_ORDER == __BIG_ENDIAN
#define __BIG_ENDIAN__
#endif
#endif


#ifdef __LITTLE_ENDIAN__

#define STORE8_BE8(d) \
	((uint8_t*)&d)[0]


#define STORE16_BE8(d) \
	((uint8_t*)&d)[0]

#define STORE16_BE16(d) \
	((uint8_t*)&d)[1], ((uint8_t*)&d)[0]


#define STORE32_BE8(d) \
	((uint8_t*)&d)[0]

#define STORE32_BE16(d) \
	((uint8_t*)&d)[1], ((uint8_t*)&d)[0]

#define STORE32_BE32(d) \
	((uint8_t*)&d)[3], ((uint8_t*)&d)[2], ((uint8_t*)&d)[1], ((uint8_t*)&d)[0]


#define STORE64_BE8(d) \
	((uint8_t*)&d)[0]

#define STORE64_BE16(d) \
	((uint8_t*)&d)[1], ((uint8_t*)&d)[0]

#define STORE64_BE32(d) \
	((uint8_t*)&d)[3], ((uint8_t*)&d)[2], ((uint8_t*)&d)[1], ((uint8_t*)&d)[0]

#define STORE64_BE64(d) \
	((uint8_t*)&d)[7], ((uint8_t*)&d)[6], ((uint8_t*)&d)[5], ((uint8_t*)&d)[4], \
	((uint8_t*)&d)[3], ((uint8_t*)&d)[2], ((uint8_t*)&d)[1], ((uint8_t*)&d)[0]


#elif __BIG_ENDIAN__

#define STORE8_BE8(d) \
	((uint8_t*)&d)[0]


#define STORE16_BE8(d) \
	((uint8_t*)&d)[1]

#define STORE16_BE16(d) \
	((uint8_t*)&d)[0], ((uint8_t*)&d)[1]


#define STORE32_BE8(d) \
	((uint8_t*)&d)[3]

#define STORE32_BE16(d) \
	((uint8_t*)&d)[2], ((uint8_t*)&d)[3]

#define STORE32_BE32(d) \
	((uint8_t*)&d)[0], ((uint8_t*)&d)[1], ((uint8_t*)&d)[2], ((uint8_t*)&d)[3]


#define STORE64_BE8(d) \
	((uint8_t*)&d)[7]

#define STORE64_BE16(d) \
	((uint8_t*)&d)[6], ((uint8_t*)&d)[7]

#define STORE64_BE32(d) \
	((uint8_t*)&d)[4], ((uint8_t*)&d)[5], ((uint8_t*)&d)[6], ((uint8_t*)&d)[7]

#define STORE64_BE64(d) \
	((uint8_t*)&d)[0], ((uint8_t*)&d)[1], ((uint8_t*)&d)[2], ((uint8_t*)&d)[3], \
	((uint8_t*)&d)[4], ((uint8_t*)&d)[5], ((uint8_t*)&d)[6], ((uint8_t*)&d)[7]

#endif

#ifndef msgpack_pack_inline_func
#error msgpack_pack_inline_func template is not defined
#endif

#ifndef msgpack_pack_user
#error msgpack_pack_user type is not defined
#endif

#ifndef msgpack_pack_append_buffer
#error msgpack_pack_append_buffer callback is not defined
#endif


/*
 * Integer
 */

#define msgpack_pack_real_uint8(x, d) \
do { \
	if(d < (1<<7)) { \
		/* fixnum */ \
		msgpack_pack_append_buffer(x, &STORE8_BE8(d), 1); \
	} else { \
		/* unsigned 8 */ \
		const unsigned char buf[2] = {0xcc, STORE8_BE8(d)}; \
		msgpack_pack_append_buffer(x, buf, 2); \
	} \
} while(0)

#define msgpack_pack_real_uint16(x, d) \
do { \
	if(d < (1<<7)) { \
		/* fixnum */ \
		msgpack_pack_append_buffer(x, &STORE16_BE8(d), 1); \
	} else if(d < (1<<8)) { \
		/* unsigned 8 */ \
		const unsigned char buf[2] = {0xcc, STORE16_BE8(d)}; \
		msgpack_pack_append_buffer(x, buf, 2); \
	} else { \
		/* unsigned 16 */ \
		const unsigned char buf[3] = {0xcd, STORE16_BE16(d)}; \
		msgpack_pack_append_buffer(x, buf, 3); \
	} \
} while(0)

#define msgpack_pack_real_uint32(x, d) \
do { \
	if(d < (1<<8)) { \
		if(d < (1<<7)) { \
			/* fixnum */ \
			msgpack_pack_append_buffer(x, &STORE32_BE8(d), 1); \
		} else { \
			/* unsigned 8 */ \
			const unsigned char buf[2] = {0xcc, STORE32_BE8(d)}; \
			msgpack_pack_append_buffer(x, buf, 2); \
		} \
	} else { \
		if(d < (1<<16)) { \
			/* unsigned 16 */ \
			const unsigned char buf[3] = {0xcd, STORE32_BE16(d)}; \
			msgpack_pack_append_buffer(x, buf, 3); \
		} else { \
			/* unsigned 32 */ \
			const unsigned char buf[5] = {0xce, STORE32_BE32(d)}; \
			msgpack_pack_append_buffer(x, buf, 5); \
		} \
	} \
} while(0)

#define msgpack_pack_real_uint64(x, d) \
do { \
	if(d < (1ULL<<8)) { \
		if(d < (1<<7)) { \
			/* fixnum */ \
			msgpack_pack_append_buffer(x, &STORE64_BE8(d), 1); \
		} else { \
			/* unsigned 8 */ \
			const unsigned char buf[2] = {0xcc, STORE64_BE8(d)}; \
			msgpack_pack_append_buffer(x, buf, 2); \
		} \
	} else { \
		if(d < (1ULL<<16)) { \
			/* signed 16 */ \
			const unsigned char buf[3] = {0xcd, STORE64_BE16(d)}; \
			msgpack_pack_append_buffer(x, buf, 3); \
		} else if(d < (1ULL<<32)) { \
			/* signed 32 */ \
			const unsigned char buf[5] = {0xce, STORE64_BE32(d)}; \
			msgpack_pack_append_buffer(x, buf, 5); \
		} else { \
			/* signed 64 */ \
			const unsigned char buf[9] = {0xcf, STORE64_BE64(d)}; \
			msgpack_pack_append_buffer(x, buf, 9); \
		} \
	} \
} while(0)

#define msgpack_pack_real_int8(x, d) \
do { \
	if(d < -(1<<5)) { \
		/* signed 8 */ \
		const unsigned char buf[2] = {0xd0, STORE8_BE8(d)}; \
		msgpack_pack_append_buffer(x, buf, 2); \
	} else { \
		/* fixnum */ \
		msgpack_pack_append_buffer(x, &STORE8_BE8(d), 1); \
	} \
} while(0)

#define msgpack_pack_real_int16(x, d) \
do { \
	if(d < -(1<<5)) { \
		if(d < -(1<<7)) { \
			/* signed 16 */ \
			const unsigned char buf[3] = {0xd1, STORE16_BE16(d)}; \
			msgpack_pack_append_buffer(x, buf, 3); \
		} else { \
			/* signed 8 */ \
			const unsigned char buf[2] = {0xd0, STORE16_BE8(d)}; \
			msgpack_pack_append_buffer(x, buf, 2); \
		} \
	} else if(d < (1<<7)) { \
		/* fixnum */ \
		msgpack_pack_append_buffer(x, &STORE16_BE8(d), 1); \
	} else { \
		if(d < (1<<8)) { \
			/* unsigned 8 */ \
			const unsigned char buf[2] = {0xcc, STORE16_BE8(d)}; \
			msgpack_pack_append_buffer(x, buf, 2); \
		} else { \
			/* unsigned 16 */ \
			const unsigned char buf[3] = {0xcd, STORE16_BE16(d)}; \
			msgpack_pack_append_buffer(x, buf, 3); \
		} \
	} \
} while(0)

#define msgpack_pack_real_int32(x, d) \
do { \
	if(d < -(1<<5)) { \
		if(d < -(1<<15)) { \
			/* signed 32 */ \
			const unsigned char buf[5] = {0xd2, STORE32_BE32(d)}; \
			msgpack_pack_append_buffer(x, buf, 5); \
		} else if(d < -(1<<7)) { \
			/* signed 16 */ \
			const unsigned char buf[3] = {0xd1, STORE32_BE16(d)}; \
			msgpack_pack_append_buffer(x, buf, 3); \
		} else { \
			/* signed 8 */ \
			const unsigned char buf[2] = {0xd0, STORE32_BE8(d)}; \
			msgpack_pack_append_buffer(x, buf, 2); \
		} \
	} else if(d < (1<<7)) { \
		/* fixnum */ \
		msgpack_pack_append_buffer(x, &STORE32_BE8(d), 1); \
	} else { \
		if(d < (1<<8)) { \
			/* unsigned 8 */ \
			const unsigned char buf[2] = {0xcc, STORE32_BE8(d)}; \
			msgpack_pack_append_buffer(x, buf, 2); \
		} else if(d < (1<<16)) { \
			/* unsigned 16 */ \
			const unsigned char buf[3] = {0xcd, STORE32_BE16(d)}; \
			msgpack_pack_append_buffer(x, buf, 3); \
		} else { \
			/* unsigned 32 */ \
			const unsigned char buf[5] = {0xce, STORE32_BE32(d)}; \
			msgpack_pack_append_buffer(x, buf, 5); \
		} \
	} \
} while(0)

#define msgpack_pack_real_int64(x, d) \
do { \
	if(d < -(1LL<<5)) { \
		if(d < -(1LL<<15)) { \
			if(d < -(1LL<<31)) { \
				/* signed 64 */ \
				const unsigned char buf[9] = {0xd3, STORE64_BE64(d)}; \
				msgpack_pack_append_buffer(x, buf, 9); \
			} else { \
				/* signed 32 */ \
				const unsigned char buf[5] = {0xd2, STORE64_BE32(d)}; \
				msgpack_pack_append_buffer(x, buf, 5); \
			} \
		} else { \
			if(d < -(1<<7)) { \
				/* signed 16 */ \
				const unsigned char buf[3] = {0xd1, STORE64_BE16(d)}; \
				msgpack_pack_append_buffer(x, buf, 3); \
			} else { \
				/* signed 8 */ \
				const unsigned char buf[2] = {0xd0, STORE64_BE8(d)}; \
				msgpack_pack_append_buffer(x, buf, 2); \
			} \
		} \
	} else if(d < (1<<7)) { \
		/* fixnum */ \
		msgpack_pack_append_buffer(x, &STORE64_BE8(d), 1); \
	} else { \
		if(d < (1LL<<16)) { \
			if(d < (1<<8)) { \
				/* unsigned 8 */ \
				const unsigned char buf[2] = {0xcc, STORE64_BE8(d)}; \
				msgpack_pack_append_buffer(x, buf, 2); \
			} else { \
				/* unsigned 16 */ \
				const unsigned char buf[3] = {0xcd, STORE64_BE16(d)}; \
				msgpack_pack_append_buffer(x, buf, 3); \
			} \
		} else { \
			if(d < (1LL<<32)) { \
				/* unsigned 32 */ \
				const unsigned char buf[5] = {0xce, STORE64_BE32(d)}; \
				msgpack_pack_append_buffer(x, buf, 5); \
			} else { \
				/* unsigned 64 */ \
				const unsigned char buf[9] = {0xcf, STORE64_BE64(d)}; \
				msgpack_pack_append_buffer(x, buf, 9); \
			} \
		} \
	} \
} while(0)


#ifdef msgpack_pack_inline_func_fastint

msgpack_pack_inline_func_fastint(_uint8)(msgpack_pack_user x, uint8_t d)
{
	const unsigned char buf[2] = {0xcc, STORE8_BE8(d)};
	msgpack_pack_append_buffer(x, buf, 2);
}

msgpack_pack_inline_func_fastint(_uint16)(msgpack_pack_user x, uint16_t d)
{
	const unsigned char buf[3] = {0xcd, STORE16_BE16(d)};
	msgpack_pack_append_buffer(x, buf, 3);
}

msgpack_pack_inline_func_fastint(_uint32)(msgpack_pack_user x, uint32_t d)
{
	const unsigned char buf[5] = {0xce, STORE32_BE32(d)};
	msgpack_pack_append_buffer(x, buf, 5);
}

msgpack_pack_inline_func_fastint(_uint64)(msgpack_pack_user x, uint64_t d)
{
	const unsigned char buf[9] = {0xcf, STORE64_BE64(d)};
	msgpack_pack_append_buffer(x, buf, 9);
}

msgpack_pack_inline_func_fastint(_int8)(msgpack_pack_user x, int8_t d)
{
	const unsigned char buf[2] = {0xd0, STORE8_BE8(d)};
	msgpack_pack_append_buffer(x, buf, 2);
}

msgpack_pack_inline_func_fastint(_int16)(msgpack_pack_user x, int16_t d)
{
	const unsigned char buf[3] = {0xd1, STORE16_BE16(d)};
	msgpack_pack_append_buffer(x, buf, 3);
}

msgpack_pack_inline_func_fastint(_int32)(msgpack_pack_user x, int32_t d)
{
	const unsigned char buf[5] = {0xd2, STORE32_BE32(d)};
	msgpack_pack_append_buffer(x, buf, 5);
}

msgpack_pack_inline_func_fastint(_int64)(msgpack_pack_user x, int64_t d)
{
	const unsigned char buf[9] = {0xd3, STORE64_BE64(d)};
	msgpack_pack_append_buffer(x, buf, 9);
}

#undef msgpack_pack_inline_func_fastint
#endif


msgpack_pack_inline_func(_uint8)(msgpack_pack_user x, uint8_t d)
{
	msgpack_pack_real_uint8(x, d);
}

msgpack_pack_inline_func(_uint16)(msgpack_pack_user x, uint16_t d)
{
	msgpack_pack_real_uint16(x, d);
}

msgpack_pack_inline_func(_uint32)(msgpack_pack_user x, uint32_t d)
{
	msgpack_pack_real_uint32(x, d);
}

msgpack_pack_inline_func(_uint64)(msgpack_pack_user x, uint64_t d)
{
	msgpack_pack_real_uint64(x, d);
}

msgpack_pack_inline_func(_int8)(msgpack_pack_user x, int8_t d)
{
	msgpack_pack_real_int8(x, d);
}

msgpack_pack_inline_func(_int16)(msgpack_pack_user x, int16_t d)
{
	msgpack_pack_real_int16(x, d);
}

msgpack_pack_inline_func(_int32)(msgpack_pack_user x, int32_t d)
{
	msgpack_pack_real_int32(x, d);
}

msgpack_pack_inline_func(_int64)(msgpack_pack_user x, int64_t d)
{
	msgpack_pack_real_int64(x, d);
}


#ifdef msgpack_pack_inline_func_cint

msgpack_pack_inline_func_cint(_short)(msgpack_pack_user x, short d)
{
#if defined(SIZEOF_SHORT) || defined(SHRT_MAX)
#if SIZEOF_SHORT == 2 || SHRT_MAX == 0x7fff
	msgpack_pack_real_int16(x, d);
#elif SIZEOF_SHORT == 4 || SHRT_MAX == 0x7fffffff
	msgpack_pack_real_int32(x, d);
#else
	msgpack_pack_real_int64(x, d);
#endif
#else
if(sizeof(short) == 2) {
	msgpack_pack_real_int16(x, d);
} else if(sizeof(short) == 4) {
	msgpack_pack_real_int32(x, d);
} else {
	msgpack_pack_real_int64(x, d);
}
#endif
}

msgpack_pack_inline_func_cint(_int)(msgpack_pack_user x, int d)
{
#if defined(SIZEOF_INT) || defined(INT_MAX)
#if SIZEOF_INT == 2 || INT_MAX == 0x7fff
	msgpack_pack_real_int16(x, d);
#elif SIZEOF_INT == 4 || INT_MAX == 0x7fffffff
	msgpack_pack_real_int32(x, d);
#else
	msgpack_pack_real_int64(x, d);
#endif
#else
if(sizeof(int) == 2) {
	msgpack_pack_real_int16(x, d);
} else if(sizeof(int) == 4) {
	msgpack_pack_real_int32(x, d);
} else {
	msgpack_pack_real_int64(x, d);
}
#endif
}

msgpack_pack_inline_func_cint(_long)(msgpack_pack_user x, long d)
{
#if defined(SIZEOF_LONG) || defined(LONG_MAX)
#if SIZEOF_LONG == 2 || LONG_MAX == 0x7fffL
	msgpack_pack_real_int16(x, d);
#elif SIZEOF_LONG == 4 || LONG_MAX == 0x7fffffffL
	msgpack_pack_real_int32(x, d);
#else
	msgpack_pack_real_int64(x, d);
#endif
#else
if(sizeof(long) == 2) {
	msgpack_pack_real_int16(x, d);
} else if(sizeof(long) == 4) {
	msgpack_pack_real_int32(x, d);
} else {
	msgpack_pack_real_int64(x, d);
}
#endif
}

msgpack_pack_inline_func_cint(_long_long)(msgpack_pack_user x, long long d)
{
#if defined(SIZEOF_LONG_LONG) || defined(LLONG_MAX)
#if SIZEOF_LONG_LONG == 2 || LLONG_MAX == 0x7fffL
	msgpack_pack_real_int16(x, d);
#elif SIZEOF_LONG_LONG == 4 || LLONG_MAX == 0x7fffffffL
	msgpack_pack_real_int32(x, d);
#else
	msgpack_pack_real_int64(x, d);
#endif
#else
if(sizeof(long long) == 2) {
	msgpack_pack_real_int16(x, d);
} else if(sizeof(long long) == 4) {
	msgpack_pack_real_int32(x, d);
} else {
	msgpack_pack_real_int64(x, d);
}
#endif
}

msgpack_pack_inline_func_cint(_unsigned_short)(msgpack_pack_user x, unsigned short d)
{
#if defined(SIZEOF_SHORT) || defined(USHRT_MAX)
#if SIZEOF_SHORT == 2 || USHRT_MAX == 0xffffU
	msgpack_pack_real_uint16(x, d);
#elif SIZEOF_SHORT == 4 || USHRT_MAX == 0xffffffffU
	msgpack_pack_real_uint32(x, d);
#else
	msgpack_pack_real_uint64(x, d);
#endif
#else
if(sizeof(unsigned short) == 2) {
	msgpack_pack_real_uint16(x, d);
} else if(sizeof(unsigned short) == 4) {
	msgpack_pack_real_uint32(x, d);
} else {
	msgpack_pack_real_uint64(x, d);
}
#endif
}

msgpack_pack_inline_func_cint(_unsigned_int)(msgpack_pack_user x, unsigned int d)
{
#if defined(SIZEOF_INT) || defined(UINT_MAX)
#if SIZEOF_INT == 2 || UINT_MAX == 0xffffU
	msgpack_pack_real_uint16(x, d);
#elif SIZEOF_INT == 4 || UINT_MAX == 0xffffffffU
	msgpack_pack_real_uint32(x, d);
#else
	msgpack_pack_real_uint64(x, d);
#endif
#else
if(sizeof(unsigned int) == 2) {
	msgpack_pack_real_uint16(x, d);
} else if(sizeof(unsigned int) == 4) {
	msgpack_pack_real_uint32(x, d);
} else {
	msgpack_pack_real_uint64(x, d);
}
#endif
}

msgpack_pack_inline_func_cint(_unsigned_long)(msgpack_pack_user x, unsigned long d)
{
#if defined(SIZEOF_LONG) || defined(ULONG_MAX)
#if SIZEOF_LONG == 2 || ULONG_MAX == 0xffffUL
	msgpack_pack_real_uint16(x, d);
#elif SIZEOF_LONG == 4 || ULONG_MAX == 0xffffffffUL
	msgpack_pack_real_uint32(x, d);
#else
	msgpack_pack_real_uint64(x, d);
#endif
#else
if(sizeof(unsigned int) == 2) {
	msgpack_pack_real_uint16(x, d);
} else if(sizeof(unsigned int) == 4) {
	msgpack_pack_real_uint32(x, d);
} else {
	msgpack_pack_real_uint64(x, d);
}
#endif
}

msgpack_pack_inline_func_cint(_unsigned_long_long)(msgpack_pack_user x, unsigned long long d)
{
#if defined(SIZEOF_LONG_LONG) || defined(ULLONG_MAX)
#if SIZEOF_LONG_LONG == 2 || ULLONG_MAX == 0xffffUL
	msgpack_pack_real_uint16(x, d);
#elif SIZEOF_LONG_LONG == 4 || ULLONG_MAX == 0xffffffffUL
	msgpack_pack_real_uint32(x, d);
#else
	msgpack_pack_real_uint64(x, d);
#endif
#else
if(sizeof(unsigned long long) == 2) {
	msgpack_pack_real_uint16(x, d);
} else if(sizeof(unsigned long long) == 4) {
	msgpack_pack_real_uint32(x, d);
} else {
	msgpack_pack_real_uint64(x, d);
}
#endif
}

#undef msgpack_pack_inline_func_cint
#endif



/*
 * Float
 */

msgpack_pack_inline_func(_float)(msgpack_pack_user x, float d)
{
	union { char buf[4]; uint32_t num; } f;
	*((float*)&f.buf) = d;  // FIXME
	const unsigned char buf[5] = {0xca, STORE32_BE32(f.num)};
	msgpack_pack_append_buffer(x, buf, 5);
}

msgpack_pack_inline_func(_double)(msgpack_pack_user x, double d)
{
	union { char buf[8]; uint64_t num; } f;
	*((double*)&f.buf) = d;  // FIXME
	const unsigned char buf[9] = {0xcb, STORE64_BE64(f.num)};
	msgpack_pack_append_buffer(x, buf, 9);
}


/*
 * Nil
 */

msgpack_pack_inline_func(_nil)(msgpack_pack_user x)
{
	static const unsigned char d = 0xc0;
	msgpack_pack_append_buffer(x, &d, 1);
}


/*
 * Boolean
 */

msgpack_pack_inline_func(_true)(msgpack_pack_user x)
{
	static const unsigned char d = 0xc3;
	msgpack_pack_append_buffer(x, &d, 1);
}

msgpack_pack_inline_func(_false)(msgpack_pack_user x)
{
	static const unsigned char d = 0xc2;
	msgpack_pack_append_buffer(x, &d, 1);
}


/*
 * Array
 */

msgpack_pack_inline_func(_array)(msgpack_pack_user x, unsigned int n)
{
	if(n < 16) {
		unsigned char d = 0x90 | n;
		msgpack_pack_append_buffer(x, &d, 1);
	} else if(n < 65536) {
		uint16_t d = (uint16_t)n;
		unsigned char buf[3] = {0xdc, STORE16_BE16(d)};
		msgpack_pack_append_buffer(x, buf, 3);
	} else {
		uint32_t d = (uint32_t)n;
		unsigned char buf[5] = {0xdd, STORE32_BE32(d)};
		msgpack_pack_append_buffer(x, buf, 5);
	}
}


/*
 * Map
 */

msgpack_pack_inline_func(_map)(msgpack_pack_user x, unsigned int n)
{
	if(n < 16) {
		unsigned char d = 0x80 | n;
		msgpack_pack_append_buffer(x, &STORE8_BE8(d), 1);
	} else if(n < 65536) {
		uint16_t d = (uint16_t)n;
		unsigned char buf[3] = {0xde, STORE16_BE16(d)};
		msgpack_pack_append_buffer(x, buf, 3);
	} else {
		uint32_t d = (uint32_t)n;
		unsigned char buf[5] = {0xdf, STORE32_BE32(d)};
		msgpack_pack_append_buffer(x, buf, 5);
	}
}


/*
 * Raw
 */

msgpack_pack_inline_func(_raw)(msgpack_pack_user x, size_t l)
{
	if(l < 32) {
		unsigned char d = 0xa0 | l;
		msgpack_pack_append_buffer(x, &STORE8_BE8(d), 1);
	} else if(l < 65536) {
		uint16_t d = (uint16_t)l;
		unsigned char buf[3] = {0xda, STORE16_BE16(d)};
		msgpack_pack_append_buffer(x, buf, 3);
	} else {
		uint32_t d = (uint32_t)l;
		unsigned char buf[5] = {0xdb, STORE32_BE32(d)};
		msgpack_pack_append_buffer(x, buf, 5);
	}
}

msgpack_pack_inline_func(_raw_body)(msgpack_pack_user x, const void* b, size_t l)
{
	msgpack_pack_append_buffer(x, (const unsigned char*)b, l);
}

#undef msgpack_pack_inline_func
#undef msgpack_pack_user
#undef msgpack_pack_append_buffer

#undef STORE8_BE8

#undef STORE16_BE8
#undef STORE16_BE16

#undef STORE32_BE8
#undef STORE32_BE16
#undef STORE32_BE32

#undef STORE64_BE8
#undef STORE64_BE16
#undef STORE64_BE32
#undef STORE64_BE64

#undef msgpack_pack_real_uint8
#undef msgpack_pack_real_uint16
#undef msgpack_pack_real_uint32
#undef msgpack_pack_real_uint64
#undef msgpack_pack_real_int8
#undef msgpack_pack_real_int16
#undef msgpack_pack_real_int32
#undef msgpack_pack_real_int64

