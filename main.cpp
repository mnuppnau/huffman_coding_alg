#include <numeric>
#include <iomanip>
#include <cmath>
#include <set>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <ios>
#include <codecvt>
#include <bitset>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <boost/serialization/map.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/serialization.hpp>
#include "mergesort.h"
#include "minheap.h"

std::vector<unsigned char> sort_file(std::vector<unsigned char>& input_arr) {
    std::vector<unsigned char> arr;

    std::ifstream file("sample_binary", std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    arr.reserve(fileSize);
    std::copy(std::istream_iterator<unsigned char>(file), std::istream_iterator<unsigned char>(), std::back_inserter(arr));
    file.close();
    std::ifstream file2("sample_binary", std::ios::binary);
    file2.unsetf(std::ios::skipws);


    std::streampos fileSize2;

    file2.seekg(0, std::ios::end);
    fileSize2 = file2.tellg();
    file2.seekg(0, std::ios::beg);

    input_arr.reserve(fileSize2);

    std::copy(std::istream_iterator<unsigned char>(file2), std::istream_iterator<unsigned char>(), std::back_inserter(input_arr));

    file2.close();
    
    int n = arr.size();
    
    mergesort::merge_sort(arr, 0, n-1);

    return arr;
};

std::vector<unsigned char> get_freq(std::vector<int>& freq, std::vector<unsigned char>& arr) {

    std::vector<unsigned char> distinct_arr;
    distinct_arr.push_back(arr[0]);
    int freq_counter = 1;
    for ( int i = 1; i < arr.size(); ++i ) {
        if ( arr[i] != arr[i-1]) {
            distinct_arr.push_back(arr[i]);
            freq.push_back(freq_counter);
            
            freq_counter = 1;
        } else {
        freq_counter = freq_counter + 1;
        }
    }
    freq.push_back(freq_counter);

    return distinct_arr;
};

void saveFile (std::string name, std::string contents) {
    std::ofstream file(name.c_str(), std::ios::binary);
    for ( std::size_t byte = 0; byte < contents.size(); byte  += 8 ) {
        file.put( std::bitset< 8 >( contents, byte, std::min( (std::size_t) 8, contents.size() - byte )).to_ulong());
    }
};


int main(int argc, char** argv) {
    if ( argc != 2 ) {
        std::cout << "Enter only one of the following arguments: frequency_stats, table, encode, decode, compress, or decompress" << std::endl;
        return 0;
    } if ( std::strcmp(argv[1],"frequency_stats")==0 ) {
        std::vector<unsigned char> arr;
        std::vector<unsigned char> input_arr;
        arr = sort_file(input_arr);

        std::vector<int>  freq;
        std::vector<unsigned char> sorted_arr;
        sorted_arr = get_freq(freq, arr);

        int n = freq.size();
        int m = sorted_arr.size();
        int k = input_arr.size();

        std::map<int, unsigned char> freq_char_map;
        mergesort::merge_sort_freq(sorted_arr,freq, 0, m-1);

        freopen( "output_freq" , "w", stdout );
        for ( int i = 0; i < n; ++i ) {
           std::cout << freq[i] << std::endl;
        }
        
        std::ofstream outfile("output_sorted", std::ofstream::binary);
        outfile.write((char *)&sorted_arr[0], sorted_arr.size());

    } else if ( std::strcmp(argv[1],"table")==0 ) {
        std::vector<unsigned char> sorted_arr;
        std::vector<int> freq;

        std::ifstream file("output_sorted", std::ios::binary);
        file.unsetf(std::ios::skipws);

        std::streampos fileSize;

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        sorted_arr.reserve(fileSize);

        std::copy(std::istream_iterator<unsigned char>(file), std::istream_iterator<unsigned char>(),std::back_inserter(sorted_arr));
        std::ifstream inputFile("output_freq");

        if ( inputFile ) {
            int value;

            while ( inputFile >> value ) {
                freq.push_back(value);
            }
        }
        inputFile.close();
 
        std::vector<unsigned char> char_map;
        std::vector<std::string> code_map;
        std::vector<int> binary_stack;

        int size = sorted_arr.size();

        minheap::HuffmanCodes(sorted_arr, freq, size, char_map, code_map, binary_stack);

        freopen( "value_lookup" , "w", stdout );
        for ( int i = 0; i < code_map.size(); ++i ) {
           std::cout << code_map[i] << std::endl;
        }
        
        std::ofstream outfile("key_lookup", std::ofstream::binary);
        outfile.write((char *)&char_map[0], char_map.size());

    } else if ( std::strcmp(argv[1],"encode")==0 ) {
        std::vector<unsigned char> arr;

        std::ifstream file("sample_binary", std::ofstream::binary);
        file.unsetf(std::ios::skipws);

        std::streampos fileSize;

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        arr.reserve(fileSize);

        std::copy(std::istream_iterator<unsigned char>(file), std::istream_iterator<unsigned char>(), std::back_inserter(arr));

        std::vector<std::string> values;
        std::vector<unsigned char> keys;
        std::map<unsigned char, std::string> code_lookup;
        
        std::ifstream value_in("value_lookup");
        if ( value_in ) {
            std::string binary_encoding;

            while ( value_in >> binary_encoding ) {
                values.push_back(binary_encoding);
            }
        }
        value_in.close();

        std::ifstream key_in("key_lookup", std::ifstream::binary);
        key_in.unsetf(std::ios::skipws);
        std::streampos keySize;
        key_in.seekg(0, std::ios::end);
        keySize = key_in.tellg();
        key_in.seekg(0, std::ios::beg);

        keys.reserve(keySize);
        std::copy(std::istream_iterator<unsigned char>(key_in), std::istream_iterator<unsigned char>(), std::back_inserter(keys));
        
        for ( int i = 0; i < keys.size(); ++i ) {
            code_lookup[keys[i]] = values[i];
        
        }

        key_in.close();

        std::vector<std::string> input_code_map;
        std::string binary_code;

        int k = arr.size();

        for ( int i = 0; i < k; ++i ) {
            binary_code = code_lookup.find(arr[i])->second;
            input_code_map.push_back(binary_code);
            binary_code.clear();
        }

        freopen( "input_binary_code.txt", "w", stdout );
        for ( int i = 0; i < k; ++i ) {
           std::cout << input_code_map[i];
        }


    } else if ( std::strcmp(argv[1],"decode")==0 ) {

        std::vector<std::string> values;
        std::vector<unsigned char> keys;
        std::vector<unsigned char> decoded_bytes;
        std::map<std::string,unsigned char> code_lookup;
        
        std::string binary_encoding;
        int string_size;
        int binary_encoding_size;
        std::set<int> string_size_set;

        std::ifstream value_in("value_lookup");
        if ( value_in ) {
            int ss = 0;
            while ( value_in >> binary_encoding ) {
                values.push_back(binary_encoding);
                binary_encoding_size = binary_encoding.size();
                if ( ss == 0 ) {
                    string_size = binary_encoding_size;
                    string_size_set.insert(string_size);
                }
                string_size = std::min(string_size, binary_encoding_size);
                string_size_set.insert(string_size);
                ss = ss + 1;
            }
        }
        value_in.close();

        std::ifstream key_in("key_lookup", std::ifstream::binary);
        key_in.unsetf(std::ios::skipws);
        std::streampos keySize;
        key_in.seekg(0, std::ios::end);
        keySize = key_in.tellg();
        key_in.seekg(0, std::ios::beg);

        keys.reserve(keySize);
        std::copy(std::istream_iterator<unsigned char>(key_in), std::istream_iterator<unsigned char>(), std::back_inserter(keys));

        for ( int i = 0; i < keys.size(); ++i ) {
            code_lookup[values[i]] = keys[i];
        }

        key_in.close();

        std::ifstream t("input_binary_code.txt");
        std::stringstream buffer;
        buffer << t.rdbuf();

        std::string encoded_string;

        std::string input_string = buffer.str();

        for ( int i = 0; i < input_string.size(); ++i ) {
            encoded_string += input_string[i];
            if ( encoded_string.size() < string_size ) {
            } else {
                if ( code_lookup.count(encoded_string) == 1 ) {
                    decoded_bytes.push_back(code_lookup.at(encoded_string));
                    encoded_string.clear();
                }
            }
        }

        std::ofstream outfile("decoded_file", std::ofstream::binary);
        outfile.write((char *)&decoded_bytes[0], decoded_bytes.size());

    } else if ( std::strcmp(argv[1],"compress") == 0 ) {
        std::vector<unsigned char> arr;
        std::vector<unsigned char> input_arr;
        arr = sort_file(input_arr);

        std::vector<int>  freq;
        std::vector<unsigned char> sorted_arr;
        sorted_arr = get_freq(freq, arr);

        int n = freq.size();
        int m = sorted_arr.size();
        int k = input_arr.size();

        mergesort::merge_sort_freq(sorted_arr,freq, 0, m-1);

        std::vector<unsigned char> char_map;
        std::vector<std::string> code_map;
        std::vector<int> binary_stack;

        int size = sorted_arr.size();

        minheap::HuffmanCodes(sorted_arr, freq, size, char_map, code_map, binary_stack);

        std::string encoded_tree_binary;
        size_t number_of_codes = char_map.size();
        std::vector<int> code_sizes;

        std::string pre_header;
        std::map<unsigned char, std::string> code_lookup;
        std::map<std::string, unsigned char> char_lookup;

        std::vector<std::string> new_code_map;
        for ( const auto &s: code_map ) {
            new_code_map.push_back(s);
        }

        for ( int i = 0; i < m; ++i ) {
            code_sizes.push_back(code_map[i].length());
            code_lookup[char_map[i]] = code_map[i];
            char_lookup[code_map[i]] = char_map[i];
        }

        // Canonical Mapping
        // Move map into vector of pairs to sort based on huffman code length then lexicographical order of bytes
        std::vector<std::pair<unsigned char, std::string>> canonical_map;
        canonical_map.assign(code_lookup.begin(), code_lookup.end());

        std::sort(canonical_map.begin(), canonical_map.end(), [=](std::pair<unsigned char, std::string>& a, std::pair<unsigned char, std::string>& b) {
                return (a.second.size() != b.second.size())? a.second.size() < b.second.size() : a.first < b.first;
            }
        );

        pre_header = std::to_string(std::to_string(number_of_codes).size()) + std::to_string(number_of_codes);
       
        std::vector<int> ordered_code_sizes;
        std::vector<int> ordered_code_freq;
        std::set<size_t> code_set;


        std::sort(std::begin(code_sizes),std::end(code_sizes));
        std::map<size_t, size_t> code_size_count_map;

        for ( auto i : code_sizes ) {
            code_size_count_map[i]++;
        }
        
        for ( auto [element, count] : code_size_count_map ) {
            pre_header = pre_header + std::to_string(count) + " ";
            pre_header = pre_header + std::to_string(element) + " ";
            ordered_code_freq.push_back(count);
            ordered_code_sizes.push_back(element);
        }


        for ( int i = 0; code_sizes[i]; i++ ) {
            if ( code_size_count_map.find(code_sizes[i]) == code_size_count_map.end() ) {
                code_size_count_map[(int)code_sizes[i]] = 1;
            } else {
                code_size_count_map[(int)code_sizes[i]]++;
            }
        }

        std::string header;
      
        for ( int i = 0; i < m; ++i ) {
            header = header + canonical_map[i].second;
            header.append(1, canonical_map[i].first) + " ";
            
        }
        
        std::vector<std::string> input_code_map;
        std::string binary_code;

        for ( int i = 0; i < k; ++i ) {
            binary_code = code_lookup.find(input_arr[i])->second;
            input_code_map.push_back(binary_code);
            binary_code.clear();
        }

        std::string binary_string;
        std::vector<unsigned long> bit_arr;
        for ( const auto &piece : input_code_map ) binary_string += piece;

        std::ofstream binary_strings_out("input_binary_code.txt");
        binary_strings_out << binary_string;
        binary_strings_out.close();

        std::string binary_header;
        std::string binary_pre_header;

        for ( int i = 0; i < pre_header.size(); ++i ) {
            binary_pre_header = binary_pre_header + std::bitset<8>(pre_header[i]).to_string();
        }

        std::string temp_header = header;

        for ( int i = 0; i < ordered_code_freq.size(); ++i ) {
            for ( int j = 0; j < ordered_code_freq[i]; ++j ) {
                for ( int k = 0; k < ordered_code_sizes[i]; ++k ) {
                    binary_header = binary_header + temp_header[0];
                    temp_header.erase(0,1);
                }
                binary_header = binary_header + std::bitset<8>(temp_header[0]).to_string();
                temp_header.erase(0,1);
            }
        }

        // Turn into function
        int header_bit_remainder = binary_header.size()%8;

        if ( header_bit_remainder > 0 ) {
            std::string header_byte_padding;
            std::string header_bit_counter;

            for ( int i = 0; i < 8 - header_bit_remainder; ++i ) {
                header_byte_padding = header_byte_padding + "0";
            }

            for ( int i = 0; i < header_bit_remainder; ++i ) {
                header_bit_counter = header_bit_counter + "1";
            }

            binary_header = binary_header.substr(0,binary_header.size()-header_bit_remainder) + header_byte_padding + binary_header.substr(binary_header.size()-header_bit_remainder, header_bit_remainder);
            binary_header = binary_header + header_byte_padding + header_bit_counter;
        }

        int bit_remainder = binary_string.size()%8;

        if ( bit_remainder > 0 ) {
            std::string byte_padding;
            std::string bit_counter;

            for ( int i = 0; i < 8 - bit_remainder; ++i ) {
                byte_padding = byte_padding + "0";
            }

            for ( int i = 0; i < bit_remainder; ++i ) {
                bit_counter = bit_counter + "1";
            }

            binary_string = binary_string.substr(0,binary_string.size()-bit_remainder) + byte_padding + binary_string.substr(binary_string.size()-bit_remainder, bit_remainder);
            binary_string = binary_string + byte_padding + bit_counter;
        }

        binary_string = binary_pre_header + binary_header + binary_string;


        saveFile("output.bin", binary_string);
    } else if ( std::strcmp(argv[1],"decompress") == 0 ) {
        std::vector<unsigned char> binary_arr;
        std::ifstream binary_file("output.bin", std::ios::binary);
        binary_file.unsetf(std::ios::skipws);

        std::streampos fileSize;

        binary_file.seekg(0, std::ios::end);
        fileSize = binary_file.tellg();
        binary_file.seekg(0, std::ios::beg);

        binary_arr.reserve(fileSize);

        binary_arr.insert(binary_arr.begin(), std::istream_iterator<unsigned char>(binary_file), std::istream_iterator<unsigned char>());

        binary_file.close();


        std::string binary_string_in;

        for ( int i = 0; i < binary_arr.size(); ++i ) {
            std::bitset<8> x(binary_arr[i]);
            binary_string_in += x.to_string();
        }

        std::string number_of_unique_huffman_codes_binary;
        std::string number_of_unique_huffman_codes_str;
        std::string number_of_digits_in_unique_huffman_code_count_binary;
        char number_of_digits_in_unique_huffman_code_count;

        number_of_digits_in_unique_huffman_code_count_binary = binary_string_in.substr(0, 8);
        number_of_digits_in_unique_huffman_code_count = char(std::bitset<8>(number_of_digits_in_unique_huffman_code_count_binary).to_ulong());

        binary_string_in.erase(0,8);

        for ( int i = 0; i < number_of_digits_in_unique_huffman_code_count - '0'; ++i ) {
            number_of_unique_huffman_codes_binary = binary_string_in.substr(0,8);
            number_of_unique_huffman_codes_str = number_of_unique_huffman_codes_str + char(std::bitset<8>(number_of_unique_huffman_codes_binary).to_ulong());
            binary_string_in.erase(0,8);
        }

        int number_of_unique_huffman_codes = std::stoi(number_of_unique_huffman_codes_str);

        int unique_code_length_count = 0;
        std::vector<int> number_of_codes_v;
        std::vector<int> code_size_v;
        std::string temp_number_of_codes;
        std::string temp_size_of_codes;
        bool number_of_codes_flag = 1;

        while ( unique_code_length_count < number_of_unique_huffman_codes ) {

            if ( binary_string_in.substr(0,8) == "00100000" ) {
                binary_string_in.erase(0,8);
            } else if ( number_of_codes_flag == 0 ) {
                while ( binary_string_in.substr(0,8) != "00100000" ) {
                    temp_size_of_codes = temp_size_of_codes + char(std::bitset<8>(binary_string_in.substr(0,8)).to_ulong());
                    binary_string_in.erase(0,8);
                }
                code_size_v.push_back(std::stoi(temp_size_of_codes));
                temp_size_of_codes.clear();
                number_of_codes_flag = 1;
            } else if ( number_of_codes_flag == 1 ){
                while ( binary_string_in.substr(0,8) != "00100000" ) {
                    temp_number_of_codes = temp_number_of_codes + char(std::bitset<8>(binary_string_in.substr(0,8)).to_ulong());
                    binary_string_in.erase(0,8);
                }
                number_of_codes_v.push_back(std::stoi(temp_number_of_codes));
                unique_code_length_count = unique_code_length_count + std::stoi(temp_number_of_codes);
                temp_number_of_codes.clear();
                number_of_codes_flag = 0;
            }

        }

        if ( binary_string_in.substr(0,8) == "00100000" ) {
            binary_string_in.erase(0,8);
        } 
        if ( number_of_codes_flag == 0 ) {
            while ( binary_string_in.substr(0,8) != "00100000" ) {
                temp_size_of_codes = temp_size_of_codes + char(std::bitset<8>(binary_string_in.substr(0,8)).to_ulong());
                binary_string_in.erase(0,8);
            }
            code_size_v.push_back(std::stoi(temp_size_of_codes));
            temp_size_of_codes.clear();
            if ( binary_string_in.substr(0,8) == "00100000" ) {
                binary_string_in.erase(0,8);
            } 
        }

        std::vector<int> number_of_header_bits;
        const double number_of_bits = 8.0;
        const double bit = 1.0;
        for ( int i = 0; i < code_size_v.size(); ++i ) {
            number_of_header_bits.push_back(number_of_codes_v[i]*8);
            number_of_header_bits.push_back(number_of_codes_v[i]*code_size_v[i]);

        }

        int sum_of_elements = 0;
        for ( int i = 0; i < number_of_header_bits.size(); ++i ) {
            sum_of_elements = sum_of_elements + number_of_header_bits[i];
        }

        int header_bits_remainder;
        int sum_of_elements_padding;
        int sum_of_elements_trunc;
        header_bits_remainder = sum_of_elements%8;
        if ( header_bits_remainder != 0 ) {
            sum_of_elements_padding = sum_of_elements + (8-header_bits_remainder) + 8;
            sum_of_elements_trunc = sum_of_elements-(8-(8-header_bits_remainder));
            std::string padding_test_sub_str = binary_string_in.substr(sum_of_elements_trunc,16);
            bool no_padding_flag = 0;

            int zero_counter = 0;

                    for ( int i = 0; i < 8; ++i ) {
                        if ( padding_test_sub_str.substr(8+i,1) == "0") {
                            zero_counter = zero_counter + 1;
                        }
                    }

                    for ( int i = 0; i < zero_counter; ++i ) {
                        if ( padding_test_sub_str.substr(0+i,1) == "1" )
                            no_padding_flag = 1;
                    }

                    if ( no_padding_flag == 0 ) {
                        binary_string_in.erase(sum_of_elements_trunc,16);
                        binary_string_in = binary_string_in.substr(0,sum_of_elements_trunc) + padding_test_sub_str.substr(zero_counter,8-zero_counter) + binary_string_in.substr(sum_of_elements_trunc,binary_string_in.size()-sum_of_elements_trunc);
                    }
        }
        int header_bit_counter_int = 0;
        int bit_tracker;
        std::map<std::string, unsigned char> code_mapping;
        std::vector<unsigned char> char_from_binary_string_v;
        std::vector<std::string> code_from_binary_string_v;
        std::string temp_code;

        for ( int i = 0; i < number_of_codes_v.size(); ++i ) {
            for ( int k = 0; k < number_of_codes_v[i]; ++k ) {
                for ( int j = 0; j < code_size_v[i]; ++j ) {
                    temp_code = temp_code + binary_string_in.substr(0,1);
                    binary_string_in.erase(0,1);
                }
                code_from_binary_string_v.push_back(temp_code);
                temp_code.clear();
                char_from_binary_string_v.push_back(static_cast<unsigned char>(std::bitset<8>(binary_string_in.substr(0,8)).to_ulong()));
                binary_string_in.erase(0,8);
            }
        }

        for ( int i = 0; i < code_from_binary_string_v.size(); ++i ) {
            code_mapping[code_from_binary_string_v[i]] = char_from_binary_string_v[i];
        }

        std::string padding_test_sub_str = binary_string_in.substr(binary_string_in.size()-16,16);
        bool no_padding_flag = 0;

        int zero_counter = 0;
        for ( int i = 0; i < 8; ++i ) {
            if ( padding_test_sub_str.substr(8+i,1) == "0") {
                zero_counter = zero_counter + 1;
            }
        }

        for ( int i = 0; i < zero_counter; ++i ) {
            if ( padding_test_sub_str.substr(0+i,1) == "1" )
                no_padding_flag = 1;
        }

        if ( no_padding_flag == 0 ) {
            binary_string_in.erase(binary_string_in.size()-16,16);
            binary_string_in = binary_string_in + padding_test_sub_str.substr(zero_counter,8-zero_counter);
        }

        int string_size = code_size_v[0];
        
        std::string encoded_string;
        std::vector<unsigned char> decoded_bytes;

        for ( int i = 0; i < binary_string_in.size(); ++i ) {
            encoded_string += binary_string_in[i];
            if ( encoded_string.size() < string_size ) {
            } else {
                if ( code_mapping.count(encoded_string) == 1 ) {
                    decoded_bytes.push_back(code_mapping.at(encoded_string));
                    encoded_string.clear();
                } 
            }
        }

        std::ofstream outfile("decoded_file", std::ofstream::binary);
        outfile.write((char *)&decoded_bytes[0], decoded_bytes.size());

    }
    return 0;
}
