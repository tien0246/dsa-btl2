g++ -o main main.cpp -I . -std=c++11 -fsanitize=address -static-libasan -g
./main test.txt
