#pragma once
#include <iostream>
#include <vector>

#ifndef MERGE_SORT_H
#define MERGE_SORT_H

namespace mergesort {
    int merge( std::vector<unsigned char>& arr, int p, int q, int r );
    void merge_sort( std::vector<unsigned char>& arr, int p, int r );
    int merge_freq( std::vector<unsigned char>& unq_arr, std::vector<int>& arr, int p, int q, int r );
    void merge_sort_freq( std::vector<unsigned char>& unq_arr, std::vector<int>& arr, int p, int r );

}

#endif // !MERGE_SORT_H
