#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <vector>
using namespace std;
class SearchAlgorithms {
public:
    int expontential(vector<int>& arr, int l, int r, int x);
    int binary(vector<int>& arr, int l, int r, int x);
    int linear(vector<int>& arr, int x);
    bool ternary(vector<int>& arr, int x);

};
#endif
