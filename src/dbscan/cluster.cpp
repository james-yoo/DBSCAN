

#include "cluster.h"

#include <math.h>
/**
 * @brief Construct a new Cluster:: Cluster object
 *
 * @param id
 */
Cluster::Cluster(const unsigned int& id) : id(id), radius(-1.0), points({}) {
    centroid = nullCoords;
}

/**
 * @brief Construct a new Cluster:: Cluster object
 *
 * @param id
 * @param coordinates
 */
Cluster::Cluster(const unsigned int& id, const std::vector<coords>& coordinates)
    : id(id), radius(-1.0), points(coordinates) {
    centroid = nullCoords;
}

/**
 * @brief Includes new points into the cluster
 *
 * @param pts
 */
void Cluster::addPoints(const std::vector<coords>& pts) {
    for (const coords& p : pts) {
        points.push_back(p);
    }
}

/**
 * @brief Method to update the cluster and compute the Centroid and Radius
 *
 */
void Cluster::update() {
    computeCentroid();
    computeRadius();
}
/**
 * @brief Returns the radius of the cluster
 *
 * @return float
 */
const float Cluster::getRadius() {
    if (radius == -1.0) {
        radius = computeRadius();
    }
    return radius;
}

/**
 * @brief Method to calculate the radius of the cluster. The radius is defined
 * as the mean distance from each city to the centroid
 * Definition extracted from:
 *
 * Smith-Miles, K., Van Hemert, J., & Lim, X. Y. (2010). Understanding TSP
 * difficulty by learning from evolved instances. Lecture Notes in Computer
 * Science (Including Subseries Lecture Notes in Artificial Intelligence and
 * Lecture Notes in Bioinformatics), 6073 LNCS, 266–280.
 * https://doi.org/10.1007/978-3-642-13800-3_29
 * @return float
 */
float Cluster::computeRadius() {
    radius = 0.0f;
    if (centroid == nullCoords) {
        // Making sure the centroid is already calculated
        computeCentroid();
    }
    for (auto [x, y] : points) {
        radius += sqrt(((x - centroid.first) * (x - centroid.first)) +
                       ((y - centroid.second) * (y - centroid.second)));
    }
    radius /= points.size();
    return radius;
}

/**
 * @brief Returns the centroid of the cluster
 *
 * @return const coords
 */
const coords Cluster::getCentroid() {
    if (centroid == nullCoords) {
        centroid = computeCentroid();
    }
    return centroid;
}

/**
 * @brief Method to calculate the centroid of the cluster.
 *
 * @return coords
 */
coords Cluster::computeCentroid() {
    auto cX = 0.0f;
    auto cY = 0.0f;
    for (auto [x, y] : points) {
        cX += x;
        cY += y;
    }
    cX /= points.size();
    cY /= points.size();
    centroid = {cX, cY};
    return centroid;
}

/**
 * @brief Prints the information of a Cluster object
 *
 * @param os
 * @param c
 * @return std::ostream&
 */
std::ostream& operator<<(std::ostream& os, const Cluster& c) {
    os << "Cluster with id: " << c.id << std::endl;
    os << "Radius: " << c.radius << std::endl;
    os << "Centroid: (" << c.centroid.first << ", " << c.centroid.second << ")"
       << std::endl;
    os << "Number of points: " << c.points.size() << std::endl;
    return os;
}