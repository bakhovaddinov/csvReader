#include "table_reader.hpp"
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace csv_reader::core
{

// unordered map<string, int> headers;
// vector<vector<string>> rows;
// unordered_map <int, int> orders; A1
// int column_index = headers[A]
// int row_index = orders[1]
// string value = rows[row_index][column_index]

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Invalid number of params" << argc << std::endl;
        return EXIT_FAILURE;
    }
    std::string file_name = argv[1];
    if (!std::filesystem::exists(file_name)) {
        std::cout << "File does not exist" << std::endl;
        return EXIT_FAILURE;
    }
    CsvTable table = CsvTable(file_name);
    std::cout << table["A2"] << std::endl;
    return EXIT_SUCCESS;
}
} // namespace csv_reader::core
