#pragma once

#include "Node.hpp"
#include <string>

class Benchmark {
public:
    static double measureLoadTime(const std::string& filename);
    static double measureSearchTime(const Node* root, const std::string& objectName);
    static double measureGameSpeed(const Node* root, int iterations);
};