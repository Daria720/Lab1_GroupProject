#include "shared_types.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>

int main()
{
    try
    {
        auto data = std::make_shared<const InputData>(InputData{
            {
                {0.0, 2.1}, {1.0, 4.2}, {2.0, 8.3}, {3.0, 13.6}, {4.0, 20.9},
                {5.0, 29.4}, {6.0, 39.8}, {7.0, 51.5}, {8.0, 65.3}, {9.0, 80.1}
            }
        });

        std::cout << "=================================================================\n";
        std::cout << "  Lab 1 (Variant 7): Least Squares Approximation Comparison\n";
        std::cout << "  Experimental points count: " << data->points.size() << "\n";
        std::cout << "=================================================================\n\n";

        auto resultA = calculateA(data);
        auto [nameA, coeffsA, rmseA, r2A, opsA, timeA] = *resultA;

        auto resultB = calculateB(data);
        auto [nameB, coeffsB, rmseB, r2B, opsB, timeB] = *resultB;

        std::cout << std::fixed << std::setprecision(4);

        std::cout << "[Student A] " << nameA << "\n";
        std::cout << "  Model:        y = " << coeffsA[0] << " + (" << coeffsA[1] << ") * x\n";
        std::cout << "  RMSE:         " << rmseA << "\n";
        std::cout << "  R^2:          " << r2A << "\n";
        std::cout << "  Operations:   " << opsA << "\n";
        std::cout << "  Exec time:    " << timeA << " us\n\n";

        std::cout << "[Student B] " << nameB << "\n";
        std::cout << "  Model:        y = " << coeffsB[0] << " + (" << coeffsB[1] << ") * x + ("
                  << coeffsB[2] << ") * x^2\n";
        std::cout << "  RMSE:         " << rmseB << "\n";
        std::cout << "  R^2:          " << r2B << "\n";
        std::cout << "  Operations:   " << opsB << "\n";
        std::cout << "  Exec time:    " << timeB << " us\n\n";

        std::cout << "------------------------ COMPARISON -----------------------------\n";
        std::cout << "  RMSE ratio (Linear / Quadratic): " << (rmseA / rmseB) << "x higher error in linear\n";
        std::cout << "  Arithmetic ops overhead:         +" << (opsB - opsA) << " ops ("
                  << static_cast<double>(opsB) / static_cast<double>(opsA) << "x)\n";

        if (rmseB < rmseA)
        {
            std::cout << "  Conclusion: Quadratic model provides significantly better fit (lower RMSE).\n";
        }
        else
        {
            std::cout << "  Conclusion: Linear model is sufficient for this dataset.\n";
        }
        std::cout << "=================================================================\n";
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}