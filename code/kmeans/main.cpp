#include "include/kmeans.hpp"
/* <vector> <string> */
#include <iomanip>
#include <iostream>

int main() {
	srand(time(NULL));
	std::cout << std::fixed << std::setprecision(3); // Set precision
    KMeans kmeans("../dataset/data2k.csv");

    return 0;
}
