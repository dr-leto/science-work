#pragma once
#include "Header.h"

set<int> Get_vertices_to_recolor(const vector<int>& tree) {
    set <int> vertices_to_recolor;
    for (int i = tree.size() - 1; i > -1; --i) {
        unsigned int left = 2 * i + 1;
        unsigned int right = 2 * i + 2;
        if (right < tree.size() && tree[left] > 0 && tree[right] > 0 && tree[left] != tree[right]) {
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

pair<vector<int>, int> Color_random_vertex(const vector<int>& tree) {
    vector<int> new_tree(tree);
    set<int> v_rec = Get_vertices_to_recolor(tree);
    int cur_v = Get_random_vertex(v_rec);
    int first_recolored = cur_v;
    new_tree[cur_v] = (new_tree[cur_v] == new_tree[cur_v * 2 + 1]) ? new_tree[cur_v * 2 + 2] : new_tree[cur_v * 2 + 1];
    while (cur_v != 0) {
        int cur_p = (cur_v - 1) / 2;

        int left = cur_p * 2 + 1;
        int right = cur_p * 2 + 2;
        if (new_tree[cur_p] != new_tree[left] && new_tree[cur_p] != new_tree[right]) {
            new_tree[cur_p] = (rand() % 2 == 0) ? new_tree[left] : new_tree[right];
        }
        else {
            break;
        }
        cur_v = cur_p;
    }
    return std::make_pair(new_tree, first_recolored);
}

vector<vector<int>> Build_transm_network(const vector<int>& transm_tree) {
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
        pair<vector<int>, int> new_tree_v = Color_random_vertex(old_tree);
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