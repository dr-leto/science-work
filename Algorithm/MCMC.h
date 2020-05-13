#pragma once
#include "Header.h"
#include "Tree_generation.h"

set<int> Get_v_to_recol(const Graph& tree) {
    set <int> vertices_to_recolor;
    int cur_v = 0;
    for (vec neighboors : tree.adj_list) {
        neighboors.erase(neighboors.begin());
        set<int> unq_child_cols;
        for (int child_v: neighboors) {
            unq_child_cols.insert(tree.ind_to_color[child_v]);
        }
        if (unq_child_cols.size() > 1) {
            vertices_to_recolor.insert(cur_v);
        }
        ++cur_v;
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
    int first_v = cur_v;
    int child_v, parent_v = tree.adj_list[cur_v][0];

    do {
        set<int> child_cols;
        for (unsigned int i = 1; i < tree.adj_list[cur_v].size(); ++i) {
            child_cols.insert(tree.ind_to_color[tree.adj_list[cur_v][i]]);
        }
        int cur_col = tree.ind_to_color[cur_v];
        if (cur_v == first_v) {
            child_cols.erase(cur_col);
        }
        if (child_cols.find(cur_col) == child_cols.end()) { // make recoloring stronger, recolor parend vertex even if it is not necessary
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
    for (unsigned int i = 0; i < tree.adj_list.size(); ++i) {
        int cur_col = tree.ind_to_color[tree.adj_list[i][0]];
        for (unsigned int j = 1; j < tree.adj_list[i].size(); ++j) {
            int child_col = tree.ind_to_color[tree.adj_list[i][j]];
            if (cur_col != child_col) {
                pairs.insert(make_pair(cur_col, child_col));
            }
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

int Calc_s_metric(vec_vec transm_network) {
    vec deg_arr;
    for (const vec& net : transm_network) {
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

Graph MCMC_run(Graph& tree, int n, vec& s_metrics, vec& rec_vs) {
    Shape_random_color(tree, 0);
    Graph min_s_tree(tree);
    int min_s = INT_MAX;
    Graph base_tree(tree);
    for (int i = 0; i < n; ++i) {
        if (i % (n / 100) == 0) {
            printf("%d perc completed \n", i * 100 / n);
        }
        vec_vec old_t_net = Build_t_net(base_tree);
        int old_s = Calc_s_metric(old_t_net);
        s_metrics.push_back(old_s);

        Graph new_tree(base_tree);
        int rec_v = Recol_rand_v(new_tree);
        vec_vec new_t_net = Build_t_net(new_tree);
        rec_vs.push_back(rec_v);

        int new_s = Calc_s_metric(new_t_net);
        //if (rand() % (old_s + new_s) > old_s) {
        if ((new_s < old_s) || (rand() % 100 == 0)) {
            // add some loss if there are cycles in t_net
            base_tree = new_tree;
            if (new_s < min_s) {
                min_s_tree = new_tree;
                min_s = old_s;
            }
        }
    }
    return min_s_tree;
}