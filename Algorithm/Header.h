#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <utility>
#include <string>
#include <map>
#include <sstream>
#include <time.h>


using std::vector;
using std::pair;
using std::make_pair;
using std::set;
using std::map;
using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::istringstream;
using std::endl;
using std::advance;


typedef vector<int> vec;
typedef vector<vec> vec_vec;
struct Graph {
    vec_vec adj_list;
    vec ind_to_color;

    Graph() = default;
    Graph(vec_vec list, vec itc) {
        adj_list = list;
        ind_to_color = itc;
    }
};