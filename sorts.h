#ifndef SORTS_H
#define SORTS_H

#include <string>
#include <vector>

extern unsigned long long g_char_comparisons;

void reset_comparisons();
unsigned long long get_comparisons();

int compare_strings(const std::string &a, const std::string &b);

void quickSort(std::vector<std::string> &arr, int left, int right);
void mergeSort(std::vector<std::string> &arr, size_t left, size_t right);

std::vector<std::string> TernaryQuickSort(std::vector<std::string> R, int L);
std::vector<std::string> StringMergeSort(std::vector<std::string> R);
std::vector<std::string> MSDRadixSort(std::vector<std::string> R, int L);
std::vector<std::string> MSBRadixSort(std::vector<std::string> R, int L, int m);

#endif
