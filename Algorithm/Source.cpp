#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <utility>
#include <string>
#include <map>
#include <sstream>
#include <time.h>

using std::vector;
using std::set;
using std::map;
using std::pair;
using std::make_pair;
using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::istringstream;
using std::endl;

struct TransmissionProp {
    vector<int> tree;
    vector<vector<int>> net;
    int s;

    TransmissionProp(vector<int> tree, vector<vector<int>> net, int s) {
        this->tree = tree;
        this->net = net;
        this->s = s;
    }
};

void leaves_random_color(vector<int>& shape, int k_num) {
    srand(time(NULL));
    for (unsigned int i = 0; i < shape.size(); ++i) {
        if (shape[i] == 0) {
            if (2 * i + 1 < shape.size()) {
                if (shape[2 * i + 1] == -1 && shape[2 * i + 2] == -1) {
                    shape[i] = rand() % k_num + 1;
                }
            }
            else {
                shape[i] = rand() % k_num + 1;
            }
        }
    }
}

vector<int> Generate_random_tree(int max_height, int k_num) {
    int shape_size = pow(2, max_height + 1) - 1;
    vector<int> tree_shape(shape_size, -1);
    tree_shape[0] = 0;
    srand(time(NULL));
    // int num_it = rand() % shape_size + 2;
    int num_it = pow(2, max_height);
    set<int> nodes;
    nodes.insert(0);

    for (int i = 0; i < num_it; ++i) {
        vector<int> nodes_vec(nodes.size(), 0);
        std::copy(nodes.begin(), nodes.end(), nodes_vec.begin());
        int index = nodes_vec[rand() % nodes.size()];
        if (index > shape_size / 2 - 2) {
            continue;
        }
        
        int l = 2 * index + 1;
        int r = 2 * index + 2;
        if (tree_shape[l] == 0 && tree_shape[r] == -1) {
            tree_shape[r] = 0;
            nodes.erase(index);
            nodes.insert(r);
        }
        else if (tree_shape[r] == 0 && tree_shape[l] == -1) {
            tree_shape[l] = 0;
            nodes.erase(index);
            nodes.insert(l);
        }
        else {
            int lr = rand() % 2 + 1;
            tree_shape[2 * index + lr] = 0;
            nodes.insert(2 * index + lr);
        }
    }
    leaves_random_color(tree_shape, k_num);
    return tree_shape;
}

vector<int> Merge_shapes(const vector<int>& left_shape, const vector<int>& right_shape) {
    vector<int> new_shape;
    for (unsigned int i = 0; i < left_shape.size(); ++i) {
        new_shape.push_back((left_shape[i] != 0) ? left_shape[i] : right_shape[i]);
    }
    return new_shape;
}

vector<vector<int>> Generate_all_transm_trees(const int root_id, vector<int>& shape) {
    vector<vector<int>> left_shapes, right_shapes, possible_shapes;

    unsigned int left_child_id = 2 * root_id + 1;
    unsigned int right_child_id = 2 * root_id + 2;

    if ((shape.size() <= left_child_id) || (shape[left_child_id] < 0 && shape[right_child_id] < 0)) {
        return vector<vector<int>>{ shape };
    }
    if (shape[left_child_id] < 0) { //when left child is empty
        possible_shapes = Generate_all_transm_trees(right_child_id, shape);
        for (unsigned int i = 0; i < possible_shapes.size(); ++i) {
            possible_shapes[i][root_id] = possible_shapes[i][right_child_id];
        }
        return possible_shapes;
    }
    if (shape[right_child_id] < 0) { //when right child is empty
        possible_shapes = Generate_all_transm_trees(left_child_id, shape);
        for (unsigned int i = 0; i < possible_shapes.size(); ++i) {
            possible_shapes[i][root_id] = possible_shapes[i][left_child_id];
        }
        return possible_shapes;
    }
    left_shapes = Generate_all_transm_trees(left_child_id, shape);
    right_shapes = Generate_all_transm_trees(right_child_id, shape);
  
    for (unsigned int i = 0; i < left_shapes.size(); ++i) {
        for (unsigned int j = 0; j < right_shapes.size(); ++j) {
            if (left_shapes[i][left_child_id] == right_shapes[j][right_child_id]) {
                left_shapes[i][root_id] = left_shapes[i][left_child_id];
                possible_shapes.push_back(Merge_shapes(left_shapes[i], right_shapes[j]));
            } else {
                left_shapes[i][root_id] = left_shapes[i][left_child_id];
                possible_shapes.push_back(Merge_shapes(left_shapes[i], right_shapes[j]));
                left_shapes[i][root_id] = right_shapes[j][right_child_id];
                possible_shapes.push_back(Merge_shapes(left_shapes[i], right_shapes[j]));
            }
        }
    }

    return possible_shapes;
}

