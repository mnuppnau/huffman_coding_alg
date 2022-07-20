// C++ program for Huffman Coding
#include <iostream>
#include <cstdlib>
#include <stack>
#include <bitset>
#include <string>
#include "minheap.h"

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 1000

struct minheap::Node {
    unsigned char data;
    struct minheap::Node *left, *right;
};

// A Huffman tree node
struct minheap::MinHeapNode {

    // One of the input characters
    unsigned char data;

    // Frequency of the character
    unsigned freq;

    // Left and right child of this node
    struct minheap::MinHeapNode *left, *right;
};

// A Min Heap: Collection of
// min-heap (or Huffman tree) nodes
struct minheap::MinHeap {

    // Current size of min heap
    unsigned size;

    // capacity of min heap
    unsigned capacity;

    // Array of minheap node pointers
    struct minheap::MinHeapNode** array;
};

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct minheap::MinHeapNode* minheap::newNode(unsigned char data, unsigned freq)
{
    struct minheap::MinHeapNode* temp = (struct minheap::MinHeapNode*)malloc(sizeof(struct minheap::MinHeapNode));

    temp->left = 
    temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

// A utility function to create
// a min heap of given capacity
struct minheap::MinHeap* minheap::createMinHeap(unsigned capacity)

{

    struct minheap::MinHeap* minHeap
        = (struct minheap::MinHeap*)malloc(sizeof(struct minheap::MinHeap));

    // current size is 0
    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array
        = (struct minheap::MinHeapNode**)malloc(minHeap->
capacity * sizeof(struct minheap::MinHeapNode*));
    return minHeap;
}

// A utility function to
// swap two min heap nodes
void minheap::swapMinHeapNode(struct minheap::MinHeapNode** a,
                    struct minheap::MinHeapNode** b)

{

    struct minheap::MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void minheap::minHeapify(struct minheap::MinHeap* minHeap, int idx)

{

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->
freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->
freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        minheap::swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minheap::minHeapify(minHeap, smallest);
    }
}

// A utility function to check
// if size of heap is 1 or not
int minheap::isSizeOne(struct minheap::MinHeap* minHeap)
{

    return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
struct minheap::MinHeapNode* minheap::extractMin(struct minheap::MinHeap* minHeap)

{

    struct minheap::MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]
        = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minheap::minHeapify(minHeap, 0);

    return temp;
}

// A utility function to insert
// a new node to Min Heap
void minheap::insertMinHeap(struct minheap::MinHeap* minHeap,
                struct minheap::MinHeapNode* minHeapNode)

{

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void minheap::buildMinHeap(struct minheap::MinHeap* minHeap)

{

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minheap::minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void minheap::printArr(int arr[], int n, std::vector<std::string>& code_map, std::vector<int>& binary_stack)
{
    int i;
    std::string binary_code;
    for (i = 0; i < n; ++i)
        // std::wcout << "n : " << n << std::endl;
        binary_stack.push_back(arr[i]);
        //std::wcout<< arr[i];

    //binary_stack.push(arr[i]);
    int z = binary_stack.size();
    for ( i = 0; i < z; ++i ) {
        if ( binary_code.empty() ) {
            binary_code = std::to_string(binary_stack[i]);
        } else {
            binary_code = binary_code + std::to_string(binary_stack[i]);
        }
    }
    //std::cout << "binary code in printArr : " << binary_code << std::endl;
    //int binary_codes = std::stoi(binary_code);
    code_map.push_back(binary_code);
    binary_stack.clear();
    //std::wcout<<"\n";
}

// Utility function to check if this node is leaf
int minheap::isLeaf(struct minheap::MinHeapNode* root)

{
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct minheap::MinHeap* minheap::createAndBuildMinHeap(std::vector<unsigned char>& data, std::vector<int> freq, int size)

{

    struct MinHeap* minHeap = minheap::createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = minheap::newNode(data[i], freq[i]);

    minHeap->size = size;
    minheap::buildMinHeap(minHeap);

    return minHeap;
}

// The main function that builds Huffman tree
struct minheap::MinHeapNode* minheap::buildHuffmanTree(std::vector<unsigned char>& data, std::vector<int> freq, int size)

{
    struct minheap::MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity
    // equal to size. Initially, there are
    // modes equal to size.
    struct minheap::MinHeap* minHeap = minheap::createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!minheap::isSizeOne(minHeap)) {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = minheap::extractMin(minHeap);
        right = minheap::extractMin(minHeap);

        // Step 3: Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = minheap::newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        minheap::insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return minheap::extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void minheap::printCodes(struct minheap::MinHeapNode* root, int arr[], int top, std::vector<unsigned char>& char_map, std::vector<std::string>& code_map, std::vector<int>& binary_stack)

{

    // Assign 0 to left edge and recur
    if (root->left) {

        arr[top] = 0;
        minheap::printCodes(root->left, arr, top + 1, char_map, code_map, binary_stack);
    }

    // Assign 1 to right edge and recur
    if (root->right) {

        //encoded_tree = encoded_tree + "0";

        arr[top] = 1;
        minheap::printCodes(root->right, arr, top + 1, char_map, code_map, binary_stack);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (minheap::isLeaf(root)) {

        char_map.push_back(root->data);
        //encoded_tree = encoded_tree.append(1,static_cast<char>(root->data));
        //std::wcout<< root->data <<": ";
        minheap::printArr(arr, top, code_map, binary_stack);
    }
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void minheap::HuffmanCodes(std::vector<unsigned char>& data, std::vector<int> freq, int size, std::vector<unsigned char>& char_map, std::vector<std::string>& code_map, std::vector<int>& binary_stack)

{
    // Construct Huffman Tree
    struct MinHeapNode* root = minheap::buildHuffmanTree(data, freq, size);

    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;

    minheap::printCodes(root, arr, top, char_map, code_map, binary_stack);
}
