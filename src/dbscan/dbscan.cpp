/**
 * @file dbscan.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-04-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "dbscan.h"

DBSCAN::DBSCAN(const unsigned int& minPts, const float& eps,
               const vector<Point>& points)
    : nClusters(0), m_minPoints(minPts), m_epsilon(eps) {
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
        std::cout << *iter << std::endl;
    }
    this->nClusters = clusterID;
    return nClusters;
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
                m_points.at(*iterSeeds).y == point.y) {
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
           ((pointCore.y - pointTarget.y) * (pointCore.y - pointTarget.y));
}