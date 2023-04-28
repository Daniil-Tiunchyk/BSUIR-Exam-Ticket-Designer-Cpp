#include <cstdint>
#include <memory.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN    1

bool endian() {
    uint32_t i = 1;
    uint8_t* p = reinterpret_cast<uint8_t*>(&i);

    if (*p == 1)
        return LITTLE_ENDIAN;
    else
        return BIG_ENDIAN;
}

void* input;
uint64_t saved_length;
uint8_t* end;

uint32_t a;
uint32_t b;
uint32_t c;
uint32_t d;
uint32_t block[16];


const uint32_t t[65] = { 0,
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};// (2^32)*sin(ind)

void append_padding_bits() {
    end = static_cast<uint8_t*>(input) + saved_length;
    *end = 0x80;
    ++end;

    while ((end - static_cast<uint8_t*>(input)) % 64 != 56)
        *end = 0x00, ++end;
}

void append_length() {
    uint64_t length = saved_length * 8;
    memcpy(static_cast<void*>(end), &length, 8);

    if (endian() == BIG_ENDIAN) {
        std::swap(*end, *(end + 7));
        std::swap(*(end + 1), *(end + 6));
        std::swap(*(end + 2), *(end + 5));
        std::swap(*(end + 3), *(end + 4));
    }

    end += 8;
}

void init_buffer() {
    uint8_t _a[4] = { 0x01, 0x23, 0x45, 0x67 };
    uint8_t _b[4] = { 0x89, 0xab, 0xcd, 0xef };
    uint8_t _c[4] = { 0xfe, 0xdc, 0xba, 0x98 };
    uint8_t _d[4] = { 0x76, 0x54, 0x32, 0x10 };

    a = *reinterpret_cast<uint32_t*>(&_a);
    b = *reinterpret_cast<uint32_t*>(&_b);
    c = *reinterpret_cast<uint32_t*>(&_c);
    d = *reinterpret_cast<uint32_t*>(&_d);
}

uint32_t rotate_left(uint32_t x, uint32_t s) { return (x << s) | (x >> (32 - s)); }

