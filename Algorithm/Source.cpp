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

typedef vector<int> arr;
typedef vector<vector<int>> matrix;

struct Max_trees {
    arr max_v_deg_tree;
    arr max_n_term_edge_tree;

    Max_trees(arr max_v_deg_tree, arr max_n_term_edge_tree) {
        this->max_v_deg_tree = max_v_deg_tree;
        this->max_n_term_edge_tree = max_n_term_edge_tree;
    }
};

void leaves_random_color(arr& shape, int k_num) {
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

arr Generate_random_tree(int max_height, int k_num) {
    int shape_size = pow(2, max_height + 1) - 1;
    arr tree_shape(shape_size, -1);
    tree_shape[0] = 0;
    srand(time(NULL));
    // int num_it = rand() % shape_size + 2;
    int num_it = pow(2, max_height);
    set<int> nodes;
    nodes.insert(0);

    for (int i = 0; i < num_it; ++i) {
        arr nodes_vec(nodes.size(), 0);
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

arr Merge_shapes(const arr& left_shape, const arr& right_shape) {
    arr new_shape;
    for (unsigned int i = 0; i < left_shape.size(); ++i) {
        new_shape.push_back((left_shape[i] != 0) ? left_shape[i] : right_shape[i]);
    }
    return new_shape;
}

matrix Generate_all_transm_trees(const int root_id, arr& shape) {
    matrix left_shapes, right_shapes, possible_shapes;

    unsigned int left_child_id = 2 * root_id + 1;
    unsigned int right_child_id = 2 * root_id + 2;

    if ((shape.size() <= left_child_id) || (shape[left_child_id] < 0 && shape[right_child_id] < 0)) {
        return matrix{ shape };
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

matrix Build_transm_network(arr transm_tree) {
    int max_val = *std::max_element(transm_tree.begin(), transm_tree.end());
    matrix transm_network(max_val + 1, arr());
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

bool Check_connection(matrix transm_net) {
    vector<bool> visited(transm_net.size(), false);
    arr stack;
    stack.push_back(0);
    while (!stack.empty()) {
        int cur_v = stack.back();
        visited[cur_v] = true;
        stack.pop_back();
        for (int v : transm_net[cur_v]) {
            if (!visited[v]) {
                stack.push_back(v);
            }
        }
    }
    return std::all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}

int Num_of_terminal_edge(matrix transm_net) {
    int num = 0;
    for (const arr& neighbors : transm_net) {
        num += neighbors.size() == 1 ? 1 : 0;
    }
    return num;
}

int Max_vertex_degree(matrix transm_net) {
    return (*std::max_element(transm_net.begin(), transm_net.end(),
        [](const arr& l_net,const arr& r_net) { return l_net.size() < r_net.size(); })).size();
}

Max_trees Get_max_trees(matrix transm_trees) {
    arr max_v_deg_tree = *std::max_element(transm_trees.begin(), transm_trees.end(),
        [](const arr& tree_l, const arr& tree_r)
        {return Max_vertex_degree(Build_transm_network(tree_l)) < Max_vertex_degree(Build_transm_network(tree_r)); });

    arr max_n_term_edge_tree = *std::max_element(transm_trees.begin(), transm_trees.end(),
        [](const arr& tree_l, const arr& tree_r)
        { return Num_of_terminal_edge(Build_transm_network(tree_l)) < Num_of_terminal_edge(Build_transm_network(tree_r)); });

    return Max_trees(max_v_deg_tree, max_n_term_edge_tree);
}

void Save_trees(matrix shapes, string path) {
    ofstream fout(path);
    for (unsigned int i = 0; i < shapes.size(); ++i) {
        for (int el : shapes[i]) {
            fout << el << " ";
        }
        fout << endl;
    }
    fout.close();
}
matrix Read_trees(string path) {
    ifstream fin(path);
    string row;
    matrix shapes;
    while (getline(fin, row)) {
        int num;
        istringstream n_stream(row);
        arr shape;
        while (n_stream >> num) {
            shape.push_back(num);
        }
        shapes.push_back(shape);
    }
    fin.close();
    return shapes;
}

void Save_transm_net(matrix transm_net, string path, int param = 0) {
    ofstream fout(path);
    if (param != 0) {
        fout << param << endl;
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
    arr simple_shape = Generate_random_tree(5, 4);
    Save_trees(matrix{ simple_shape }, "shape.txt");
    
    arr shape = Read_trees("shape.txt")[0];
    // array{ 0, 0, 0, 0, 0, 4, 0, 1, 0, 0, 1, -1, -1, 0, 0, -1, -1, 1, 2, 3, 2, -1, -1, -1, -1, -1, -1, 1, 2, 3, 4};
    matrix possible_trees = Generate_all_transm_trees(0, shape);
    Save_trees(possible_trees, "possible_trees.txt");

    matrix trees = Read_trees("possible_trees.txt");
    Max_trees max_trees = Get_max_trees(trees);

    arr max_v_deg_tree = max_trees.max_v_deg_tree;
    matrix max_v_deg_net = Build_transm_network(max_v_deg_tree);
    int max_v_deg = Max_vertex_degree(max_v_deg_net);
    Save_trees(matrix{ max_v_deg_tree }, "max_v_deg_tree.txt");
    Save_transm_net(max_v_deg_net, "max_v_deg_net.txt", max_v_deg);

    arr max_n_term_edge_tree = max_trees.max_n_term_edge_tree;
    matrix max_n_term_edge_net = Build_transm_network(max_n_term_edge_tree);
    int max_n_term_edge = Num_of_terminal_edge(max_n_term_edge_net);
    Save_trees(matrix{ max_n_term_edge_tree }, "max_n_term_edge_tree.txt");
    Save_transm_net(max_n_term_edge_net, "max_n_term_edge_net.txt", max_n_term_edge);

    return 0;
}