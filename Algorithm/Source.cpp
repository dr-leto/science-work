#include "Tree_generation.h"
#include "Save_load.h"
#include "MCMC.h"


void Run_test() {
    // vector<int> shape_sample{ 5, 2, 5, 2, 4, 5, 8, 1, 2, 4, 4, 5, 6, 8, 8};
    int max_height = 17;
    int col_num = 5;
    srand(time(NULL));
    // Arr_to_graph(shape_sample, 0, 0, tree);
    // Leaves_random_color(tree, col_num);
    // Shape_random_color(tree, 0);
    Graph tree = Generate_random_shape(max_height, col_num);
    vec metrics, rec_v;
    Graph best_tree = MCMC_run(tree, 100, metrics, rec_v);
    //printf("Saving graph... \n");
    //Save_graph(tree, "tree.txt");
    //printf("Saving best graph... \n");
    //Save_graph(best_tree, "best_tree.txt");
    //printf("Loading graph... \n");
    //Graph read_tree = Read_graph("tree.txt");
    //printf("Loading best graph... \n");
    //Graph read_best_tree = Read_graph("best_tree.txt");
    int a = 5;
}

void Run_hepatit() {
    vector<int> s_metrics;
    vector<int> rec_vs;
    string outbreak_name = "BB";
    Graph outbreak = Read_graph("in/hepatit/" + outbreak_name + "/matrix.txt");
    Graph result = MCMC_run(outbreak, 10000, s_metrics, rec_vs);
    vec_vec transm_net = Build_t_net(result);
    int s_metric = Calc_s_metric(transm_net);
    string prefix = "out/hepatit/" + outbreak_name;
    Save_graph(result, prefix + "/mcmc_best_tree.txt");
    Save_t_net(transm_net, prefix + "/t_net.txt", s_metric);
    Save_arr(s_metrics, prefix + "/s_metrics.txt");
    Save_arr(rec_vs, prefix + "/rec_vs.txt");
}

int main() {
    Run_hepatit();
    return 0;
}