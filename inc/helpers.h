#ifndef __HELPERS_H__
#define __HELPERS_H__

#define size_t unsigned

#define bit(b) (1 << (b))
#define get_bit(n, b) ((n) & bit(b))
#define set_bit(n, b) ((b) | bit(b))
#define xor_bit(n, b) ((b) ^ bit(b))
#define del_bit(n, b) ((b) & (~bit(b)))

#define lsb0(n) ((n) & 0x00ffu)
#define lsb1(n) ((n) & 0xff00u)

#endif /*__HELPERS_H__*/
