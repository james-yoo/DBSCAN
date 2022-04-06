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
               const vector<Point>& p)
    : minPoints(minPts), eps(eps) {
    pointSize = points.size();
    this->points = p;
}

int DBSCAN::run() {
    int clusterID = 1;
    typename vector<Point>::iterator iter;
    for (iter = points.begin(); iter != points.end(); ++iter) {
        if (iter->clusterID == UNCLASSIFIED) {
            if (expandCluster(*iter, clusterID) != FAILURE) {
                clusterID += 1;
            }
        }
    }
    nClusters = clusterID;
    return nClusters;
}

int DBSCAN::expandCluster(Point point, int clusterID) {
    vector<int> clusterSeeds = calculateCluster(point);

    if (clusterSeeds.size() < minPoints) {
        point.clusterID = NOISE;
        return FAILURE;
    } else {
        int index = 0, indexCorePoint = 0;
        vector<int>::iterator iterSeeds;
        for (iterSeeds = clusterSeeds.begin(); iterSeeds != clusterSeeds.end();
             ++iterSeeds) {
            points.at(*iterSeeds).clusterID = clusterID;
            if (points.at(*iterSeeds).x == point.x &&
                points.at(*iterSeeds).y == point.y) {
                indexCorePoint = index;
            }
            ++index;
        }
        clusterSeeds.erase(clusterSeeds.begin() + indexCorePoint);

        for (vector<int>::size_type i = 0, n = clusterSeeds.size(); i < n;
             ++i) {
            vector<int> clusterNeighors =
                calculateCluster(points.at(clusterSeeds[i]));

            if (clusterNeighors.size() >= minPoints) {
                vector<int>::iterator iterNeighors;
                for (iterNeighors = clusterNeighors.begin();
                     iterNeighors != clusterNeighors.end(); ++iterNeighors) {
                    if (points.at(*iterNeighors).clusterID == UNCLASSIFIED ||
                        points.at(*iterNeighors).clusterID == NOISE) {
                        if (points.at(*iterNeighors).clusterID ==
                            UNCLASSIFIED) {
                            clusterSeeds.push_back(*iterNeighors);
                            n = clusterSeeds.size();
                        }
                        points.at(*iterNeighors).clusterID = clusterID;
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
    for (iter = points.begin(); iter != points.end(); ++iter) {
        if (calculateDistance(point, *iter) <= eps) {
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