bool Comp_by_arr_len(const vector<int>& a, const vector<int>& b) {
    return (a.size() > b.size());
}

int Get_longest_path_start_pos(const vector<int>& shape, const vector<int>& start_poses) {
    int max_path_len = 0, best_start_pos = start_poses[0];
    for (int start_pos : start_poses) {
        int cur_pos = start_pos;
        int cur_parent_pos = (start_pos + 1) / 2 - 1;
        if (shape[cur_parent_pos] != 0) {
            continue;
        }
        int path_len = 0;
        while (cur_pos != 0 && shape[cur_parent_pos] == 0) {
            cur_pos = cur_parent_pos;
            cur_parent_pos = (cur_pos + 1) / 2 - 1;
            ++path_len;
        }
        if (max_path_len < path_len) {
            max_path_len = path_len;
            best_start_pos = start_pos;
        }
    }
    return best_start_pos;
}

void Color_path(vector<int>& shape, const int start_pos) {
    int cur_pos = start_pos;
    int cur_parent_pos = (start_pos + 1) / 2 - 1;
    while (cur_pos != 0 && shape[cur_parent_pos] == 0) {
        shape[cur_parent_pos] = shape[cur_pos];
        cur_pos = cur_parent_pos;
        cur_parent_pos = (cur_pos + 1) / 2 - 1;
    }
}

vector<int> Build_heuristic_transm_tree(vector<int> shape) {
    int max_val = *std::max_element(shape.begin(), shape.end() - 1);
    vector<vector<int>> val_to_pos(max_val + 1, vector<int>());
    for (unsigned int i = 0; i < shape.size(); ++i) {
        if (shape[i] > 0) {
            val_to_pos[shape[i]].push_back(int(i));
        }
    }

    std::sort(val_to_pos.begin(), val_to_pos.end(), Comp_by_arr_len);
    for (unsigned int i = 0; i < val_to_pos.size(); ++i) {
        if (!val_to_pos[i].empty()) {
            vector<int> cur_poses = val_to_pos[i]; //
            int best_start_pos = Get_longest_path_start_pos(shape, cur_poses);
            Color_path(shape, best_start_pos);
        }
    }

    std::reverse(val_to_pos.begin(), val_to_pos.end());
    for (unsigned int i = 0; i < val_to_pos.size(); ++i) {
        for (int cur_pos : val_to_pos[i]) {
            Color_path(shape, cur_pos);
        }
    }
    return shape;
}

vector<vector<int>> Build_transm_network(vector<int> transm_tree) {
    int max_val = *std::max_element(transm_tree.begin(), transm_tree.end() - 1);
    vector<vector<int>> transm_network(max_val + 1, vector<int>());
    set<pair<int, int>> pairs;
    for (unsigned int i = 0; i < (transm_tree.size() - 1) / 2; ++i) {
        int left_child_id = 2 * i + 1;
        int right_child_id = 2 * i + 2;
        if (transm_tree[i] != -1) {
            if (transm_tree[left_child_id] != -1 && (transm_tree[left_child_id] != transm_tree[i])) {
                pairs.insert(make_pair(transm_tree[i], transm_tree[left_child_id]));
            }
            if (transm_tree[right_child_id] != -1 && (transm_tree[right_child_id] != transm_tree[i])) {
                pairs.insert(make_pair(transm_tree[i], transm_tree[right_child_id]));
            }
        }
    }
    while (!pairs.empty()) {
        pair<int, int> p = *pairs.begin();
        transm_network[p.first].push_back(p.second);
        transm_network[p.second].push_back(p.first);
        pairs.erase(p);
        pairs.erase(make_pair(p.second, p.first));
    }
    return transm_network;
}

