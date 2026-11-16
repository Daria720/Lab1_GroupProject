#include "shared_types.h"
#include <cmath>
#include <chrono>
#include <vector>
#include <array>
#include <stdexcept>
#include <algorithm>

std::unique_ptr<Result> calculateB(std::shared_ptr<const InputData> data) {
    auto start = std::chrono::high_resolution_clock::now();

    if (!data || data->points.size() < 3) {
        throw std::invalid_argument("Not enough points for quadratic approximation");
    }

    const size_t n = data->points.size();
    double sX = 0.0, sX2 = 0.0, sX3 = 0.0, sX4 = 0.0;
    double sY = 0.0, sXY = 0.0, sX2Y = 0.0;

    for (const auto& [x, y] : data->points) {
        double x2 = x * x;
        sX += x;
        sX2 += x2;
        sX3 += x2 * x;
        sX4 += x2 * x2;
        sY += y;
        sXY += x * y;
        sX2Y += x2 * y;
    }

    // Augmented matrix for [a0, a1, a2]
    std::array<std::array<double, 4>, 3> mat = { {
        {static_cast<double>(n), sX,  sX2, sY},
        {sX,                     sX2, sX3, sXY},
        {sX2,                    sX3, sX4, sX2Y}
    } };

    // Gaussian elimination with partial pivoting
    for (int i = 0; i < 3; ++i) {
        int maxRow = i;
        for (int k = i + 1; k < 3; ++k) {
            if (std::abs(mat[k][i]) > std::abs(mat[maxRow][i])) {
                maxRow = k;
            }
        }
        std::swap(mat[i], mat[maxRow]);

        if (std::abs(mat[i][i]) < 1e-12) {
            throw std::runtime_error("Singular matrix in quadratic least squares");
        }

        for (int k = i + 1; k < 3; ++k) {
            double factor = mat[k][i] / mat[i][i];
            for (int j = i; j < 4; ++j) {
                mat[k][j] -= factor * mat[i][j];
            }
        }
    }

    // Back substitution
    std::vector<double> coeffs(3);
    for (int i = 2; i >= 0; --i) {
        double sum = mat[i][3];
        for (int j = i + 1; j < 3; ++j) {
            sum -= mat[i][j] * coeffs[j];
        }
        coeffs[i] = sum / mat[i][i];
    }

    // Root Mean Square Error (RMSE)
    double sqErrorSum = 0.0;
    for (const auto& [x, y] : data->points) {
        double yPred = coeffs[0] + coeffs[1] * x + coeffs[2] * x * x;
        double diff = y - yPred;
        sqErrorSum += diff * diff;
    }
    double rmse = std::sqrt(sqErrorSum / n);

    auto end = std::chrono::high_resolution_clock::now();
    double execTimeMs = std::chrono::duration<double, std::milli>(end - start).count();

    return std::make_unique<Result>(Result{
        coeffs,
        rmse,
        execTimeMs
        });
}