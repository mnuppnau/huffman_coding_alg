#pragma once
// C++ program for Huffman Coding
#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <string>

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 1000

namespace minheap {
    // A Huffman tree node
    struct MinHeapNode;

    struct Node;
    // A Min Heap: Collection of
    // min-heap (or Huffman tree) nodes
    struct MinHeap;

    void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
    void minHeapify(struct MinHeap* minHeap, int idx);
    void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* mindHeapNode);
    void buildMinHeap(struct MinHeap* minHeap);
    void printArr(int arr[], int n, std::vector<std::string>& code_map, std::vector<int>& binary_stack);
    void printCodes(struct MinHeapNode* root, int arr[], int top, std::vector<unsigned char>& char_map, std::vector<std::string>& code_map, std::vector<int>& binary_stack);
    void HuffmanCodes(std::vector<unsigned char>& data, std::vector<int> freq, int size, std::vector<unsigned char>& char_map, std::vector<std::string>& code_map, std::vector<int>& binary_stack);

    int isSizeOne(struct MinHeap* minHeap);
    int isLeaf(struct MinHeapNode* root);

    struct MinHeapNode* newNode(unsigned char data, unsigned freq);
    struct MinHeap* createMinHeap(unsigned capactity);
    struct MinHeapNode* extractMin(struct MinHeap* minHeap);
    struct MinHeap* createAndBuildMinHeap(std::vector<unsigned char>& data, std::vector<int> freq, int size);
    struct MinHeapNode* buildHuffmanTree(std::vector<unsigned char>& data, std::vector<int> freq, int size);

}

#endif // !MIN_HEAP_H
