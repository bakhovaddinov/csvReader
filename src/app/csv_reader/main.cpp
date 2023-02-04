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
    std::vector<std::string> expressions;
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

// std::pair<std::vector<std::string>, std::vector<char>> unfold_value(const std::string &value)
// {
//     //A1 + B5 -> B5 = H5 + C8
//     std::vector<std::string> nested_values;
//     std::vector<char> nested_operations;
//     for (auto sign : {'+', '-', '*', '/'}) { //make const array
//         if (auto position = value.find(sign); position != value.npos) {
//             nested_values.push_back(value.substr(0, position));
//             nested_values.push_back(value.substr(position, value.size()));
//             nested_operations.push_back(sign);
//         }
//         for (auto value : nested_values) {
//             unfold_value(value);
//         }
//     }
//     return std::make_pair(nested_values, nested_operations);
// }

// std::string calculate_value(const std::string &value, ) {

// }
// func(&cell):
//

// unordered map<string, int> headers;
// vector<vector<string>> rows;
// unordered_map <int, int> orders; A1
// int column_index = headers[A]
// int row_index = orders[1]
// string value = rows[row_index][column_index]

bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

std::string calculate_value(const std::string &value);

int unfold_value(const std::string &value)
{
    std::vector<char> signs = {'+', '-', '*', '/'};
    std::string arg1;
    std::string arg2;
    char operation;
    for (auto sign : signs) {
        if (auto operation = value.find(sign); operation != value.npos) {
            arg1 = value.substr(1, operation);
            arg2 = value.substr(operation + 1, value.size());
        }
        operation = sign;
    }
    if (!is_number(arg1)) {
        arg1 = calculate_value(arg1);
    }

    if (!is_number(arg2)) {
        arg2 = calculate_value(arg2);
    }

    switch (operation) {
    case '+':
        return stoi(arg1) + stoi(arg2);
    case '-':
        return stoi(arg1) - stoi(arg2);
    case '*':
        return stoi(arg1) * stoi(arg2);
    case '/':
        return stoi(arg1) / stoi(arg2);
    }
}

std::string calculate_value(const std::string &value)
{
    if (value[0] == '=') {
        return std::to_string(unfold_value(value));
    }
    return value;
}

// table["A1"] = "asdf"
class Table
{
  private:
    std::unordered_map<std::string, std::string> table;
    std::vector<std::string> columns;
    std::vector<std::string> row_indexes;

  public:
    std::string GetValue(const std::string &adress)
    {
        return table[adress];
    }
};

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
