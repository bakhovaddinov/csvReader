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
#include <vector>

std::unordered_map<std::string, int> split_headers(const std::string &headers_row)
{
    // ,abc,def,asdf
    size_t begin = 1;
    size_t end = 0;
    std::unordered_map<std::string, int> headers_values;
    while ((end = headers_row.find(',', begin)) != std::string_view::npos) {
        std::string current_header = headers_row.substr(begin, end - begin);
        if (current_header.empty()) {
            throw std::invalid_argument("Empty column name!");
        } else if (auto search = headers_values.find(current_header); search != headers_values.end()) {
            throw std::invalid_argument("Non-unique columns names!");
        }
        headers_values.emplace(std::move(current_header), headers_values.size());
        begin = end + 1;
    };
    auto current_header = headers_row.substr(begin);
    if (current_header.empty()) {
        throw std::invalid_argument("Empty column name!");
    }
    headers_values.emplace(std::move(current_header), headers_values.size());
    return headers_values;
}

std::pair<int, std::vector<std::string>> split_rows(const std::string &row_line)
{
    // 1,2,34,5,4,
    size_t begin = 0;
    size_t end = row_line.find(',', begin);
    std::vector<std::string> row_values;
    int row_index = stoi(row_line.substr(begin, end));
    begin = end + 1;
    while ((end = row_line.find(',', begin)) != std::string_view::npos) {
        std::string current_value = row_line.substr(begin, end - begin);
        row_values.push_back(current_value);
        begin = end + 1;
    };
    auto current_value = row_line.substr(begin);
    row_values.push_back(current_value);
    return {row_index, row_values};
}

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
    std::ifstream csv_file(file_name);

    std::vector<std::vector<std::string>> table;

    std::string column;
    std::getline(csv_file, column);
    std::unordered_map<std::string, int> headers = split_headers(column);

    std::string line;
    while (std::getline(csv_file, line)) {
        auto [current_index, current_line] = split_rows(line);
        table.push_back(current_line);
    }

    for (const auto &row : table) {
        for (const auto &value : row) {
            std::cout << value << ',';
        }
        std::cout << '\n';
    }
    return EXIT_SUCCESS;
}
