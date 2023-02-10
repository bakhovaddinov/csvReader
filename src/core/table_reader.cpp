#include "table_reader.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace csv_reader::core
{
namespace
{

bool isValidInt(std::string_view s)
{
    if (s.empty()) {
        return false;
    }

    int start = 0;
    int length = s.size();

    if (s[0] == '-') {
        if (length == 1) {
            return false;
        }
        start = 1;
    }

    if (s[start] == '0' && length > 1) {
        return false;
    }

    for (int i = start; i < length; i++) {
        if (!std::isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

int to_int(std::string_view sv)
{
    int result{};
    std::from_chars(sv.data(), sv.data() + sv.size(), result);
    return result;
}

int calculate_operation(std::string_view arg1, std::string arg2, char operation)
{
    int result = 0;
    switch (operation) {
    case '+':
        result = to_int(arg1) + to_int(arg2);
        break;
    case '-':
        result = to_int(arg1) - to_int(arg2);
        break;
    case '*':
        result = to_int(arg1) * to_int(arg2);
        break;
    case '/':
        if (to_int(arg2)) {
            throw std::runtime_error("Math error: Attempted to divide by Zero\n");
        }
        result = to_int(arg1) / to_int(arg2);
        break;
    default:
        auto error_message = std::string{"Unexpected operation: "} + operation;
        throw std::invalid_argument(std::move(error_message));
    }
    return result;
}

bool is_operation(char c)
{
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
        return true;
    }
    return false;
}

std::string unfold_value(std::string_view value,
                         std::unordered_map<std::string, std::string> &cells)
{
    if (value[0] != '=') {
        auto key = std::string(value);
        if (isValidInt(value)) {
            return key;
        } else if (cells.count(key) == 0) {
            auto error_message = std::string{"Invalid value: "} + std::string(value);
            throw std::invalid_argument(error_message);
        }
        auto it = cells.find(key);
        it->second = unfold_value(it->second, cells);
        return it->second;
    }
    auto it = std::find_if(value.begin(), value.end(), is_operation);
    // = A1
    if (it == value.end()) {
        auto error_message = "Failed to parse a cell value:" + std::string{value};
        throw std::invalid_argument(error_message);
    }
    char operation = *it;
    size_t position = value.find(operation);
    auto arg1 = unfold_value(value.substr(1, position - 1), cells);
    auto arg2 = unfold_value(value.substr(position + 1, value.size()), cells);
    auto result = std::to_string(calculate_operation(arg1, arg2, operation));
    return result;
}

std::vector<std::string> split_headers(const std::string &headers_line)
{
    // ,abc,def,asdf
    size_t begin = 1;
    size_t end = 0;
    std::vector<std::string> headers;
    while ((end = headers_line.find(',', begin)) != std::string::npos) {
        std::string current_header = headers_line.substr(begin, end - begin);
        if (current_header.empty()) {
            throw std::invalid_argument("Empty column name!");
        }
        headers.push_back(std::move(current_header));
        begin = end + 1;
    };
    auto current_header = headers_line.substr(begin);
    if (current_header.empty()) {
        throw std::invalid_argument("Empty column name!");
    }
    headers.push_back(std::move(current_header));
    std::set<std::string> unique_headers(headers.begin(), headers.end());
    if (unique_headers.size() != headers.size()) {
        throw std::invalid_argument("There are non-unique column headers!");
    }
    return headers;
}

std::pair<std::string, std::vector<std::string>> split_row(const std::string &row_line,
                                                           const std::vector<std::string> &headers)
{
    // 1,2,34,5,4,
    size_t begin = 0;
    size_t end = row_line.find(',', begin);
    std::string row_index = row_line.substr(begin, end);
    begin = end + 1;
    if (!isValidInt(row_index) || row_index.empty()) {
        std::string error_message =
            std::string{"The given row_index is not a positive integer:"} + row_index;
        throw std::invalid_argument(std::move(error_message));
    }
    std::vector<std::string> row;
    int current_column = 0;
    while ((end = row_line.find(',', begin)) != std::string::npos) {
        std::string current_value = row_line.substr(begin, end - begin);
        if (current_value.size() == 0) {
            std::string error_message =
                std::string{"The given cell is empty: "} + headers[current_column] + row_index;
            throw std::invalid_argument(std::move(error_message));
        }
        row.push_back(current_value);
        begin = end + 1;
        current_column++;
    }
    auto current_value = row_line.substr(begin);
    row.push_back(current_value);
    return {row_index, row};
}

} // namespace

CsvTable::CsvTable(std::string &file_name)
{
    std::ifstream input;
    input.open(file_name.c_str());
    std::string headers_line;
    std::getline(input, headers_line);
    headers = split_headers(headers_line);

    std::string current_line;
    const auto &headers_size = headers.size();
    while (std::getline(input, current_line)) {
        auto [row_index, row] = split_row(current_line, headers);
        row_indices.push_back(row_index);
        for (int i = 0; i < headers_size; i++) {
            std::string index = headers[i] + row_index;
            cells[index] = row[i];
        }
    }
    std::set<std::string> unique_indices(row_indices.begin(), row_indices.end());
    if (unique_indices.size() != unique_indices.size()) {
        throw std::invalid_argument("There are non-unique row indices!");
    }
}

std::string CsvTable::operator[](const std::string &address)
{
    auto it = cells.find(address);
    it->second = unfold_value(it->second, cells);
    return it->second;
}

void CsvTable::print()
{
    std::cout << ',';
    for (auto header : headers) {
        std::cout << header << ',';
    }
    for (auto row_index : row_indices) {
        std::cout << '\n' << row_index << ',';
        for (auto header : headers) {
            std::cout << (*this)[header + row_index] << ',';
        }
    }
}
} // namespace csv_reader::core
