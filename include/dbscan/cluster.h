

#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <utility>
#include <vector>

using coords = std::pair<float, float>;

class Cluster {
   public:
    Cluster() = default;
    explicit Cluster(const unsigned int& id);
    Cluster(const unsigned int& id, const std::vector<coords>& coordinates);

    const int getID() const { return id; }
    const float getRadius();
    const coords getCentroid();

    void addPoint(const coords& p) { points.push_back(p); }
    void addPoints(const std::vector<coords>& points);
    std::vector<coords> getPoints() const { return points; }

    void update();
    bool empty() const { return points.empty(); }

    friend std::ostream& operator<<(std::ostream& os, const Cluster& c);

   private:
    float computeRadius();
    coords computeCentroid();

   private:
    unsigned int id;
    float radius;
    coords centroid;
    std::vector<coords> points;

   protected:
    const coords nullCoords = {-1.0, -1.0};
};

#endif  //