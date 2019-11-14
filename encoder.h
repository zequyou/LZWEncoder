//
// Created by developer on 10/31/19.
//

#ifndef LZW_ENCODER_H
#define LZW_ENCODER_H

#include <string>
#include <unordered_map>
#include <cstdint>

using namespace std;

bool lzw_encode(const uint8_t *input, uint8_t *output, uint32_t input_length, uint32_t &output_length) {
    unordered_map<string, uint16_t> dictionary(8192);
    uint32_t output_index = 0;
    uint16_t dict_index = 256;
    uint8_t output_buffer = 0;
    bool half = false;

    for (int i = 0; i < 256; i++) {
        string temp(1, static_cast<unsigned char>(i));
        dictionary.insert({temp, i});
    }

    string last(1, input[0]);
    for (uint32_t i = 1; i < input_length; i++) {
        string curr(1, input[i]);  // current

        string temp = last + curr;
        if (dictionary.find(temp) != dictionary.end()) {
            last.append(curr);
        } else {
            uint16_t dict_value = dictionary.find(last)->second;
            if (half) {
                output_buffer |= (uint8_t)(dict_value >> 8u);
                output[output_index++] = output_buffer;
                output[output_index++] = (dict_value & 255u);
                output_buffer = 0;
            } else {
                output[output_index++] = (dict_value >> 4u);
                output_buffer = ((dict_value & 15u) << 4u);
            }
            half = !half;
            dictionary.insert({temp, dict_index});
            dict_index++;
            last = curr;
        }

        if (dict_index == 4096) {
            return false;
        }
    }

    uint16_t dict_value = dictionary.find(last)->second;
    if (half) {
        output_buffer |= (uint8_t)(dict_value >> 8u);
        output[output_index++] = output_buffer;
        output[output_index++] = (dict_value & 255u);
    } else {
        output_buffer = (dict_value >> 4u);
        output[output_index++] = output_buffer;
        output[output_index++] = ((dict_value & 15u) << 4u);
    }
    output_length = output_index;

    return true;
}


bool lzw_tree_encode(const uint8_t *input, uint8_t *output, uint32_t input_length, uint32_t &output_length) {
    // map and the index
    unordered_map<uint32_t, uint32_t> dictionary(4096);
    uint16_t dict_index = 256;

    // used to set output
    uint32_t output_index = 0;
    uint8_t output_buffer = 0;
    bool half = false;

    // key used to search in map
    // value used to set the value
    uint32_t key;
    uint32_t value;
    auto *tree_root = (uint8_t *)&key;
    auto *tree_node = (uint8_t *)&key + 1;
    auto *tree_layer = (uint16_t *)&key + 1;
    auto *dict_value = (uint16_t *)&value;
    auto *jump_value = (uint16_t *)&value + 1;

    *tree_root = input[0];
    *tree_layer = 0;
    uint16_t last = *tree_root;
    for (uint32_t i = 1; i < input_length; i++) {
        *tree_node = input[i];

        auto element = dictionary.find(key);
        if (element != dictionary.end()) {
            last = (element->second & 65535u);
            *tree_layer = (element->second >> 16u);
        } else {
            if (half) {
                output_buffer |= (uint8_t)(last >> 8u);
                output[output_index++] = output_buffer;
                output[output_index++] = (last & 255u);
                output_buffer = 0;
            } else {
                output[output_index++] = (last >> 4u);
                output_buffer = ((last & 15u) << 4u);
            }
            half = !half;

            // set new index in dict
            *dict_value = dict_index;
            *jump_value = i;
            dictionary[key] = value;

            // prepare for next loop
            dict_index++;
            *tree_root = *tree_node;
            *tree_layer = 0;
            last = *tree_root;
        }

        if (dict_index == 4096) {
            return false;
        }
    }

    if (half) {
        output_buffer |= (uint8_t)(last >> 8u);
        output[output_index++] = output_buffer;
        output[output_index++] = (last & 255u);
    } else {
        output_buffer = (last >> 4u);
        output[output_index++] = output_buffer;
        output[output_index++] = ((last & 15u) << 4u);
    }
    output_length = output_index;

    return true;
}

#endif //LZW_ENCODER_H
