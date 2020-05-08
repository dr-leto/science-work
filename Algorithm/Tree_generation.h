#pragma once

#include "Header.h"

void Arr_to_graph(const vec& tree, int cur_v, int prev_it, Graph& new_tree) {
    int left = 2 * cur_v + 1;
    int right = 2 * cur_v + 2;
    int cur_it = new_tree.adj_list.size();

    if (cur_v > tree.size() - 1 || tree[cur_v] < 0) {
        return;
    }
    new_tree.adj_list.push_back(vec({ prev_it }));
    if (cur_v != 0) {
        new_tree.adj_list[prev_it].push_back(cur_it);
    }
    new_tree.ind_to_color.push_back(tree[cur_v]);
    Arr_to_graph(tree, left, cur_it, new_tree);
    Arr_to_graph(tree, right, cur_it, new_tree);
}

vec_vec Graph_to_matrix(const Graph& list) {
    int n = list.adj_list.size();
    vec_vec matrix(n, vec(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int cur_v : list.adj_list[i]) {
            matrix[i][cur_v] = list.ind_to_color[cur_v];
        }
    }
    matrix[0][0] = 0; // it's often that root tree has loop edge, just to remove it.
    return matrix;
}

void Leaves_random_color(Graph& shape, int col_num) {
    for (unsigned int i = 1; i < shape.adj_list.size(); ++i) {
        if (shape.adj_list[i].size() == 1) { // if vertex is not a root and has only parent as neighboor => it is a leaf
            shape.ind_to_color[i] = rand() % col_num + 1;
        }
    }
}

Graph Generate_random_shape(int max_height, int col_num) {
    int shape_size = pow(2, max_height + 1) - 1;
    vector<int> tree_shape(shape_size, -1);
    tree_shape[0] = 0;
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
    Graph new_shape;
    Arr_to_graph(tree_shape, 0, 0, new_shape);
    Leaves_random_color(new_shape, col_num);
    return new_shape;
}

int Shape_random_color(Graph& shape, int cur_v) {
    switch (shape.adj_list[cur_v].size()) {
    case 2:
    {
        int col = Shape_random_color(shape, shape.adj_list[cur_v][1]);
        shape.ind_to_color[cur_v] = col;
    }
    break;
    case 3:
    {
        int l_col = Shape_random_color(shape, shape.adj_list[cur_v][1]);
        int r_col = Shape_random_color(shape, shape.adj_list[cur_v][2]);
        shape.ind_to_color[cur_v] = (rand() % 2 == 0) ? l_col : r_col;
    }
    break;
    }
    return shape.ind_to_color[cur_v];
}