#pragma once
#include "Header.h"
#include "Tree_generation.h"
#include "Utils.h"

set<int> Get_v_to_recol(const Graph& tree) {
    set <int> vertices_to_recolor;
    int i = 0;
    for (vec neighboors : tree.adj_list) {
        neighboors.erase(neighboors.begin());
        set<int> unq_child_cols;
        for (int child_v : neighboors) {
            unq_child_cols.insert(tree.ind_to_color[child_v]);
        }
        if (unq_child_cols.size() > 1) {
            vertices_to_recolor.insert(i);
        }
        ++i;
    }
    return vertices_to_recolor;
}

int Get_rand_v(const set<int>& vertices) {
    vector<int> v_vec(vertices.size(), 0);
    std::copy(vertices.begin(), vertices.end(), v_vec.begin());
    return v_vec[rand() % vertices.size()];
}

int Recol_rand_v(Graph& tree) {
    set<int> rec_vs = Get_v_to_recol(tree);
    int cur_v = Get_rand_v(rec_vs);
    int first_v = cur_v, child_v, parent_v = tree.adj_list[cur_v][0];

    do {
        set<int> child_cols;
        for (unsigned int i = 1; i < tree.adj_list[cur_v].size(); ++i) {
            child_cols.insert(tree.ind_to_color[tree.adj_list[cur_v][i]]);
        }
        int cur_col = tree.ind_to_color[cur_v];
        if (cur_v == first_v) {
            child_cols.erase(cur_col);
        }
        if (child_cols.find(cur_col) == child_cols.end()) {
            int new_col = Get_rand_v(child_cols);
            tree.ind_to_color[cur_v] = new_col;
        } else {
            break;
        }
        child_v = cur_v;
        cur_v = parent_v;
        parent_v = tree.adj_list[cur_v][0];
    } while (child_v != 0); // same as child_v != parent_v
    return first_v;
}

vec_vec Build_t_net(const Graph& tree) {
    int max_col = *std::max_element(tree.ind_to_color.begin(), tree.ind_to_color.end() - 1);
    vec_vec t_net(max_col + 1, vector<int>()); // we assume that each of n colors match only one number from [1, n]
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

int Calc_s_metric(vec_vec t_net) {
    vec deg_arr;
    for (const vec& net : t_net) {
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

Graph MCMC_run(Graph& tree, int n, vec& s_metrics) {
    Shape_random_color(tree, 0);
    Graph base_tree(tree);
    vec_vec base_t_net = Build_t_net(base_tree);
    int base_s = Calc_s_metric(base_t_net);
    int base_edges_cnt = 0;
    for (const vec& neighbor : base_t_net) {
        base_edges_cnt += neighbor.size();
    }
    base_edges_cnt /= 2;
    s_metrics.push_back(base_s);

    for (int i = 0; i < n; ++i) {
        print_perc(i, n);

        Graph new_tree(base_tree);
        int rec_v = Recol_rand_v(new_tree);
        vec_vec new_t_net = Build_t_net(new_tree);
        int new_s = Calc_s_metric(new_t_net);
        int new_edges_cnt = 0;
        for (const vec& neighbor : new_t_net) {
            new_edges_cnt += neighbor.size();
        }
        new_edges_cnt /= 2;

        if (new_edges_cnt < base_edges_cnt || new_s > base_s || rand() % 100 == 0) {
            base_tree = new_tree;
            base_t_net = new_t_net;
            base_s = new_s;
            base_edges_cnt = new_edges_cnt;
        }
    }
    return base_tree;
}