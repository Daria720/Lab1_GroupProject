#include "shared_types.h"
#include <cmath>
#include <chrono>
#include <stdexcept>

std::unique_ptr<Result> calculateA(std::shared_ptr<const InputData> data) {
    auto start = std::chrono::high_resolution_clock::now();

    if (!data || data->points.size() < 2) {
        throw std::invalid_argument("Not enough points for linear approximation");
    }

    const size_t n = data->points.size();
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;

    for (const auto& [x, y] : data->points) {
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    double denominator = n * sumX2 - sumX * sumX;
    if (std::abs(denominator) < 1e-12) {
        throw std::runtime_error("Denominator is zero in linear least squares");
    }

    // Коефіцієнти лінійної моделі y = a0 + a1 * x
    double a1 = (n * sumXY - sumX * sumY) / denominator;
    double a0 = (sumY - a1 * sumX) / n;

    // Обчислення середньоквадратичної похибки (RMSE)
    double sqErrorSum = 0.0;
    for (const auto& [x, y] : data->points) {
        double yPred = a0 + a1 * x;
        double diff = y - yPred;
        sqErrorSum += diff * diff;
    }
    double rmse = std::sqrt(sqErrorSum / n);

    auto end = std::chrono::high_resolution_clock::now();
    double execTimeMs = std::chrono::duration<double, std::milli>(end - start).count();

    return std::make_unique<Result>(Result{
        {a0, a1},
        rmse,
        execTimeMs
        });
}