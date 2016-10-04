#ifndef __HELPERS_H__
#define __HELPERS_H__

#define size_t unsigned

#define bit(b) (1 << (b))
#define get_bit(n, b) ((n) & bit(b))
#define set_bit(n, b) ((b) | bit(b))
#define xor_bit(n, b) ((b) ^ bit(b))
#define del_bit(n, b) ((b) & (~bit(b)))

// FIXME something is clearly wrong!
#define BYTE0(n) ((n)  & 0x00000000000000ffull)
#define BYTE1(n) ((n)  & 0x000000000000ff00ull)
#define BYTE2(n) ((n)  & 0x0000000000ff0000ull)
#define BYTE3(n) ((n)  & 0x00000000ff000000ull)
#define BYTE4(n) ((n)  & 0x000000ff00000000ull)
#define BYTE5(n) ((n)  & 0x0000ff0000000000ull)
#define BYTE6(n) ((n)  & 0x00ff000000000000ull)
#define BYTE7(n) ((n)  & 0xff00000000000000ull)

#define WORD0(n) ((n)  & 0x000000000000ffffull)
#define WORD1(n) ((n)  & 0x00000000ffff0000ull)
#define WORD2(n) ((n)  & 0x0000ffff00000000ull)
#define WORD3(n) ((n)  & 0xffff000000000000ull)

#define DWORD0(n) ((n) & 0x00000000ffffffffull)
#define DWORD1(n) ((n) & 0xffffffff00000000ull)


#endif /*__HELPERS_H__*/
