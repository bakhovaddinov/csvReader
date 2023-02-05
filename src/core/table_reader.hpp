#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace csv_reader::core
{

class CsvTable
{
  public:
    CsvTable(std::istream &input);
    CsvTable(const std::string &file_name);
    std::string operator[](const std::string &address);

    static CsvTable from_file(const std::string &file_name);

  private:
    std::unordered_map<std::string, std::string> cells;
    std::vector<std::string> headers;
    std::vector<std::string> row_indecies;
};

} // namespace csv_reader::core
