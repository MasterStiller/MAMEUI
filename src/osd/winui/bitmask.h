// For licensing and usage information, read docs/release/winui_license.txt
//****************************************************************************

#ifndef WINUI_BITMASK_H
#define WINUI_BITMASK_H

/* Bit array type */
typedef struct
{
	UINT m_nSize;
	UCHAR* m_lpBits;
} BITS, * LPBITS;

/* Bit functions */
LPBITS NewBits(UINT nLength /* in bits */);
void DeleteBits(LPBITS lpBits);
BOOL TestBit(LPBITS lpBits, UINT nBit);
void SetBit(LPBITS lpBits, UINT nBit);
void ClearBit(LPBITS lpBits, UINT nBit);
void SetAllBits(LPBITS lpBits, BOOL bSet);
int FindBit(LPBITS lpBits, int nStartPos, BOOL bSet);

/* Linked list type */
typedef struct
{
	LPVOID  data;
	LPVOID  next;
	LPVOID  prev;
} NODE, * LPNODE;

/* Linked list functions */

#endif /* BITMASK_H */

