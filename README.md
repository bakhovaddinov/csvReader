# csvReader
A csv format file reader implemented using C++

# How to use
Pass a csv file name via the command line. The application will calculate values in cells that contain expressions in a form of: "=ARG1 OP ARG2", where ARG1, ARG1 are integers or other cell's addresses and OP is an arithmetic operation from the list: +, -, *, / 

When finished, the program will print the contents of the finilized csv

**Note** that row indices must be positive integers in any order and the cells themselves must contain integer values
