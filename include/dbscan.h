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
  float x, y, z;  // X, Y, Z position
  int clusterID;  // clustered ID
  Point_(const float& _x, const float& _y, const float& _z, const int& _c)
      : x(_x), y(_y), z(_z), clusterID(_c) {}

  friend ostream& operator<<(std::ostream& os, const Point_& p) {
    return os << "(" << p.x << ", " << p.y << ", " << p.z << ") - "
              << p.clusterID;
  }
} Point;

using coords = std::pair<float, float>;

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

  int getTotalPointSize() { return m_pointSize; }
  int getMinimumClusterSize() { return m_minPoints; }
  int getEpsilonSize() { return m_epsilon; }
  const vector<Point>& getPoints() { return m_points; }

 private:
  unsigned int m_minPoints;
  float m_epsilon;
  unsigned int m_pointSize;
  vector<Point> m_points;
};

DBSCAN::DBSCAN(const unsigned int& minPts, const float& eps,
               const vector<Point>& points)
    : m_minPoints(minPts), m_epsilon(eps) {
  m_pointSize = points.size();
  m_points = points;
}

int DBSCAN::run() {
  int clusterID = 1;
  typename vector<Point>::iterator iter;
  for (iter = m_points.begin(); iter != m_points.end(); ++iter) {
    if (iter->clusterID == UNCLASSIFIED) {
      if (expandCluster(*iter, clusterID) != FAILURE) {
        clusterID += 1;
      }
    }
  }
  return 0;
}

int DBSCAN::expandCluster(Point point, int clusterID) {
  vector<int> clusterSeeds = calculateCluster(point);

  if (clusterSeeds.size() < m_minPoints) {
    point.clusterID = NOISE;
    return FAILURE;
  } else {
    int index = 0, indexCorePoint = 0;
    vector<int>::iterator iterSeeds;
    for (iterSeeds = clusterSeeds.begin(); iterSeeds != clusterSeeds.end();
         ++iterSeeds) {
      m_points.at(*iterSeeds).clusterID = clusterID;
      if (m_points.at(*iterSeeds).x == point.x &&
          m_points.at(*iterSeeds).y == point.y &&
          m_points.at(*iterSeeds).z == point.z) {
        indexCorePoint = index;
      }
      ++index;
    }
    clusterSeeds.erase(clusterSeeds.begin() + indexCorePoint);

    for (vector<int>::size_type i = 0, n = clusterSeeds.size(); i < n; ++i) {
      vector<int> clusterNeighors =
          calculateCluster(m_points.at(clusterSeeds[i]));

      if (clusterNeighors.size() >= m_minPoints) {
        vector<int>::iterator iterNeighors;
        for (iterNeighors = clusterNeighors.begin();
             iterNeighors != clusterNeighors.end(); ++iterNeighors) {
          if (m_points.at(*iterNeighors).clusterID == UNCLASSIFIED ||
              m_points.at(*iterNeighors).clusterID == NOISE) {
            if (m_points.at(*iterNeighors).clusterID == UNCLASSIFIED) {
              clusterSeeds.push_back(*iterNeighors);
              n = clusterSeeds.size();
            }
            m_points.at(*iterNeighors).clusterID = clusterID;
          }
        }
      }
    }

    return SUCCESS;
  }
}

/**
 * @brief Calculates the clusters in the points
 *
 * @param point
 * @return vector<int>
 */

vector<int> DBSCAN::calculateCluster(Point point) {
  int index = 0;
  typename vector<Point>::iterator iter;
  vector<int> clusterIndex;
  for (iter = m_points.begin(); iter != m_points.end(); ++iter) {
    if (calculateDistance(point, *iter) <= m_epsilon) {
      clusterIndex.push_back(index);
    }
    index++;
  }
  return clusterIndex;
}

/**
 * @brief Calculates the distance between two 3d points
 *
 * @param pointCore
 * @param pointTarget
 * @return double
 */

inline double DBSCAN::calculateDistance(const Point& pointCore,
                                        const Point& pointTarget) {
  return ((pointCore.x - pointTarget.x) * (pointCore.x - pointTarget.x)) +
         ((pointCore.y - pointTarget.y) * (pointCore.y - pointTarget.y)) +
         ((pointCore.z - pointTarget.z) * (pointCore.z - pointTarget.z));
}

#endif  // DBSCAN_H
