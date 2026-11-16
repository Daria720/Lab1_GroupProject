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

    // Call Student A algorithm
    auto resultA = calculateA(data);
    auto [coeffsA, rmseA, timeA] = resultA;
    std::cout << "[Student A - Linear Approximation]\n";
    std::cout << "Model: y = " << coeffsA[0] << " + " << coeffsA[1] << " x\n";
    std::cout << "RMSE: " << rmseA << " | Time: " << timeA << " ms\n\n";

    // Call Student B algorithm
    auto resultB = calculateB(data);
    auto [coeffsB, rmseB, timeB] = resultB;
    std::cout << "[Student B - Quadratic Approximation]\n";
    std::cout << "Model: y = " << coeffsB[0] << " + " << coeffsB[1] << " x + " << coeffsB[2] << " * x^2\n";
    std::cout << "RMSE: " << rmseB << " | Time: " << timeB << " ms\n\n";

    // Comparison of models
    std::cout << " Comparison \n";
    if (rmseB < rmseA) {
        std::cout << "Quadratic model has lower RMSE (better fit) by " << (rmseA - rmseB) << "\n";
    } else {
        std::cout << "Linear model has lower or equal RMSE by " << (rmseB - rmseA) << "\n";
    }
    std::cout << "Linear model parameters: 2 (O(n) complexity) vs Quadratic model parameters: 3 (O(n) + 3x3 Gauss)\n";

    return 0;
}