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
typedef size_t             u64;
typedef ptr                u64;
#else
typedef size_t             u32;
typedef ptr                u32;
#endif
