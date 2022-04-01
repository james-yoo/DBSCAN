#ifndef DBSCAN_H
#define DBSCAN_H

#include <cmath>
#include <vector>

#define UNCLASSIFIED -1
#define CORE_POINT 1
#define BORDER_POINT 2
#define NOISE -2
#define SUCCESS 0
#define FAILURE -3

using namespace std;

template <typename T = float>
struct Point {
    T x, y, z;      // X, Y, Z position
    int clusterID;  // clustered ID
};

template <typename T = float>
class DBSCAN {
   public:
    DBSCAN(const unsigned int& minPts, const float& eps,
           const vector<Point<T>>& points);
    ~DBSCAN() = default;

    int run();
    vector<int> calculateCluster(Point<T> point);
    int expandCluster(Point<T> point, int clusterID);
    inline double calculateDistance(const Point<T>& pointCore,
                                    const Point<T>& pointTarget);

    int getTotalPointSize() { return m_pointSize; }
    int getMinimumClusterSize() { return m_minPoints; }
    int getEpsilonSize() { return m_epsilon; }
    const vector<Point<T>>& getPoints() { return m_points; }

   private:
    unsigned int m_minPoints;
    float m_epsilon;
    unsigned int m_pointSize;
    vector<Point<T>> m_points;
};

template <typename T>
DBSCAN<T>::DBSCAN(const unsigned int& minPts, const float& eps,
                  const vector<Point<T>>& points)
    : m_minPoints(minPts), m_epsilon(eps) {
    m_pointSize = points.size();
    m_points = points;
}

template <typename T>
int DBSCAN<T>::run() {
    int clusterID = 1;
    vector<Point<T>>::iterator iter;
    for (iter = m_points.begin(); iter != m_points.end(); ++iter) {
        if (iter->clusterID == UNCLASSIFIED) {
            if (expandCluster(*iter, clusterID) != FAILURE) {
                clusterID += 1;
            }
        }
    }
    return 0;
}

template <typename T>
int DBSCAN<T>::expandCluster(Point<T> point, int clusterID) {
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

        for (vector<int>::size_type i = 0, n = clusterSeeds.size(); i < n;
             ++i) {
            vector<int> clusterNeighors =
                calculateCluster(m_points.at(clusterSeeds[i]));

            if (clusterNeighors.size() >= m_minPoints) {
                vector<int>::iterator iterNeighors;
                for (iterNeighors = clusterNeighors.begin();
                     iterNeighors != clusterNeighors.end(); ++iterNeighors) {
                    if (m_points.at(*iterNeighors).clusterID == UNCLASSIFIED ||
                        m_points.at(*iterNeighors).clusterID == NOISE) {
                        if (m_points.at(*iterNeighors).clusterID ==
                            UNCLASSIFIED) {
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
template <typename T>
vector<int> DBSCAN<T>::calculateCluster(Point<T> point) {
    int index = 0;
    vector<Point<T>>::iterator iter;
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
template <typename T>
inline double DBSCAN<T>::calculateDistance(const Point<T>& pointCore,
                                           const Point<T>& pointTarget) {
    return ((pointCore.x - pointTarget.x) * (pointCore.x - pointTarget.x)) +
           ((pointCore.y - pointTarget.y) * (pointCore.y - pointTarget.y)) +
           ((pointCore.z - pointTarget.z) * (pointCore.z - pointTarget.z));
}

#endif  // DBSCAN_H