int Calc_s_metric(vector<vector<int>> transm_network) { //first change in the new branch
    vector<int> deg_arr;
    for (auto net : transm_network) {
        if (!net.empty()) {
            deg_arr.push_back(net.size());
        }
    }
    int s_metric = 0;
    for (unsigned int i = 0; i < deg_arr.size(); ++i) {
        for (unsigned int j = i + 1; j < deg_arr.size(); ++j) {
            s_metric += deg_arr[i] * deg_arr[j];
        }
    }
    return s_metric;
}

TransmissionProp Get_max_s_net(vector<vector<int>> transm_trees) {
    int max_val = *std::max_element(transm_trees[0].begin(), transm_trees[0].end() - 1);
    vector<vector<int>> best_net;
    vector<int> best_tree;
    int max_s = 0;
    for (const auto& tree : transm_trees) {
        auto net = Build_transm_network(tree);
        int s = Calc_s_metric(net);
        if (s > max_s) {
            best_net = net;
            best_tree = tree;
            max_s = s;
        }
    }
    return TransmissionProp(best_tree, best_net, max_s);
}

void Save_trees(vector<vector<int>> shapes, string path) {
    ofstream fout(path);
    for (unsigned int i = 0; i < shapes.size(); ++i) {
        for (int el : shapes[i]) {
            fout << el << " ";
        }
        fout << endl;
    }
    fout.close();
}
vector<vector<int>> Read_trees(string path) {
    ifstream fin(path);
    string row;
    vector<vector<int>> shapes;
    while (getline(fin, row)) {
        int num;
        istringstream n_stream(row);
        vector<int> shape;
        while (n_stream >> num) {
            shape.push_back(num);
        }
        shapes.push_back(shape);
    }
    fin.close();
    return shapes;
}

void Save_transm_net(vector<vector<int>> transm_net, string path, int s = 0) {
    ofstream fout(path);
    if (s != 0) {
        fout << s << endl;
    }
    for (unsigned int i = 0; i < transm_net.size(); ++i) {
        if (!transm_net[i].empty()) {
            fout << i;
            for (int el: transm_net[i]) {
                fout << " " << el;
            }
            fout << endl;
        }
    }
    fout.close();
}

int main() {
    vector<int> simple_shape = Generate_random_tree(5, 4);
    Save_trees(vector<vector<int>>{ simple_shape }, "shape.txt");
    vector<int> shape = Read_trees("shape.txt")[0];
    // vector<int>{ 0, 0, 0, 0, 0, 4, 0, 1, 0, 0, 1, -1, -1, 0, 0, -1, -1, 1, 2, 3, 2, -1, -1, -1, -1, -1, -1, 1, 2, 3, 4};

    vector<vector<int>> possible_trees = Generate_all_transm_trees(0, shape);
    Save_trees(possible_trees, "possible_trees.txt");
    vector<vector<int>> trees = Read_trees("possible_trees.txt");
    TransmissionProp best_prop = Get_max_s_net(trees);
    Save_trees(vector<vector<int>>{ best_prop.tree }, "best_tree.txt");
    Save_transm_net(best_prop.net, "best_net.txt", best_prop.s);

    vector<int> heuristic_transm_tree = Build_heuristic_transm_tree(shape);
    Save_trees(vector<vector<int>>{heuristic_transm_tree}, "heuristic_tree.txt");
    vector<vector<int>> heuristic_transm_net = Build_transm_network(heuristic_transm_tree);
    int heuristic_s = Calc_s_metric(heuristic_transm_net);
    Save_transm_net(heuristic_transm_net, "heuristic_net.txt", heuristic_s);

    return 0;
}