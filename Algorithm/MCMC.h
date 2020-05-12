#pragma once
#include "Header.h"
#include "Tree_generation.h"

set<int> Get_v_to_recol(const Graph& tree) {
    set <int> vertices_to_recolor;
    int i = 0;
    for (vec neighboors : tree.adj_list) {
        neighboors.erase(neighboors.begin());
        set<int> unq_childs(neighboors.begin(), neighboors.end());
        if (unq_childs.size() > 1) {
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
    set<int> v_rec = Get_v_to_recol(tree);
    int cur_v = Get_rand_v(v_rec);
    int first_v = cur_v;

    int left_col = tree.ind_to_color[tree.adj_list[cur_v][1]];
    int right_col = tree.ind_to_color[tree.adj_list[cur_v][2]];
    int cur_col = tree.ind_to_color[cur_v];
    int new_col = (cur_col != left_col) ? left_col : right_col;
    tree.ind_to_color[cur_v] = new_col;
    while(cur_v != 0) {
        int parent_v = tree.adj_list[cur_v][0];
        int parent_col = tree.ind_to_color[parent_v];

        if (tree.adj_list[parent_v].size() == 2) {
            tree.ind_to_color[parent_v] = new_col;
        } else {
            int neighboor_v = (tree.adj_list[parent_v][1] != cur_v) ? tree.adj_list[parent_v][1] : tree.adj_list[parent_v][2];
            int neighboor_col = tree.ind_to_color[neighboor_v];
            if (parent_col != new_col && parent_col != neighboor_col) { // Rewrite according to the new logic of 3-son vertices presense
                tree.ind_to_color[parent_v] = (rand() % 2 == 0) ? new_col : neighboor_col;
            } else {
                break;
            }
        }
        cur_v = parent_v;
    }
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
    Graph base_tree(tree);
    for (int i = 0; i < n; ++i) {
        printf("Iteration %d \n", i);
        vec_vec old_t_net = Build_t_net(base_tree);
        int old_s = Calc_s_metric(old_t_net);
        s_metrics.push_back(old_s);

        Graph new_tree(base_tree);
        int rec_v = Recol_rand_v(new_tree);
        vec_vec new_t_net = Build_t_net(new_tree);
        rec_vs.push_back(rec_v);

        int new_s = Calc_s_metric(new_t_net);
        if (rand() % (old_s + new_s) > old_s) {
            base_tree = new_tree;
        }
    }
    return base_tree;
}