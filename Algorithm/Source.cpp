#include "Header.h"

void Test() {
    //vector<int> tree_sample{ 2, 3, 2, 3, 3, -1, -1, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    int max_height = 6;
    int num_k = 10;
    vector<int> raw_shape = Generate_random_shape(max_height, num_k);
    Save_tree(raw_shape, "test/raw_shape.txt");
    vector<int> pruned_shape = Prune_shape(raw_shape);
    Save_tree(pruned_shape, "test/pruned_shape.txt");
    vector<int> colored_tree = Shape_random_color(pruned_shape);
    Save_tree(colored_tree, "test/colored_tree.txt");
    vector<vector<int>> transm_net = Build_transm_network(colored_tree);
    int s_metric = Calc_s_metric(transm_net);
    Save_transm_net(transm_net, "test/transm_net.txt", s_metric);

    set<int> v_to_recol = Get_vertices_to_recolor(colored_tree);
    vector<int> vec_v_to_recol(v_to_recol.size(), 0);
    std::copy(v_to_recol.begin(), v_to_recol.end(), vec_v_to_recol.begin());
    pair<vector<int>, int> recolored_tree = Color_random_vertex(colored_tree);
    vec_v_to_recol.push_back(recolored_tree.second);
    Save_tree(vec_v_to_recol, "test/vertices_to_recolor.txt");
    Save_tree(recolored_tree.first, "test/recolored_tree.txt");
    vector<vector<int>> rec_transm_net = Build_transm_network(recolored_tree.first);
    int rec_s_metric = Calc_s_metric(rec_transm_net);
    Save_transm_net(rec_transm_net, "test/rec_transm_net.txt", rec_s_metric);

    vector<int> s_metrics;
    vector<int> recolored_vertices;
    vector<int> result = MCMC_run(colored_tree, 1000, s_metrics, recolored_vertices);
    Save_tree(result, "out/test/mcmc_tree.txt");
    Save_tree(s_metrics, "out/test/mcmc_s_metrics.txt");
    Save_tree(recolored_vertices, "test/mcmc_recolored_vertices.txt");
}

void Run_hepatit() {
    vector<int> s_metrics;
    vector<int> recolored_vertices;
    vector<int> outbreak = Read_tree("AA_tree.txt");
    vector<int> result = MCMC_run(outbreak, 10000, s_metrics, recolored_vertices);
    vector<vector<int>> transm_net = Build_transm_network(result);
    int s_metric = Calc_s_metric(transm_net);
    Save_tree(result, "out/hepatit/mcmc_tree.txt");
    Save_transm_net(transm_net, "out/hepatit/transm_net.txt", s_metric);
    Save_tree(s_metrics, "out/hepatit/mcmc_s_metrics.txt");
    Save_tree(recolored_vertices, "out/hepatit/mcmc_recolored_vertices.txt");

}

int main() {
    Run_hepatit();

    return 0;
}