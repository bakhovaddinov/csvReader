#include "table_reader.hpp"
#include <exception>
#include <fstream>
#include <vector>

namespace csv_reader::core
{
std::unordered_map<std::string, Row> TableReader::read(std::string file_name)
{
    std::ifstream csv_file;
    std::unordered_map<std::string, Row> table;
    csv_file.open(file_name.c_str());
    std::string column;
    std::vector<std::string> headers;
    std::getline(csv_file, column);
    size_t pos = 0;
    std::string current_header;
    while ((pos = column.find(',')) != std::string::npos) {
        current_header = column.substr(0, pos);
        headers.push_back(current_header);
        column.erase(0, pos + 1);
    };

    std::string line;
    while (std::getline(csv_file, line)) {
        Row current_row;
        current_row.parse(line);
    }
};
} // namespace csv_reader::core
