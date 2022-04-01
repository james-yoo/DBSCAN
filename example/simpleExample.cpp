#include <dbscan.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>
#define MINIMUM_POINTS 4       // minimum number of cluster
#define EPSILON (0.75 * 0.75)  // distance for clustering, metre^2

std::vector<Point> readBenchmarkData() {
  // load point cloud
  std::vector<Point> points;
  FILE *stream;
  stream = fopen("benchmark_hepta.dat", "ra");

  unsigned int minpts, num_points, cluster, i = 0;
  double epsilon;
  fscanf(stream, "%u\n", &num_points);

  Point *p = (Point *)calloc(num_points, sizeof(Point));

  while (i < num_points) {
    fscanf(stream, "%f,%f,%f,%d\n", &(p[i].x), &(p[i].y), &(p[i].z), &cluster);
    p[i].clusterID = UNCLASSIFIED;
    points.push_back(p[i]);
    ++i;
  }

  free(p);
  fclose(stream);
  return points;
}

void printResults(const vector<Point> &points, int num_points) {
  int i = 0;
  std::cout << "Number of Points: " << num_points << std::endl;
  std::cout << "x\t\t\ty\t\t\tz\t\t\t" << std::endl;

  while (i < num_points) {
    printf("%5.2lf %5.2lf %5.2lf: %d\n", points[i].x, points[i].y, points[i].z,
           points[i].clusterID);
    ++i;
  }
}

int main() {
  // read point data
  std::vector<Point> points = readBenchmarkData();

  // constructor
  DBSCAN ds(MINIMUM_POINTS, EPSILON, points);
  // main loop
  ds.run();
  // result of DBSCAN algorithm
  printResults(ds.getPoints(), ds.getTotalPointSize());

  return 0;
}
