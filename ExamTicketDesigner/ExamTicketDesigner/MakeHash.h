#pragma once
#include <cstdint>
#include <memory.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

bool endian();

void append_padding_bits();

void append_length();


void init_buffer();

uint32_t rotate_left(uint32_t, uint32_t);

uint32_t F(uint32_t, uint32_t, uint32_t);
uint32_t G(uint32_t, uint32_t, uint32_t);
uint32_t H(uint32_t, uint32_t, uint32_t);
uint32_t I(uint32_t, uint32_t, uint32_t);

void round(uint32_t&, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t(*function)(uint32_t, uint32_t, uint32_t));

void process_block(uint8_t*);
void process();
std::vector<uint8_t> finish();
std::vector<uint8_t> MD5(void*, uint64_t);

std::string md5hash_to_string(std::vector<uint8_t>);


std::string doHashNow(std::string);
