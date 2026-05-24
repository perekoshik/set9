#include "sorts.h"

#include <algorithm>

unsigned long long g_char_comparisons = 0;

void reset_comparisons() {
    g_char_comparisons = 0;
}

unsigned long long get_comparisons() {
    return g_char_comparisons;
}

int compare_strings(const std::string &a, const std::string &b) {
    size_t i = 0;
    size_t na = a.size();
    size_t nb = b.size();
    while (i < na && i < nb) {
        ++g_char_comparisons;
        if (a[i] != b[i]) {
            return (unsigned char)a[i] < (unsigned char)b[i] ? -1 : 1;
        }
        ++i;
    }
    if (na == nb) return 0;
    return na < nb ? -1 : 1;
}

// QuickSort

int partition(std::vector<std::string> &arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (compare_strings(arr[mid], arr[left]) < 0) std::swap(arr[mid], arr[left]);
    if (compare_strings(arr[right], arr[left]) < 0) std::swap(arr[right], arr[left]);
    if (compare_strings(arr[mid], arr[right]) < 0) std::swap(arr[mid], arr[right]);
    std::string pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (compare_strings(arr[j], pivot) <= 0) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[right]);
    return i + 1;
}

void quickSort(std::vector<std::string> &arr, int left, int right) {
    if (left < right) {
        int pivot = partition(arr, left, right);
        quickSort(arr, left, pivot - 1);
        quickSort(arr, pivot + 1, right);
    }
}

// MergeSort

void merge(std::vector<std::string> &arr, size_t left, size_t mid, size_t right) {
    std::vector<std::string> buf(right - left + 1);
    size_t i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (compare_strings(arr[i], arr[j]) <= 0) {
            buf[k++] = std::move(arr[i++]);
        } else {
            buf[k++] = std::move(arr[j++]);
        }
    }
    while (i <= mid) buf[k++] = std::move(arr[i++]);
    while (j <= right) buf[k++] = std::move(arr[j++]);
    for (size_t p = 0; p < k; ++p) {
        arr[left + p] = std::move(buf[p]);
    }
}

void mergeSort(std::vector<std::string> &arr, size_t left, size_t right) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Ternary String QuickSort

std::vector<std::string> TernaryQuickSort(std::vector<std::string> R, int L) {
    if (R.size() <= 1) {
        return R;
    }

    std::vector<std::string> R_excl;
    std::vector<std::string> R_work;

    for (const std::string &s : R) {
        if ((int)s.size() == L) {
            R_excl.push_back(s);
        } else {
            R_work.push_back(s);
        }
    }

    R = R_work;
    if (R.empty()) {
        return R_excl;
    }

    char pivot = R[0][L];

    std::vector<std::string> R_less;
    std::vector<std::string> R_equal;
    std::vector<std::string> R_great;

    for (const std::string &s : R) {
        ++g_char_comparisons;
        if (s[L] < pivot) {
            R_less.push_back(s);
        } else {
            ++g_char_comparisons;
            if (s[L] == pivot) {
                R_equal.push_back(s);
            } else {
                R_great.push_back(s);
            }
        }
    }

    R_less = TernaryQuickSort(R_less, L);
    R_equal = TernaryQuickSort(R_equal, L + 1);
    R_great = TernaryQuickSort(R_great, L);

    std::vector<std::string> result;
    result.insert(result.end(), R_excl.begin(), R_excl.end());
    result.insert(result.end(), R_less.begin(), R_less.end());
    result.insert(result.end(), R_equal.begin(), R_equal.end());
    result.insert(result.end(), R_great.begin(), R_great.end());

    return result;
}

// String MergeSort

std::pair<int, int> lcpCompare(const std::string &A, const std::string &B, int k) {
    int na = (int)A.size();
    int nb = (int)B.size();
    while (k < na && k < nb) {
        ++g_char_comparisons;
        if (A[k] != B[k]) {
            return {(unsigned char)A[k] < (unsigned char)B[k] ? -1 : 1, k};
        }
        ++k;
    }
    if (na == nb) return {0, k};
    return {na < nb ? -1 : 1, k};
}

