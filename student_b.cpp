#include "shared_types.h"
#include <array>
#include <chrono>
#include <cmath>
#include <stdexcept>
#include <utility>

std::unique_ptr<Result> calculateB(std::shared_ptr<const InputData> data)
{
    if (!data || data->points.size() < 3)
    {
        throw std::invalid_argument("Quadratic approximation requires at least 3 points.");
    }

    const auto startTime = std::chrono::steady_clock::now();

    const std::size_t n = data->points.size();
    double sx = 0.0, sx2 = 0.0, sx3 = 0.0, sx4 = 0.0;
    double sy = 0.0, sxy = 0.0, sx2y = 0.0;
    std::size_t ops = 0;

    for (const auto& [x, y] : data->points)
    {
        const double x2 = x * x;
        const double x3 = x2 * x;
        const double x4 = x2 * x2;

        sx += x;
        sx2 += x2;
        sx3 += x3;
        sx4 += x4;

        sy += y;
        sxy += x * y;
        sx2y += x2 * y;
        ops += 12;
    }

    const double nd = static_cast<double>(n);

    std::array<std::array<double, 4>, 3> mat = {{
        {nd,  sx,  sx2, sy},
        {sx,  sx2, sx3, sxy},
        {sx2, sx3, sx4, sx2y}
    }};

    for (std::size_t col = 0; col < 3; ++col)
    {
        std::size_t pivotRow = col;
        for (std::size_t row = col + 1; row < 3; ++row)
        {
            if (std::abs(mat[row][col]) > std::abs(mat[pivotRow][col]))
            {
                pivotRow = row;
            }
        }

        if (std::abs(mat[pivotRow][col]) < 1e-12)
        {
            throw std::runtime_error("Singular normal matrix in quadratic least squares.");
        }

        if (pivotRow != col)
        {
            std::swap(mat[col], mat[pivotRow]);
        }

        for (std::size_t row = col + 1; row < 3; ++row)
        {
            const double factor = mat[row][col] / mat[col][col];
            ops += 1;
            for (std::size_t k = col; k < 4; ++k)
            {
                mat[row][k] -= factor * mat[col][k];
                ops += 2;
            }
        }
    }

    std::array<double, 3> a{};
    for (int i = 2; i >= 0; --i)
    {
        double sum = mat[i][3];
        for (int j = i + 1; j < 3; ++j)
        {
            sum -= mat[i][j] * a[j];
            ops += 2;
        }
        a[i] = sum / mat[i][i];
        ops += 1;
    }

    const double meanY = sy / nd;
    ops += 1;

    double sqErrSum = 0.0;
    double totSqSum = 0.0;

    for (const auto& [x, y] : data->points)
    {
        const double yPred = a[0] + a[1] * x + a[2] * x * x;
        const double err = y - yPred;
        const double diffMean = y - meanY;
        sqErrSum += err * err;
        totSqSum += diffMean * diffMean;
        ops += 11;
    }

    const double rmse = std::sqrt(sqErrSum / nd);
    const double rSquared = (totSqSum > 1e-12) ? (1.0 - sqErrSum / totSqSum) : 1.0;
    ops += 4;

    const auto endTime = std::chrono::steady_clock::now();
    const double elapsedUs = std::chrono::duration<double, std::micro>(endTime - startTime).count();

    auto res = std::make_unique<Result>();
    res->modelName = "Quadratic Least Squares: y = a0 + a1*x + a2*x^2";
    res->coefficients = {a[0], a[1], a[2]};
    res->rmse = rmse;
    res->rSquared = rSquared;
    res->operationsCount = ops;
    res->executionTimeUs = elapsedUs;

    return res;
}