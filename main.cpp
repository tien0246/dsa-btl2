#include "main.h"
#include "restaurant.cpp"

int main(int argc, char* argv[]) {
    if (argc < 2)
        return 1;
    string fileName = argv[1];

    // string fileName = "test.txt";
    simulate(fileName);
    return 0;
}
