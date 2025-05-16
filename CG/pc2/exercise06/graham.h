#ifndef GRAHAM_H
#define GRAHAM_H

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
vector<vector<T>> interior_points_removal(vector<vector<T>> const &vertices);

template <typename T>
vector<vector<T>> graham(vector<vector<T>> const &vertices,
                         bool withInteriorPointsRemoval);

#endif // GRAHAM_H