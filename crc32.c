#include <stdio.h>
#include <stdlib.h>
#define QUOTIENT 0x04c11db7

typedef unsigned int uint32_t;

unsigned int crctab[256];

void crc32_init(void)
{
    int i, j;

    unsigned int crc;

    for (i = 0; i < 256; i++) {
        crc = i << 24;
        for (j = 0; j < 8; j++) {
            if (crc & 0x80000000)
                crc = (crc << 1) ^ QUOTIENT;
            else
                crc = crc << 1;
        }
        crctab[i] = crc;
    }
}

void hexdump(unsigned int *tab, int len)
{
    int j = 0;
    while (len--) {
        printf ("0x%8.8x ", *tab++);
        j++;
        if (j == 16) {
            printf("\n");
            j=0;
        }
    }
}

unsigned int wombat1(unsigned char *data, int len)
{
    unsigned int        result;
    int                 i,j;
    unsigned char       octet;
    
    result = -1;
    
    for (i=0; i<len; i++) {
        octet = *(data++);
        for (j=0; j<8; j++) {
            if ((octet >> 7) ^ (result >> 31)) {
                result = (result << 1) ^ QUOTIENT;
            } else {
                result = (result << 1);
            }
            octet <<= 1;
        }
    }
    
    return ~result;             /* The complement of the remainder */
}

unsigned int wombat2(unsigned char *data, int len)
{
    unsigned int        result;
    int                 i,j;
    unsigned char       octet;
    
    if (len < 4) abort();
    printf("wonbat2\n");

    result = *data++ << 24;
    result |= *data++ << 16;
    result |= *data++ << 8;
    result |= *data++;
    result = ~ result;
    len -=4;
    
    for (i=0; i<len; i++) {
        octet = *(data++);
        for (j=0; j<8; j++) {
            if (result & 0x80000000) {
                result = (result << 1) ^ QUOTIENT ^ (octet >> 7);
            } else {
                result = (result << 1) ^ (octet >> 7);
            }
            octet <<= 1;
        }
    }
    
    return ~result;             /* The complement of the remainder */
}

unsigned int wombat3(unsigned char *data, int len)
{
    unsigned int        result;
    int                 i;
    unsigned char       octet;
    
    if (len < 4) abort();
    printf("wonbat3\n");
    result = *data++ << 24;
    result |= *data++ << 16;
    result |= *data++ << 8;
    result |= *data++;
    result = ~ result;
    len -=4;
    
    for (i=0; i<len; i++) {
        result = (result << 8 | *data++) ^ crctab[result >> 24];
    }
    
    return ~result;
}

/* This is the algorithm used by the Posix `cksum' utility. */
unsigned long cksum(const void *b_, size_t n)
{
    const unsigned char *b = b_;
    unsigned long s = 0;
    size_t i;

    for (i = n; i > 0; --i) {
        unsigned char c = *b++;
        s = (s << 8) ^ crctab[(s >> 24) ^ c];
    }
    while (n != 0) {
        unsigned char c = n;
        n >>= 8;
        s = (s << 8) ^ crctab[(s >> 24) ^ c];
    }
    return ~s;
}

unsigned char test[] = {0,0,0,0,0,0,0,0,0,0};

int main(void)
{
    unsigned char buf[10000];
    unsigned int n, crc;

    printf("Init Table\n");
    crc32_init();
    //hexdump(crctab, 256);

    n = sizeof(test);
    crc = wombat1(test, n);
    printf("N=%d CRC32=0x%x8.8 = %ld\n", n, crc, (unsigned long)crc);
    crc = wombat2(test, n);
    printf("N=%d CRC32=0x%8.8x = %ld\n", n, crc, (unsigned long)crc);
    crc = wombat3(test, n);
    printf("N=%d CRC32=0x%8.8x = %ld\n", n, crc, (unsigned long)crc);

    printf("Wait on stdin\n");
    n = fread(buf, 1, sizeof buf, stdin);
    crc = cksum(buf,n);
    printf("n=%d (0x%8.8x)\n", n, crc);
    return 1;
}
