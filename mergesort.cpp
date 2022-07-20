#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include "mergesort.h"

int mergesort::merge(std::vector<unsigned char>& arr, int const p, int const q, int const r) {
    auto const n1 = q - p + 1;
    auto const n2 = r - q;

    unsigned char sentinel = std::numeric_limits<unsigned char>::max();

    std::vector<unsigned char> arr_l;
    std::vector<unsigned char> arr_r;
    arr_l.resize(n1+1);
    arr_r.resize(n2+1);

    for ( auto i = 0; i < n1; i++ ) {
        arr_l[i] = arr[p + i];
    }

    for ( auto j = 0; j < n2; j++ ) {
        arr_r[j] = arr[q + j + 1];
    }

    arr_l[n1] = sentinel;
    arr_r[n2] = sentinel;
    auto i = 0;
    auto j = 0;
    int k = p;

    while ( i < n1 && j < n2 ) {
        if (arr_l[i] <= arr_r[j] ) {
            arr[k] = arr_l[i];
            i = i + 1;
        } else {
            arr[k] = arr_r[j];
            j = j + 1;
        }
        k++;
    }

    while ( i < n1 ) {
        arr[k] = arr_l[i];
        i++;
        k++;
    }
    while ( j < n2 ) {
        arr[k] = arr_r[j];
        j++;
        k++;
    }
    return 0;
}

void mergesort::merge_sort(std::vector<unsigned char>& arr, int p, int r) {
    if ( p < r ) {
        //std::cout << "p and r : " << p << " " << r << std::endl;
        int q = std::floor((((double)p*1.0+1.0) + (((double)r*1.0+1.0)))/2.0);
        //auto q = p + (r - p) / 2;
        //std::cout << "mergesort1 q : " << q << std::endl;
        mergesort::merge_sort(arr, p, q-1);
        //std::cout<< "mergesort2 q and r : " << q << " " << r << std::endl;
        mergesort::merge_sort(arr, q, r);
        mergesort::merge(arr, p, q-1, r);
    }
}

int mergesort::merge_freq(std::vector<unsigned char>& unq_arr, std::vector<int>& arr, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;
    // double sentinal = std::numeric_limits<double>::infinity();

    int sentinel = std::numeric_limits<int>::max();

    std::vector<int> arr_l;
    std::vector<int> arr_r;
    arr_l.resize(n1+1);
    arr_r.resize(n2+1);

    for ( int i = 0; i < n1; ++i ) {
        arr_l[i] = arr[p + i];
    }

    for ( int j = 0; j < n2; ++j ) {
        arr_r[j] = arr[q + j + 1];
    }

    arr_l[n1] = sentinel;
    arr_r[n2] = sentinel;

    int i = 0;
    int j = 0;

    std::vector<unsigned char> unq_arr_l;
    std::vector<unsigned char> unq_arr_r;
    unq_arr_l.resize(n1+1);
    unq_arr_r.resize(n2+1);

    for ( int i = 0; i < n1; ++i ) {
        unq_arr_l[i] = unq_arr[p + i];
    }

    for ( int j = 0; j < n2; ++j ) {
        unq_arr_r[j] = unq_arr[q + j + 1];
    }

    unq_arr_l[n1] = sentinel;
    unq_arr_r[n2] = sentinel;

    for ( int k = p; k < r+1; ++k ) {
        if (arr_l[i] <= arr_r[j] ) {
            arr[k] = arr_l[i];
            unq_arr[k] = unq_arr_l[i];
            i = i + 1;
        } else {
            arr[k] = arr_r[j];
            unq_arr[k] = unq_arr_r[j];
            j = j + 1;
        }
    }
    return 0;
}

void mergesort::merge_sort_freq(std::vector<unsigned char> &unq_arr, std::vector<int> &arr, int p, int r) {
    if ( p < r ) {
        int q = std::floor((((double)p+1.0) + ((double)r+1.0))/2.0);
        mergesort::merge_sort_freq(unq_arr, arr, p, q-1);
        mergesort::merge_sort_freq(unq_arr, arr, q, r);
        mergesort::merge_freq(unq_arr, arr, p, q-1, r);
    }
}
