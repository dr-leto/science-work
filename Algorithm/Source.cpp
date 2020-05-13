#include "Tree_generation.h"
#include "Save_load.h"
#include "MCMC.h"

void Run_integration_test() {
    string outbreak_name = "1/";
    string outbreak_type = "test/";
    string raw_graph_path = "in/" + outbreak_type + outbreak_name + "raw_graph.txt";
    vec_vec adj_list{
        {0, 1, 8},
        {0, 2, 5},
        {1, 3, 4},
        {2},
        {2},
        {1, 6, 7},
        {5},
        {5},
        {0, 9, 12},
        {8, 10, 11},
        {9},
        {9},
        {8, 13, 14},
        {12},
        {12}
    };
    vec col_to_ind{ 0, 0, 0, 1, 2, 0, 3, 3, 0, 0, 5, 6, 0, 7, 7 };
    Graph raw_graph(adj_list, col_to_ind);
    Save_graph(raw_graph, raw_graph_path);
    Graph graph = Read_graph(raw_graph_path);
    vec s_metrics;
    //srand(time(NULL));
    Graph result = MCMC_run(graph, 300, s_metrics);
    vec_vec t_net = Build_t_net(result);
    int top_v_col = result.ind_to_color[0];
    int s_metric = Calc_s_metric(t_net);
    string prefix = "out/" + outbreak_type + outbreak_name;
    Save_graph(result, prefix + "mcmc_best_tree.txt");
    Save_t_net(t_net, prefix + "t_net.txt", s_metric, top_v_col);
    Save_arr(s_metrics, prefix + "s_metrics.txt");
}

void Run_hepatit() {
    vec s_metrics;
    string outbreak_name = "BB/";
    string outbreak_type = "hepatit/";

    Graph outbreak = Read_graph("in/" + outbreak_type + outbreak_name + "matrix.txt");
    Graph result = MCMC_run(outbreak, 100000, s_metrics);
    vec_vec t_net = Build_t_net(result);
    int s_metric = Calc_s_metric(t_net);
    string prefix = "out/" + outbreak_type + outbreak_name;
    Save_graph(result, prefix + "mcmc_best_tree.txt");
    Save_t_net(t_net, prefix + "t_net.txt", s_metric);
    Save_arr(s_metrics, prefix + "s_metrics.txt");
}

int main() {
    Run_integration_test();
    return 0;
}