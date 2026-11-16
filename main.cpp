#include <iostream>
#include <iomanip>
#include "shared_types.h"

int main() {
    auto data = std::make_shared<const InputData>(InputData{
        {
            {1.0, 2.1},
            {2.0, 3.9},
            {3.0, 6.2},
            {4.0, 7.8},
            {5.0, 10.1},
            {6.0, 12.2}
        }
        });

    std::cout << std::fixed << std::setprecision(4);

    // Shared section for algorithm calls (controlled conflict)
    auto resultA = calculateA(data);
    auto [coeffsA, rmseA, timeA] = *resultA;
    std::cout << "[Student A - Linear Approximation]\n";
    std::cout << "Model: y = " << coeffsA[0] << " + " << coeffsA[1] << " * x\n";
    std::cout << "RMSE: " << rmseA << " | Time: " << timeA << " ms\n\n";
    //End of shared section

    return 0;
}