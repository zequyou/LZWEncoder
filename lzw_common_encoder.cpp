//
// Created by developer on 11/13/19.
//

#include <unordered_map>
#include <cstdint>
#include <string>

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
