#pragma once
#include "Header.h"

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
void Save_tree(vector<int> shape, string path) {
    ofstream fout(path);
    for (int el : shape) {
        fout << el << " ";
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
vector<int> Read_tree(string path) {
    ifstream fin(path);
    string row;
    getline(fin, row);
    istringstream n_stream(row);
    int num;
    vector<int> shape;
    while (n_stream >> num) {
        shape.push_back(num);
    }
    fin.close();
    return shape;
}

void Save_transm_net(vector<vector<int>> transm_net, string path, int s = 0) {
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