#pragma once

#include <vector>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace sorters {
std::vector<int> sort(std::vector<int> benchmark);
std::vector<int> std_par_sort(std::vector<int> benchmark);
std::vector<int> std_quick_sort(std::vector<int> benchmark);
std::vector<int> custom_par_sort(std::vector<int> benchmark);
} // namespace sorters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
