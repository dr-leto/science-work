#include "Header.h"

void test() {
    //vector<int> tree_sample{ 2, 3, 2, 3, 3, -1, -1, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    int max_height = 10;
    int num_k = 6;
    vector<int> raw_shape = Generate_random_shape(max_height, num_k);
    Save_tree(raw_shape, "test/raw_shape.txt");
    vector<int> pruned_shape = Prune_shape(raw_shape);
    Save_tree(pruned_shape, "test/pruned_shape.txt");
    vector<int> colored_tree = Shape_random_color(pruned_shape);
    Save_tree(colored_tree, "test/colored_tree.txt");
    set<int> v_to_recol = Get_vertices_to_recolor(colored_tree);
    vector<int> vec_v_to_recol(v_to_recol.size(), 0);
    std::copy(v_to_recol.begin(), v_to_recol.end(), vec_v_to_recol.begin());
    Save_tree(vec_v_to_recol, "test/vertices_to_recolor.txt");
    vector<int> recolored_tree = Color_random_vertex(colored_tree);
    Save_tree(recolored_tree, "test/recolored_tree.txt");

    pair<vector<int>, vector<int>> result = MCMC_run(colored_tree, 100);
    Save_tree(result.first, "test/mcmc_tree.txt");
    Save_tree(result.second, "test/mcmc_s_metrics.txt");
}

int main() {
    test();

    return 0;
}