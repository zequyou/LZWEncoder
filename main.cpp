#include <bits/stdc++.h>
#include "encoder.h"
#include "golden.h"

using namespace std;

int main() {
    ifstream in_stream;
    uint64_t time_counter = 0;
    uint64_t byte_counter = 0;
    uint64_t start;
    unsigned char input[5760];
    unsigned char encode_output[5760];

    // there are multiple test files
    // in_stream.open("little_prince", ios::binary);
    // in_stream.open("gtk+.tar", ios::binary);
    // in_stream.open("limit_node", ios::binary);
    in_stream.open("linux.tar", ios::binary);

    // check streams
    if (!in_stream.good()) {
        std::cerr << "Could not open input file.\n";
        return EXIT_FAILURE;
    }

    // compress
    while (true) {
        // read in string
        if (in_stream.eof() || in_stream.bad()) {
            break;
        }

        in_stream.read((char *) input, 3584 + random() % 2177);
        byte_counter += in_stream.gcount();

        // if the string length is zero, stop the compress
        if (in_stream.gcount() == 0) {
            break;
        }

        start = clock();
        uint32_t encode_length;
        bool result = lzw_tree_encode(input, encode_output, in_stream.gcount(), encode_length);
        time_counter += clock() - start;

        if (! result) {
            cout << "chunk length " << in_stream.gcount() << endl;
            continue;
        }
        vector<int> golden = encoding(string((char *)input, in_stream.gcount()));

        int index = 0;
        for (auto data: golden) {
            uint16_t my_value = get_input(encode_output, index);
            if (my_value != data) {
                cout << "data mismatch!" << endl;
                cout << "index " << index << endl;
                cout << "chunk length " << in_stream.gcount() << endl;
                cout << "golden value " << data << endl;
                cout << "my value " << my_value << endl;
                exit(1);
            }
            index++;
        }
    }

    cout << "throughput " << byte_counter / 1024.0 / 1024.0 * CLOCKS_PER_SEC / time_counter << "MB/s" << endl;

    return 0;
}