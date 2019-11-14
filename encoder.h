//
// Created by developer on 10/31/19.
//

#ifndef LZW_ENCODER_H
#define LZW_ENCODER_H

#include <string>
#include <unordered_map>
#include <cstdint>

using namespace std;

bool lzw_encode(const uint8_t *input, uint8_t *output, uint32_t input_length, uint32_t &output_length);
bool lzw_tree_encode(const uint8_t *input, uint8_t *output, uint32_t input_length, uint32_t &output_length);

#endif //LZW_ENCODER_H
