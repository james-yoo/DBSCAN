#ifndef DBSCAN_H
#define DBSCAN_H

#include <iostream>
#include <utility>
#include <vector>

#define UNCLASSIFIED -1
#define CORE_POINT 1
#define BORDER_POINT 2
#define NOISE -2
#define SUCCESS 0
#define FAILURE -3

using namespace std;

typedef struct Point_ {
    float x, y;     // X, Y position
    int clusterID;  // clustered ID
    Point_(const float& _x, const float& _y, const int& _c)
        : x(_x), y(_y), clusterID(_c) {}

    friend ostream& operator<<(std::ostream& os, const Point_& p) {
        return os << "(" << p.x << ", " << p.y << ") - " << p.clusterID;
    }
} Point;

class DBSCAN {
   public:
    DBSCAN(const unsigned int& minPts, const float& eps,
           const vector<Point>& points);
    ~DBSCAN() = default;

    int run();
    vector<int> calculateCluster(Point point);
    int expandCluster(Point point, int clusterID);
    inline double calculateDistance(const Point& pointCore,
                                    const Point& pointTarget);

    int getTotalPointSize() { return pointSize; }
    int getMinimumClusterSize() { return minPoints; }
    int getEpsilonSize() { return eps; }
    const vector<Point>& getPoints() { return points; }
    const unsigned int getNClusters() const { return nClusters; }

   private:
    unsigned int nClusters;
    unsigned int minPoints;
    float eps;
    unsigned int pointSize;
    vector<Point> points;
};

#endif  // DBSCAN_H
