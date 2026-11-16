#include <iostream>
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

    std::cout << "Project initialized successfully. Points count: " << data->points.size() << "\n";

    return 0;
}