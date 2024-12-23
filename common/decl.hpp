#pragma once

#define NULL 0
#define FALSE 0
#define TRUE 1

typedef unsigned char      u8;
typedef short              i16;
typedef unsigned short     u16;
typedef int                i32;
typedef unsigned int       u32;
typedef __int64            i64;
typedef unsigned __int64   u64;

#ifdef ART_64BIT
typedef u64                Size_t;
typedef u64                Ptr_t;
#else
typedef u32                Size_t;
typedef u32                Ptr_t;
#endif