uint32_t F(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
uint32_t G(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (~z & y); }
uint32_t H(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
uint32_t I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (~z | x); }

void round(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t k, uint32_t s, uint32_t i, uint32_t(*function)(uint32_t, uint32_t, uint32_t)) {
    a += function(b, c, d) + block[k] + t[i];
    a = rotate_left(a, s);
    a += b;
}

void process_block(uint8_t* adress) {
    memcpy(&block, static_cast<void*>(adress), 64);
    uint32_t aa = a, bb = b, cc = c, dd = d;

    round(a, b, c, d, 0, 7, 1, F);
    round(d, a, b, c, 1, 12, 2, F);
    round(c, d, a, b, 2, 17, 3, F);
    round(b, c, d, a, 3, 22, 4, F);

    round(a, b, c, d, 4, 7, 5, F);
    round(d, a, b, c, 5, 12, 6, F);
    round(c, d, a, b, 6, 17, 7, F);
    round(b, c, d, a, 7, 22, 8, F);

    round(a, b, c, d, 8, 7, 9, F);
    round(d, a, b, c, 9, 12, 10, F);
    round(c, d, a, b, 10, 17, 11, F);
    round(b, c, d, a, 11, 22, 12, F);

    round(a, b, c, d, 12, 7, 13, F);
    round(d, a, b, c, 13, 12, 14, F);
    round(c, d, a, b, 14, 17, 15, F);
    round(b, c, d, a, 15, 22, 16, F);

    round(a, b, c, d, 1, 5, 17, G);
    round(d, a, b, c, 6, 9, 18, G);
    round(c, d, a, b, 11, 14, 19, G);
    round(b, c, d, a, 0, 20, 20, G);

    round(a, b, c, d, 5, 5, 21, G);
    round(d, a, b, c, 10, 9, 22, G);
    round(c, d, a, b, 15, 14, 23, G);
    round(b, c, d, a, 4, 20, 24, G);

    round(a, b, c, d, 9, 5, 25, G);
    round(d, a, b, c, 14, 9, 26, G);
    round(c, d, a, b, 3, 14, 27, G);
    round(b, c, d, a, 8, 20, 28, G);

    round(a, b, c, d, 13, 5, 29, G);
    round(d, a, b, c, 2, 9, 30, G);
    round(c, d, a, b, 7, 14, 31, G);
    round(b, c, d, a, 12, 20, 32, G);

    round(a, b, c, d, 5, 4, 33, H);
    round(d, a, b, c, 8, 11, 34, H);
    round(c, d, a, b, 11, 16, 35, H);
    round(b, c, d, a, 14, 23, 36, H);

    round(a, b, c, d, 1, 4, 37, H);
    round(d, a, b, c, 4, 11, 38, H);
    round(c, d, a, b, 7, 16, 39, H);
    round(b, c, d, a, 10, 23, 40, H);

    round(a, b, c, d, 13, 4, 41, H);
    round(d, a, b, c, 0, 11, 42, H);
    round(c, d, a, b, 3, 16, 43, H);
    round(b, c, d, a, 6, 23, 44, H);

    round(a, b, c, d, 9, 4, 45, H);
    round(d, a, b, c, 12, 11, 46, H);
    round(c, d, a, b, 15, 16, 47, H);
    round(b, c, d, a, 2, 23, 48, H);

    round(a, b, c, d, 0, 6, 49, I);
    round(d, a, b, c, 7, 10, 50, I);
    round(c, d, a, b, 14, 15, 51, I);
    round(b, c, d, a, 5, 21, 52, I);

    round(a, b, c, d, 12, 6, 53, I);
    round(d, a, b, c, 3, 10, 54, I);
    round(c, d, a, b, 10, 15, 55, I);
    round(b, c, d, a, 1, 21, 56, I);

    round(a, b, c, d, 8, 6, 57, I);
    round(d, a, b, c, 15, 10, 58, I);
    round(c, d, a, b, 6, 15, 59, I);
    round(b, c, d, a, 13, 21, 60, I);

    round(a, b, c, d, 4, 6, 61, I);
    round(d, a, b, c, 11, 10, 62, I);
    round(c, d, a, b, 2, 15, 63, I);
    round(b, c, d, a, 9, 21, 64, I);

    a += aa, b += bb, c += cc, d += dd;
}

void process() {
    uint8_t* temp = static_cast<uint8_t*>(input);

    while (temp != end)
        process_block(temp), temp += 64;
}

std::vector<uint8_t> finish() {
    std::vector<uint8_t> hash(16);
    memcpy(&hash[0], &a, 4);
    memcpy(&hash[4], &b, 4);
    memcpy(&hash[8], &c, 4);
    memcpy(&hash[12], &d, 4);
    return hash;
}

std::vector<uint8_t> MD5(void* original_input, uint64_t size) {
    uint8_t* place = new uint8_t[size + 100];
    memcpy(static_cast<void*>(place), original_input, size);
    input = static_cast<void*>(place);
    saved_length = size;
    append_padding_bits();
    append_length();
    init_buffer();
    process();
    delete[] place;
    return finish();
}

std::string md5hash_to_string(std::vector<uint8_t> hash) {
    std::string hex_char = "0123456789abcdef";
    std::string ret = "";

    for (int i = 0; i < 16; ++i) {
        ret += hex_char[hash[i] >> 4];
        ret += hex_char[hash[i] & 0x0F];
    }

    return ret;
}

std::string doHashNow(std::string str) {
    //#define test_speed

#ifndef test_speed
    std::string s;
    s = md5hash_to_string(MD5(&str[0], str.size()));
#else
    int arr[500];
    int counter = 0;
    int start = clock();
    while (clock() - start < 1000)
        MD5(&arr[counter % 500], 7), counter++;
    std::cout << counter;
#endif

    return s;
}