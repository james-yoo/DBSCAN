# :no_entry:[No more maintained]
# DBSCAN
[![C/C++ CI](https://github.com/amarrerod/DBSCAN/actions/workflows/c-cpp.yml/badge.svg?branch=master)](https://github.com/amarrerod/DBSCAN/actions/workflows/c-cpp.yml)

DBSCAN (Density-Based Spatial Clustering of Applications with Noise) is a data clustering algorithm proposed by Martin Ester, Hans-Peter Kriegel, Jörg Sander and Xiaowei Xu in 1996. The algorithm had implemented with pseudocode described in [wiki](https://en.wikipedia.org/wiki/DBSCAN), but it is not optimised.
  
# Example  
You can test this DBSCAN algorithm with example code(main.cpp) & sample data(benchmark_hepta.dat).
  
# Results
Clustering performance was tesed via clustering-benchmark dataset and real-world dataset.  

# Build
```
$ g++ main.cpp dbscan.cpp -o dbscan
```

## benchmark dataset  
Artificial dataset was used to test clustering algorithm which can be obtained [here](https://github.com/deric/clustering-benchmark). Following parameters were used:  
1. Minimum number of points: 4
2. Epsilon: 0.75
  
  
![dbscan_benchmark1](https://user-images.githubusercontent.com/23667624/38789657-4e5b7738-412b-11e8-9499-8fcde80fba2c.png)  
Source: Hepta (Total number of cluster: 7)
