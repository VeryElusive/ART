#pragma once

#define NULL 0
#define FALSE 0
#define TRUE 1

typedef u8          unsigned char;
typedef i16         short;
typedef u16         unsigned short;
typedef i32         int;
typedef u32         unsigned int;
typedef i64         __int64;
typedef u64         unsigned __int64;


#ifdef ART_64BIT
typedef size_t		u64
#else
typedef size_t		u32
#else

#endif