#include "Tree_generation.h"
#include "Saver.h"
#include "MCMC.h"


void Run_test() {
    vector<int> shape_sample{ 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, -1, -1, -1, -1 };
    int max_height = 3;
    int col_num = 3;
    srand(time(NULL));
    Graph new_shape;
    Arr_to_adj_list(shape_sample, 0, 0, new_shape);
    Leaves_random_color(new_shape, col_num);
    Shape_random_color(new_shape, 0);
    int a = 5;
}

void Run_hepatit() {
    vector<int> s_metrics;
    vector<int> recolored_vertices;
    string outbreak_name = "AA/";
    vector<int> outbreak = Read_tree("in/hepatit/" + outbreak_name + "tree.txt");
    vector<int> result = MCMC_run(outbreak, 100, s_metrics, recolored_vertices);
    vector<vector<int>> transm_net = Build_transm_network(result);
    int s_metric = Calc_s_metric(transm_net);
    string prefix = "out/hepatit/" + outbreak_name;
    Save_tree(result, prefix + "mcmc_tree.txt");
    Save_transm_net(transm_net, prefix + "transm_net.txt", s_metric);
    Save_tree(s_metrics, prefix + "mcmc_s_metrics.txt");
    Save_tree(recolored_vertices, prefix + "mcmc_recolored_vertices.txt");

}

int main() {
    Run_test();

    return 0;
}