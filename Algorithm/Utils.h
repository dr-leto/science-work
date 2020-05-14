#pragma once
#include "Header.h"

vec_vec Graph_to_matrix(const Graph& graph) {
    int n = graph.adj_list.size();
    vec_vec matrix(n, vec(n, -1));
    for (int i = 0; i < n; ++i) {
        for (int cur_v : graph.adj_list[i]) {
            matrix[i][cur_v] = graph.ind_to_color[cur_v];
        }
    }
    matrix[0][0] = -1; // it's often that root tree has loop edge, just to remove it.
    return matrix;
}

Graph Matrix_to_graph(const vec_vec& matrix) {
    int n = matrix.size();
    vec_vec adj_list(n, vec());
    adj_list[0].push_back(0);
    vec ind_to_color(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] != -1) {
                adj_list[i].push_back(j); // we assume here, that the very first vertex would be parent of the current one (Pre-order traversal). Otherwise that leads to errors
                if (ind_to_color[j] == 0) {
                    ind_to_color[j] = matrix[i][j];
                }
            }
        }
    }
    return Graph(adj_list, ind_to_color);
}

void print_perc(int i, int n) {
    if (n < 100) {
        printf("%d it completed \n", i);
    }
    else if (i % (n / 100) == 0) {
        printf("%d perc completed \n", i * 100 / n);
    }
}

bool Has_cycle_util(const vec_vec& t_net, int v, int prev_v, vector<bool>& visited) {
    if (visited[v]) {
        return true;
    }
    else {
        visited[v] = true;
        for (int neighboor : t_net[v]) {
            if (neighboor != prev_v && Has_cycle_util(t_net, neighboor, v, visited)) {
                return true;
            }
        }
    }
    return false;
}

bool Has_cycle(const vec_vec& t_net) {
    vector<bool> visited(t_net.size(), false);

    return Has_cycle_util(t_net, 1, 0, visited);
}

