This program is being ran on Ubuntu 18.04.5 LTS

- Download a file that you want to compress to this project directory
- Copy your file with "cp YOUR_FILE_NAME sample_binary"
- To create the object files and program file, run "make" at project directory. This will create a program file named "main"
- The "main" program file accepts the following arguments: frequency_stats, table, encode, decode, compress, and decompress
- The program should be ran in the following orders: frequency_stats -> table -> encode -> decode, compress -> decompress
- To run the program, run "./main INSERT_ARGUMENT" at the project directory
- The program will output data to different files depending on the argument passed. Each of the arguments will output the following files:
    - "./main frequency_stats" will output two files, "output_frequencies" and "output_sorted"
    - "./main table" will output "value_lookup" and "key_lookup"
    - "./main encode" will output "input_binary_code.txt"
    - "./main decode" will output "decoded_file"
    - "./main compress" will output the compressed file "output.bin"
    - "./main decompress" will output the decompressed file "decoded_file"

    - "output_frequencies" will contain the frequency stats of each byte read from the file
    - "output_sorted" will contain the bytes read from the file and related to the frequencies
    - "value_lookup" will contain the Huffman codes assigned to each byte
    - "key_lookup"  will contain the bytes related to each of the Huffman codes
    - "input_binary_code.txt" will contain the Huffman codes in order of the input file
    - "decoded_file" will be the same file as the input file "sample_binary"

