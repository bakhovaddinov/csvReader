#include "table_reader.hpp"

#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <stdexcept>

using namespace csv_reader::core;

TEST_CASE("Test table reader")
{
    std::string empty_cell_csv = "/home/iskandar/projects/csvReader/test_data/empty_cell.csv";
    SECTION("Check for empty cell error")
    {
        REQUIRE_THROWS_AS(CsvTable(empty_cell_csv), std::invalid_argument);
    }
    std::string empty_column_csv = "/home/iskandar/projects/csvReader/test_data/empty_column.csv";
    SECTION("Check for empty column error")
    {
        REQUIRE_THROWS_AS(CsvTable(empty_column_csv), std::invalid_argument);
    }
    std::string empty_row_index_csv =
        "/home/iskandar/projects/csvReader/test_data/empty_row_index.csv";
    SECTION("Check for empty row index error")
    {
        REQUIRE_THROWS_AS(CsvTable(empty_row_index_csv), std::invalid_argument);
    }
}
