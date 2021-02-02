# Wet2
g++ -std=c++0x -g -pedantic-errors -Werror -Wall *.cpp
valgrind --leak-check=full ./a.out
python ./OasisTest.py NumberOfLines TestName
