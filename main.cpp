#include "shared_types.h"
#include <iomanip>
#include <iostream>
#include <memory>

int main()
{
    auto data = std::make_shared<const InputData>(InputData{
        {
            {0.0, 2.1}, {1.0, 4.2}, {2.0, 8.3}, {3.0, 13.6}, {4.0, 20.9},
            {5.0, 29.4}, {6.0, 39.8}, {7.0, 51.5}, {8.0, 65.3}, {9.0, 80.1}
        }
    });

    std::cout << "Experimental points count: " << data->points.size() << "\n\n";

    // === ALGORITHM CALLS SECTION ===
    auto resultB = calculateB(data);
    auto [nameB, coeffsB, rmseB, r2B, opsB, timeB] = *resultB;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "[Student B] " << nameB << "\n";
    std::cout << "  Coefficients: a0 = " << coeffsB[0]
              << ", a1 = " << coeffsB[1] << ", a2 = " << coeffsB[2] << "\n";
    std::cout << "  RMSE = " << rmseB << ", R^2 = " << r2B << ", Ops = " << opsB << "\n\n";

    return 0;
}