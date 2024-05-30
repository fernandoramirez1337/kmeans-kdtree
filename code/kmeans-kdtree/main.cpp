#include "include/kmeans.hpp"
/* <vector> <string> */
#include <iomanip>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>  // for std::exit
#include <string>   // for std::stoi

int main(int argc, char* argv[]) {
    // Check if there are exactly 3 arguments (program name + 2 integers)
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <int1> <int2>" << std::endl;
        return 1;  // Exit with a non-zero status to indicate error
    }

    try {
        // Convert command-line arguments to integers
        int int1 = std::stoi(argv[1]);
        int int2 = std::stoi(argv[2]);

        // Perform a task with the integers (e.g., print them and their sum)
        srand(time(NULL));
        std::cout << std::fixed << std::setprecision(3); // Set precision
        auto start = std::chrono::high_resolution_clock::now();
        KMeans kmeans("../dataset/data2k.csv",int1,int2);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        std::cout << "Time taken: " << duration_ms.count() << " milliseconds" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: Please enter valid integers." << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range: The entered number is too large." << std::endl;
        return 1;
    }

    return 0;  // Exit with zero status to indicate success
}
