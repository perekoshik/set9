#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <functional>
#include <string>
#include <vector>

struct TestResult {
    std::string algorithm;
    std::string array_type;
    int n;
    double avg_time_us;
    double avg_comparisons;
};

class StringSortTester {
public:
    using SortFn = std::function<void(std::vector<std::string>&)>;

    StringSortTester(int repeats = 3);

    TestResult run(const std::string& algo_name,
                   SortFn sort,
                   const std::string& array_type,
                   const std::vector<std::string>& data);

private:
    int repeats_;
};

#endif
