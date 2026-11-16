#pragma once

#include <vector>
#include <memory>

struct Point {
    double x;
    double y;
};

struct InputData {
    std::vector<Point> points;
};

struct Result {
    std::vector<double> coefficients;
    double rmse;
    double executionTimeMs;
};

std::unique_ptr<Result> calculateA(std::shared_ptr<const InputData> data);
std::unique_ptr<Result> calculateB(std::shared_ptr<const InputData> data);