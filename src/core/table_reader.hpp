#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace csv_reader::core
{
class TableReader
{
  private:
    std::unordered_map<std::string, std::string> table;
    std::vector<std::string> columns;
    std::vector<std::string> row_indexes;

  public:
    std::unordered_map<std::string, std::string> Read(std::string file_name);
    std::string GetValue(const std::string &adress);
};
} // namespace csv_reader::core
