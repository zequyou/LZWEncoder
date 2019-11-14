//
// Created by developer on 11/13/19.
//

#ifndef LZW_GOLDEN_H
#define LZW_GOLDEN_H

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

uint16_t get_input(const uint8_t input[], uint32_t index) {
    uint16_t value;
    uint32_t byte_bias;
    if (index & 1u) {
        byte_bias = index / 2 * 3 + 1;
        value = ((input[byte_bias] & 15u) << 8u);
        value |= (uint16_t)(input[byte_bias + 1]);
    } else {
        byte_bias = index / 2 * 3;
        value = (((uint16_t)input[byte_bias]) << 4u);
        value |= (uint8_t)(input[byte_bias + 1] >> 4u);
    }

    return value;
}

vector<int> encoding(string s1)
{
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += s1[0];
    int code = 256;
    vector<int> output_code;
    for (int i = 0; i < s1.length(); i++) {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        }
        else {
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    output_code.push_back(table[p]);
    return output_code;
}

#endif //LZW_GOLDEN_H
