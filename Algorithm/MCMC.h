#pragma once
#include "Header.h"

set<int> Get_vertices_to_recolor(const Graph& tree) {
    set <int> vertices_to_recolor;

    for (unsigned int i = 0; i < tree.adj_list.size(); ++i) {
        if (tree.adj_list[i].size() == 3
            && tree.ind_to_color[tree.adj_list[i][1]] != tree.ind_to_color[tree.adj_list[i][2]]) {
            vertices_to_recolor.insert(i);
        }
    }
    return vertices_to_recolor;
}

int Get_random_vertex(const set<int>& vertices) {
    vector<int> v_vec(vertices.size(), 0);
    std::copy(vertices.begin(), vertices.end(), v_vec.begin());
    return v_vec[rand() % vertices.size()];
}

int Recolor_random_vertex(Graph& tree) {
    set<int> v_rec = Get_vertices_to_recolor(tree);
    int cur_v = Get_random_vertex(v_rec);
    int first_v = cur_v;

    int left_col = tree.ind_to_color[tree.adj_list[cur_v][1]];
    int right_col = tree.ind_to_color[tree.adj_list[cur_v][2]];
    int cur_col = tree.ind_to_color[cur_v];
    tree.ind_to_color[cur_v] = (cur_col != left_col) ? left_col : right_col;
    while(cur_v != 0) {
        int parent_v = tree.adj_list[cur_v][0];
        int parent_col = tree.ind_to_color[parent_v];

        if (tree.adj_list[parent_v].size() == 2) {
            tree.ind_to_color[parent_v] = cur_col;
        } else {
            int neighboor_v = (tree.adj_list[parent_v][1] != cur_v) ? tree.adj_list[parent_v][1] : tree.adj_list[parent_v][2];
            int neighboor_col = tree.ind_to_color[neighboor_v];
            if (parent_col != cur_col && parent_col != neighboor_col) {
                tree.ind_to_color[parent_v] = (rand() % 2 == 0) ? cur_col : neighboor_col;
            } else {
                break;
            }
        }
        cur_v = parent_v;
        cur_col = parent_col;
    }
    return first_v;
}

vector<vector<int>> Build_transm_network(const Graph& tree) {
    int max_val = *std::max_element(tree.ind_to_color.begin(), tree.ind_to_color.end() - 1);
    vec_vec t_net(max_val + 1, vector<int>()); // we assume that each of n colors match only one number from [1, n]
    set<pair<int, int>> pairs;
    for (unsigned int i = 1; i < tree.adj_list.size(); ++i) {
        int parent_v = tree.adj_list[i][0];
        int parent_col = tree.ind_to_color[parent_v];
        if (tree.ind_to_color[i] != parent_col) {
            pairs.insert(make_pair(tree.ind_to_color[i], parent_col));
        }
    }
    while (!pairs.empty()) {
        pair<int, int> p = *pairs.begin();
        t_net[p.first].push_back(p.second);
        t_net[p.second].push_back(p.first);
        pairs.erase(p);
        pairs.erase(make_pair(p.second, p.first));
    }
    return t_net;
}

int Calc_s_metric(vector<vector<int>> transm_network) {
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

vector<int> MCMC_run(const vector<int>& tree, int n, vector<int>& s_metrics, vector<int>& recolored_vertices) {
    vector<int> old_tree(tree);
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        pair<vector<int>, int> new_tree_v = Recolor_random_vertex(old_tree);
        recolored_vertices.push_back(new_tree_v.second);
        vector<vector<int>> old_trans = Build_transm_network(old_tree);
        int old_s = Calc_s_metric(old_trans);
        s_metrics.push_back(old_s);
        vector<vector<int>> new_trans = Build_transm_network(new_tree_v.first);
        int new_s = Calc_s_metric(new_trans);
        if (rand() % (old_s + new_s) > old_s) {
            old_tree = new_tree_v.first;
        }
    }
    return old_tree;
}