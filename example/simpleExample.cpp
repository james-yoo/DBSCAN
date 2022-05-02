#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "dbscan.h"
#define MINIMUM_POINTS 2  // minimum number of cluster
#define EPSILON (3.0)     // distance for clustering, metre^2

std::vector<coords> readBenchmarkData(const std::string &filename) {
    // load point cloud
    std::vector<coords> points;
    std::ifstream input;
    input.open(filename.c_str());
    if (input.is_open()) {
        unsigned int num_points, cluster, i = 0;
        input >> num_points;
        float x, y;
        while (i < num_points) {
            input >> x >> y >> cluster;
            points.push_back({x, y});
            ++i;
        }
        input.close();
        return points;
    } else
        return {};
}

void printResults(const vector<Point> &points, int num_points) {
    int i = 0;
    std::cout << "Number of Points: " << num_points << std::endl;
    std::cout << "x\t\t\ty\t\t\t" << std::endl;

    while (i < num_points) {
        std::cout << points[i] << std::endl;
        ++i;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Expected data filename" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto filename = argv[1];
    // read point data
    std::vector<coords> points = readBenchmarkData(filename);
    // constructor
    DBSCAN ds(MINIMUM_POINTS, EPSILON, points);
    // main loop
    ds.run();
    for (auto p : ds.getPoints()) {
        std::cout << p << std::endl;
    }
    auto clusters = ds.getClusters();
    for (auto &c : clusters) {
        c.update();
        std::cout << c << std::endl;
    }
    std::cout << "Mean radius is: " << ds.radiusOfClusters() << std::endl;
    return 0;
}
