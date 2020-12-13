// The original MegaLZ Speccy packer, Z80 depacker and packed file
// format  (C) fyrex^mhm.
// 
// Advanced C MegaLZ packer  (C) lvd^mhm.

#ifndef MEGALZ_H
#define MEGALZ_H

//#define ERRORS
// for complete error checking in depacker()
// remove if you don't need any error checking

#define PACKMODE_OPTIMAL 1
#define PACKMODE_GREEDY  2

// size of bunches of twobyters
#define BUNCHSIZE 256

// output buffer size
#define OUBUFSIZE 256

// size and mask of output buffer
#define DBSIZE 8192
#define DBMASK 8191

// LZ code types

// just emitting 1 byte
#define OUTBYTE 1
// code of length 1, disp -1..-8
#define LEN1 2
// code of length 2, disp -1..-256
#define LEN2 3
// code of length 3, disp -1..-256
#define LEN3_SHORT 4
// code of length 3, disp -257..-4352
#define LEN3_LONG 5
// code of variable length (4..255), disp -1..-256
#define VARLEN_SHORT 6
// code of variable length (4..255), disp -257..-4352
#define VARLEN_LONG 7

// depacker return codes
//
// all OK
#define DEPACKER_OK        1
// erroneous LZ code encountered
#define DEPACKER_ERRCODE   2
// unexpected end of input stream without STOP code
#define DEPACKER_NOEOF     3
// after STOP code there are more bytes in input stream
#define DEPACKER_TOOLONG   4
// to indicate put_buffer() fail
#define DEPACKER_CANTWRITE 5

typedef        signed char  BYTE;
typedef      unsigned char UBYTE;

typedef   signed short int  WORD;
typedef unsigned short int UWORD;

typedef         signed int  LONG;
typedef       unsigned int ULONG;

//LZ code descriptor
struct lzcode
{
	UWORD length; // length of code: number of bytes generated by it in output file
	WORD disp; // backward displacement of this code (-1..-4352)
};

//LZ code detailed information
struct lzinfo
{
	ULONG length; // length of code: number of bytes generated by it in output file
	LONG  disp; // backward displacement of this code (-1..-4352)
	ULONG bitsize; // total bit length of LZ code in output file
	ULONG byte; // byte to be outted to the byte part of output stream of 0xFFFFFFFF if no byte in code
	ULONG bits; // bits to be outted to the bit part of output stream (left-aligned)
	ULONG bitsnum; // number of bits
	ULONG type; // type of LZ code (see above)
};

//chained two-byter element
struct tb_chain
{
	struct tb_chain * next; // previous entry of the same 2-byte or NULL if none
	ULONG pos; // position where 2-byte last seen - max 4352 bytes back from current!
};

//container of bunch of twobyters
struct tb_bunch
{
	struct tb_bunch * next;
	struct tb_chain bunch[BUNCHSIZE];
};

struct packinfo // contains various info for every byte of input file
{
	struct lzcode best;  // best LZ code for jumping to this pos. length=0 - OUTBYTE type
	ULONG price; // price - for finding optimal LZ codes chain
};

extern unsigned char *megalz_compress(unsigned char *input_data, size_t input_size, ULONG pack_mode, size_t *output_size);
extern void megalz_decompress(unsigned char *input_data, unsigned char *output_data);

#endif
