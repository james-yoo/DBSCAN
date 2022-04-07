#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "dbscan.h"
#define MINIMUM_POINTS 6  // minimum number of cluster
#define EPSILON (2.0)     // distance for clustering, metre^2

std::vector<Point> readBenchmarkData(const std::string &filename) {
    // load point cloud
    std::vector<Point> points;
    std::ifstream input;
    input.open(filename.c_str());
    if (input.is_open()) {
        unsigned int num_points, cluster, i = 0;
        input >> num_points;
        float x, y;
        while (i < num_points) {
            input >> x >> y >> cluster;
            points.push_back(Point(x, y, UNCLASSIFIED));
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
    std::vector<Point> points = readBenchmarkData(filename);
    // constructor
    DBSCAN ds(MINIMUM_POINTS, EPSILON, points);
    // main loop
    ds.run();
    std::cout << "Number of clusters: " << ds.getNClusters() << std::endl;
    // result of DBSCAN algorithm
    // printResults(ds.getPoints(), ds.getTotalPointSize());

    return 0;
}
