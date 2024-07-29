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

#include <map>

/**
 * @brief Construct a new DBSCAN::DBSCAN object
 *
 * @param minPts
 * @param eps
 * @param points
 */
DBSCAN::DBSCAN(const unsigned int& minPts, const float& eps,
               const vector<Point>& points)
    : nClusters(0), m_minPoints(minPts), m_epsilon(eps) {
    m_pointSize = points.size();
    m_points = points;
}

/**
 * @brief Construct a new DBSCAN::DBSCAN object
 *
 * @param minPts
 * @param eps
 * @param points
 */
DBSCAN::DBSCAN(const unsigned int& minPts, const float& eps,
               const vector<coords>& coordinates)
    : nClusters(0), m_minPoints(minPts), m_epsilon(eps) {
    m_pointSize = coordinates.size();
    m_points.reserve(m_pointSize);
    for (auto& [x, y] : coordinates) {
        Point p;
        p.x = x;
        p.y = y;
        p.clusterID = UNCLASSIFIED;
        m_points.push_back(p);
    }
}

/**
 * @brief Method to run the DBSCAN algorithm
 *
 * @return int
 */
int DBSCAN::run() {
    int clusterID = 1;
    vector<Point>::iterator iter;
    for (iter = m_points.begin(); iter != m_points.end(); ++iter) {
        if (iter->clusterID == UNCLASSIFIED) {
            if (expandCluster(*iter, clusterID) != FAILURE) {
                clusterID += 1;
            }
        }
    }
    // Creating the clusters
    std::map<int, vector<coords>> clusters;

    for (unsigned int i = 1; i <= this->nClusters; i++) {
        m_clusters.push_back(Cluster(i));
    }
    // Filling the cluster
    for (const Point& p : m_points) {
        if (p.clusterID != UNCLASSIFIED) {
            if (clusters.find(p.clusterID) != clusters.end()) {
                clusters.insert({p.clusterID, {}});
            }
            clusters[p.clusterID].push_back({p.x, p.y});
        }
    }
    for (auto [id, points] : clusters) {
        this->m_clusters.push_back(Cluster(id, points));
    }
    this->nClusters = clusters.size();
    return nClusters;
}

/**
 * @brief
 *
 * @param point
 * @param clusterID
 * @return int
 */
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
 * @brief Calculates the cluster the point belongs to
 *
 * @param point
 * @return vector<int>
 */

vector<int> DBSCAN::calculateCluster(Point point) {
    int index = 0;
    vector<Point>::iterator iter;
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

/**
 * @brief Computes the mean radius of the clusters in the points
 *  It is defined as the mean distance from any city in a cluster to the cluster
 * centroid, averaged over all clusters
 * @return float
 */
float DBSCAN::radiusOfClusters() {
    auto radius = 0.0f;
    for (auto& c : m_clusters) {
        auto cr = c.getRadius();
        radius += cr;
    }
    return radius / m_clusters.size();
}