void stringMerge(std::vector<std::string> &arr, std::vector<int> &lcp,
                 size_t left, size_t mid, size_t right,
                 std::vector<std::string> &tmp_s, std::vector<int> &tmp_l) {
    size_t i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        int ki = lcp[i];
        int pj = lcp[j];
        if (ki > pj) {
            tmp_s[k] = std::move(arr[i]);
            tmp_l[k] = ki;
            ++i;
            ++k;
        } else if (ki < pj) {
            tmp_s[k] = std::move(arr[j]);
            tmp_l[k] = pj;
            ++j;
            ++k;
        } else {
            auto [cmp, l] = lcpCompare(arr[i], arr[j], ki);
            if (cmp <= 0) {
                tmp_s[k] = std::move(arr[i]);
                tmp_l[k] = ki;
                ++i;
                if (j <= right) lcp[j] = l;
            } else {
                tmp_s[k] = std::move(arr[j]);
                tmp_l[k] = pj;
                ++j;
                if (i <= mid) lcp[i] = l;
            }
            ++k;
        }
    }
    while (i <= mid) {
        tmp_s[k] = std::move(arr[i]);
        tmp_l[k] = lcp[i];
        ++i;
        ++k;
    }
    while (j <= right) {
        tmp_s[k] = std::move(arr[j]);
        tmp_l[k] = lcp[j];
        ++j;
        ++k;
    }
    for (size_t p = 0; p < k; ++p) {
        arr[left + p] = std::move(tmp_s[p]);
        lcp[left + p] = tmp_l[p];
    }
}

void stringMergeSort(std::vector<std::string> &arr, std::vector<int> &lcp,
                     size_t left, size_t right,
                     std::vector<std::string> &tmp_s, std::vector<int> &tmp_l) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        stringMergeSort(arr, lcp, left, mid, tmp_s, tmp_l);
        stringMergeSort(arr, lcp, mid + 1, right, tmp_s, tmp_l);
        stringMerge(arr, lcp, left, mid, right, tmp_s, tmp_l);
    }
}

std::vector<std::string> StringMergeSort(std::vector<std::string> R) {
    if (R.size() <= 1) return R;
    std::vector<int> lcp(R.size(), 0);
    std::vector<std::string> tmp_s(R.size());
    std::vector<int> tmp_l(R.size(), 0);
    stringMergeSort(R, lcp, 0, R.size() - 1, tmp_s, tmp_l);
    return R;
}

// MSD Radix Sort

std::vector<std::string> MSDRadixSort(std::vector<std::string> R, int L) {
    if (R.size() <= 1) {
        return R;
    }

    std::vector<std::string> R_excl;
    std::vector<std::vector<std::string>> buckets(256);

    for (const std::string &s : R) {
        if ((int)s.size() == L) {
            R_excl.push_back(s);
        } else {
            ++g_char_comparisons;
            buckets[(unsigned char)s[L]].push_back(s);
        }
    }

    for (int i = 0; i < 256; i++) {
        buckets[i] = MSDRadixSort(buckets[i], L + 1);
    }

    std::vector<std::string> result;
    result.insert(result.end(), R_excl.begin(), R_excl.end());

    for (int i = 0; i < 256; i++) {
        result.insert(result.end(), buckets[i].begin(), buckets[i].end());
    }

    return result;
}

// MSD Radix Sort with switching

std::vector<std::string> MSBRadixSort(std::vector<std::string> R, int L, int m) {
    if ((int)R.size() < m) {
        return TernaryQuickSort(R, L);
    }

    std::vector<std::string> R_excl;
    std::vector<std::vector<std::string>> buckets(256);

    for (const std::string &s : R) {
        if ((int)s.size() == L) {
            R_excl.push_back(s);
        } else {
            ++g_char_comparisons;
            buckets[(unsigned char)s[L]].push_back(s);
        }
    }

    for (int i = 0; i < 256; i++) {
        buckets[i] = MSBRadixSort(buckets[i], L + 1, m);
    }

    std::vector<std::string> result;
    result.insert(result.end(), R_excl.begin(), R_excl.end());

    for (int i = 0; i < 256; i++) {
        result.insert(result.end(), buckets[i].begin(), buckets[i].end());
    }

    return result;
}
