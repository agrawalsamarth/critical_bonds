#include <bits/stdc++.h>
#include <./eigen-3.4.0/Eigen/Sparse>

#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

namespace post_p{

class postprocessing
{
    public:

    postprocessing(char *config_filename);
    postprocessing(char *config_filename, bool build);
    ~postprocessing();
    void read_config();

    std::vector<std::string> split_string_by_delimiter(const std::string& s, char delimiter);
    void read_config_parser(char *config_filename);

    int dim() const;
    int numParticles() const;
    double  lo_hi(const int axis, const int limit) const;
    double& lo_hi(const int axis, const int limit);
    double  L(const int axis) const;
    double& L(const int axis);
    double  halfL(const int axis) const;
    double& halfL(const int axis);
    int  periodic(const int axis) const;
    int& periodic(const int axis);
    double  posDiff(const int axis) const;
    double& posDiff(const int axis);
    double  pos(const int i, const int axis) const;
    double& pos(const int i, const int axis);
    int  numAttachments(const int i) const;
    int& numAttachments(const int i);
    int  attachment(const int i, const int j) const;
    int& attachment(const int i, const int j);
    bool  is_placed(const int i) const;
    bool& is_placed(const int i);
    bool  attachments_placed(const int i) const;
    bool& attachments_placed(const int i);
    double get_periodic_image(double x, const int axis);

    void init_unfolding_without_recursion();
    void place_attachments(int i);

    void build_bond_map();
    void reset_unfolding_params();
    void switch_off_bonds(std::pair<int,int> bond);
    void switch_on_bonds(std::pair<int,int> bond);
    void reset_bond_map(bool status);

    void modify_coords();
    void modify_coords_after_minimization(int axis);
    void build_A();
    void build_A(int axis);
    void build_b(int axis);
    void calculate_bond_lengths_direction_wise(int axis);
    void calculate_bond_lengths();
    void determine_LB_bonds();
    void copy_positions();

    void copy_positions_for_cluster();
    void unfold_for_clusterwise(int prev, int next);
    void determine_LB_bonds_clusterwise(char *filename);
    bool check_if_particles_placed();
    void modify_coords_for_cluster();
    void modify_coords_after_minimization_for_cluster(int axis);
    void build_A_for_cluster();
    void build_A_for_cluster(int axis);
    void build_b_for_cluster(int axis);
    void calculate_bond_lengths_direction_wise_for_cluster(int axis);
    void calculate_bond_lengths_for_cluster();
    void calculate_bond_lengths_for_cluster(int axis);
    void dump_lb_bonds_for_cluster_via_invA(char *filename);

    void set_lbb_params(char *filename);
    void init_lbb_unfolding();
    void init_lbb_cluster_matrices();
    void print_lbb_info();
    void modify_A_matrix();

    void init_lbb_unfolding_without_recursion();
    void unfold_for_clusterwise_without_recursion(int i);
    void unfold_for_lbp_without_recursion(int i);
    
    private:

    int     N_;
    int     D_;
    double *lo_hi_;
    double *L_;
    double *halfL_;
    int    *periodic_;
    double *posDiff_;
    double *pos_;
    int    *num_attachments_;
    std::vector<std::vector<int>> attachment_;
    bool   *is_placed_;
    bool   *attachments_placed_;
    bool   *node_info;

    std::map<std::pair<int,int>, int> bond_map_status;
    std::pair<int,int> temp_pair;
    std::vector<std::pair<int,int>> unique_bonds;

    Eigen::SparseMatrix<double> A;
    Eigen::VectorXd x;
    Eigen::VectorXd b;
    Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> solver;
    Eigen::MatrixXd modified_folded_x;
    Eigen::VectorXd ref_pos;
    Eigen::VectorXd bond_lengths;
    Eigen::MatrixXd bond_lengths_direction_wise;
    int num_bonds;
    int num_bonds_for_cluster;
    int num_particles_for_cluster;
    std::vector<std::pair<int,int>> unique_bonds_for_cluster;
    std::map<int, int> particles_to_index;
    std::map<int, int> index_to_particles;
    int index_i;
    int index_j;
    int nnz;
    int a_i;
    int a_j;
    double max_length;
    int max_row;
    double lbp_tolerance;
    FILE *f_lbp;
    std::vector<Eigen::Triplet<double>> tripleList;
    int n_lbb = 0;
    int particle_counter;
    std::vector<int> to_build_list;

    int i_map;
    int j_map;
    int total_num_bonds;
    int unf_checkpoint = 0;

};

}

#endif
