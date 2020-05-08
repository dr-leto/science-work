#pragma once
#include "Header.h"
#include "Tree_generation.h"

void Save_graph(Graph graph, string path) {
    ofstream fout(path);
    vec_vec matrix = Graph_to_matrix(graph);
    for (const vec& row : matrix) {
        for (int el : row) {
            fout << el << " ";
        }
        fout << endl;
    }
    fout.close();
}

Graph Read_graph(string path) {
    ifstream fin(path);
    string row;
    int i = 0;
    vec_vec matrix;
    while (getline(fin, row)) {
        matrix.push_back(vec());
        istringstream n_stream(row);
        int num;
        while (n_stream >> num) {
            matrix[i].push_back(num);
        }
        ++i;
    }
    fin.close();
    return Matrix_to_graph(matrix);
}

void Save_transm_net(vec_vec transm_net, string path, int s = 0) {
    ofstream fout(path);
    if (s != 0) {
        fout << s << endl;
    }
    for (unsigned int i = 0; i < transm_net.size(); ++i) {
        if (!transm_net[i].empty()) {
            fout << i;
            for (int el : transm_net[i]) {
                fout << " " << el;
            }
            fout << endl;
        }
    }
    fout.close();
}