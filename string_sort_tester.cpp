#include "string_sort_tester.h"

#include <chrono>

#include "sorts.h"

StringSortTester::StringSortTester(int repeats) : repeats_(repeats) {}

TestResult StringSortTester::run(const std::string& algo_name,
                                 SortFn sort,
                                 const std::string& array_type,
                                 const std::vector<std::string>& data) {
    TestResult res;
    res.algorithm = algo_name;
    res.array_type = array_type;
    res.n = (int)data.size();

    double total_us = 0.0;
    double total_cmp = 0.0;

    for (int r = 0; r < repeats_; ++r) {
        std::vector<std::string> copy = data;
        reset_comparisons();

        auto t0 = std::chrono::high_resolution_clock::now();
        sort(copy);
        auto t1 = std::chrono::high_resolution_clock::now();

        double us = std::chrono::duration<double, std::micro>(t1 - t0).count();
        total_us += us;
        total_cmp += (double)get_comparisons();
    }

    res.avg_time_us = total_us / repeats_;
    res.avg_comparisons = total_cmp / repeats_;
    return res;
}
