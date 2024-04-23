#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>
//#include <voro++/voro++.hh>
#include <map>
#include <algorithm>
#include <bits/stdc++.h>
//#include </home/mkroeger/PC/vsandra/C4SCIENCE/dlma/eigen-3.4.0/Eigen/Sparse>
#include <./eigen-3.4.0/Eigen/Sparse>
#include <chrono>
#include <sys/resource.h>

#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

namespace post_p{

class postprocessing
{
    public:

    postprocessing(char *config_filename);
    postprocessing(char *config_filename, bool build);
    ~postprocessing();
    void read_params();
    void read_config();
    void memory_allocation_function();

    std::vector<std::string> split_string_by_delimiter(const std::string& s, char delimiter);
    void read_params_parser(char *config_filename);
    void read_config_parser(char *config_filename);

    void read_params_for_percolation_build(char *config_filename);

    int dim() const;
    int numParticles() const;
    int maxAttachments() const;
    int totalClusters() const;
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
    int  original_seed(const int i) const;
    int& original_seed(const int i);
    int  current_seed(const int i) const;
    int& current_seed(const int i);
    double  diameter(const int i) const;
    double& diameter(const int i);
    double  radius(const int i) const;
    double& radius(const int i);
    int  unfoldedAttachments(const int i) const;
    int& unfoldedAttachments(const int i);
    int  attachment(const int i, const int j) const;
    int& attachment(const int i, const int j);
    bool  is_placed(const int i) const;
    bool& is_placed(const int i);
    bool  attachments_placed(const int i) const;
    bool& attachments_placed(const int i);
    double  unfolded_coords(const int i, const int axis) const;
    double& unfolded_coords(const int i, const int axis);
    double  delta_coords(const int i, const int axis) const;
    double& delta_coords(const int i, const int axis);
    int  delta_hist(const int i) const;
    int& delta_hist(const int i);
    int  cluster_percolation(const int i, const int axis) const;
    int& cluster_percolation(const int i, const int axis);
    int  load_bearing_paths(const int i, const int axis) const;
    int& load_bearing_paths(const int i, const int axis);



    double get_periodic_image(double x, const int axis);

    void print_positions();
    
    void calc_rij();
    void dump_rij_file();
    void dump_rij_file(char *filename);
    void dump_rij_hist_file(double bin_size);
    void dump_rij_hist_file(double bin_size, char *filename);
    void calc_rij_hist(double bin_size);
    void dump_unfolded_file();
    void dump_unfolded_file(char *filename);
    void dump_density_correlation(double bin_size);
    void dump_density_correlation(double bin_size, char *filename);
    void dump_percolation_file();
    void dump_percolation_file(char *filename);
    void dump_percolation_via_invA_file();
    void dump_percolation_via_invA_file(char *filename);
    void dump_load_bearing_paths_file(char *filename);
    //void dump_load_bearing_paths_file(int origin, char *filename);
    void dump_load_bearing_paths_file();
    void dump_rog();
    void dump_rog(char *filename);
    void init_unfolding();
    void init_unfolding_for_lbp();
    void unfold(const int prev, const int next);
    void unfold_for_lbp(const int prev, const int next, bool build_pb);
    void calc_unfolded_rij();
    void dump_unfolded_hist_file(double bin_size);
    void calc_unfolded_hist(double bin_size);
    void dump_scattering_function(double q_min, double q_max, int num_q);
    void dump_scattering_function(double q_min, double q_max, int num_q, char *filename);
    //void dump_lr_scattering_function(double q_min, double q_max, int num_q, double bin_size);
    void dump_lr_scattering_function(double q_min, double q_max, int num_q, double bin_size, char *filename);
    void calc_scattering_function(double q_min, double q_max, int num_q);
    void calc_density_correlation(double bin_size);
    void save_config();
    void save_unfolded_config();
    void save_unfolded_config(char *filename);
    void get_headers();
    bool check_percolation();
    double calc_rog();

    void psd(int total_iters);
    void save_radius_distribution();
    void save_radius_distribution(char *filename);
    void init_unfolding_without_recursion();
    void place_attachments(int i);

    void build_bond_map();
    void percolation_detection();
    void reset_unfolding_params();
    void switch_off_bonds(std::pair<int,int> bond);
    void switch_on_bonds(std::pair<int,int> bond);
    void create_subsystem();
    void erase_subsystem();
    void print_lbp();
    void postprocess_lbp();
    void switch_off_alt_lbp(int i);
    void reset_bond_map(bool status);
    void activate_path(std::vector<std::pair<int,int>> bonds, bool status);
    void shred_path(int i);
    void isolation_routine();
    void martin_test(char *lb_file);
    void lbp_brute_force();
    void makeCombiUtil(int n, int left, int k);
    void makeCombi(int n, int k);

    void dump_percolation_via_inverse();
    void percolation_via_inverse();
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
    void determine_LB_bonds_network(char *filename);
    void determine_LB_bonds_clusterwise_directionwise(char *filename);
    void determine_LB_bonds_clusterwise_via_cg(char *filename);
    bool check_if_particles_placed();
    void modify_coords_for_cluster();
    void modify_coords_after_minimization_for_cluster(int axis);
    void build_A_for_cluster();
    void build_A_for_cluster(int axis);
    void build_b_for_cluster(int axis);
    void build_A_for_cluster_for_cg();
    void build_b_for_cluster_for_cg(int axis);
    void calculate_bond_lengths_direction_wise_for_cluster(int axis);
    void calculate_bond_lengths_for_cluster();
    void calculate_bond_lengths_for_cluster(int axis);
    void dump_lb_bonds_for_cluster_via_invA(char *filename);
    void dump_lb_bonds_for_cluster_via_cg(char *filename);

    void dump_chemical_distance(char *lb_filename, char *cd_filename);
    void reset_chemical_distance_index();
    void find_chemical_distance(int i, int j);
    void find_shortest_path(int i, int j);

    void find_paths_for_all_bonds(char *lb_file, char *cd_filename);
    void find_all_paths(int source, int destination);
    void find_all_paths_recursion(int source, int destination);
    
    void distort_positions();
    void increase_stiffness();
    void copy_b();
    void calculate_b_diff();
    void put_particles_back_in_box(int axis);
    void dump_xyz_stepwise(char *filename, int file_num);

    void set_lbb_params(char *filename);
    void init_lbb_unfolding();
    void init_lbb_cluster_matrices();
    void print_lbb_info();
    void modify_A_matrix();


    void switch_off_transverse_forces(int axis);
    void print_minimized_coords(char *filename, int filenum);

    void init_lbb_unfolding_without_recursion();
    void unfold_for_clusterwise_without_recursion(int i);
    void calc_total_bonds();

    void print_mem_usage();
    void print_cluster_vals(char *filename);
    void unfold_for_lbp_without_recursion(int i);
    void comb(int N, int K);

    void set_lbb_params_for_network(char *filename);
    void print_lbb_info_for_network(char *filename);
    void dump_LB_bonds_for_network(char *filename);
    void reset_unfolding_for_network();
    void add_ghost_particle_to_network_cluster();
    void fix_A_for_network_cluster();
    void fix_b_for_network_cluster();
    void build_A_for_network_cluster();
    void build_b_for_network_cluster();
    void init_lbb_cluster_matrices_for_network_cluster();
    void calculate_bond_lengths_for_network_cluster();
    bool check_sink_and_source();

    void bulltest(int num);
    void calc_long_range_iq(int dr_steps, double q_min, double q_max, int num_q, char *filename);
    

    private:

    std::mt19937 generator;
    std::uniform_real_distribution<double> dis;

    int     headers_;
    int     N_;
    int     D_;
    int     max_attachments_;
    int     folded_;
    int     lattice_;
    int     iters_;
    int     columns_;
    double  seed_mass_;
    double  alpha_;
    double  phi_;
    double *lo_hi_;
    double *L_;
    double *halfL_;
    double *diameter_;
    double *radius_;
    int    *periodic_;
    double *posDiff_;
    int    *posDiff_int_;
    double *pos_;
    int    *num_attachments_;
    std::vector<std::vector<int>> attachment_;
    int    *original_seed_;
    int    *current_seed_;
    double *r_ij_;
    double *r_ij_hist_;
    int    *rho_hist_;
    bool   *is_placed_;
    bool   *attachments_placed_;
    double *unfolded_coords_;
    int     temp_next;
    double *delta_coords_;
    //double  q_min;
    //double  q_max;
    //double  dq;
    //int     num_q;
    double *sq_;
    double *lr_sq_;
    int    *unfolded_num_attachments_;
    int    *delta_hist_;
    int     max_delta_;
    int    *cluster_percolation_;
    int    *load_bearing_paths_;
    int    *temp_lbp_;
    int     totalClusters_;
    int     N_pairs_;
    int     r_ij_hist_bins_;
    
    bool    lattice_flag_ = false;
    bool    N_flag_ = false;
    bool    D_flag_ = false;
    bool    max_attachments_flag_ = false;
    bool    L_flag_ = false;
    bool    periodic_flag_ = false;
    bool    folded_flag_ = false;
    bool    iters_flag_ = false;
    bool    seed_mass_flag_ = false;
    bool    alpha_flag_ = false;
    bool    phi_flag_   = false;
    bool    columns_flag_ = false;

    double  *radius_dis;
    double  *centres;
    int     *type_dis;
    double  *rdf;
    int      max_psd_iters;
    int      total_lbp;
    double   stiffness_inv_new;

    std::map<std::pair<int,int>, int> bond_map_status;
    std::vector<std::pair<int,int>> percolating_bonds;
    std::pair<int,int> temp_pair;
    int *divisors;
    int *bond_bit_repr;
    std::vector<int> path_components;
    std::vector<std::pair<int,int>> current_path;
    bool path_percolation;
    std::vector<std::vector<std::pair<int,int>>> final_percolating_bonds;
    std::vector<std::vector<std::pair<int,int>>> weak_links;
    std::vector<bool> pb_status;

    std::vector<std::pair<int,int>> unique_bonds;
	std::vector<int> tmp;
    std::vector<int> ans;

    
    Eigen::SparseMatrix<double> A;
    Eigen::VectorXd x;
    Eigen::VectorXd b;
    Eigen::VectorXd old_b;
    Eigen::VectorXd b_diff;
    Eigen::SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> solver;
    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower|Eigen::Upper> cg;
    Eigen::MatrixXd modified_folded_x;
    Eigen::VectorXd ref_pos;
    Eigen::VectorXd bond_lengths;
    Eigen::MatrixXd bond_lengths_direction_wise;
    Eigen::VectorXd check_for_solver_mk;


    int num_bonds;

    int num_bonds_for_cluster;
    int num_particles_for_cluster;
    std::vector<std::pair<int,int>> unique_bonds_for_cluster;
    std::map<int, int> particles_to_index;
    std::map<int, int> index_to_particles;
    int index_i;
    int index_j;

    int *chemical_distance_index;
    std::vector<int> ref_nodes;
    std::vector<int> neighbour_nodes;
    std::vector<std::string> full_lb_list;
    std::vector<std::string> lb_bonds_str;

    bool *in_path;
    std::vector<int> source_to_destination_path;
    int current_path_index;

    FILE *f_bfs;

    double stiffness_coef;
    double inv_stiffness_coef;

    int *over_the_boundary;
    double max_length;
    int max_row;
    int max_col;
    double lbp_tolerance;
    int nnz;
    int a_i;
    int a_j;

    FILE *f_lbp;

    /*std::chrono::steady_clock::time_point cp_1;
    std::chrono::steady_clock::time_point cp_2;
    std::chrono::steady_clock::time_point cp_3;
    std::chrono::steady_clock::time_point cp_4;
    std::chrono::steady_clock::time_point cp_5;*/

    std::chrono::steady_clock::time_point cp_A1;
    std::chrono::steady_clock::time_point cp_A2;
    std::chrono::steady_clock::time_point cp_A3;
    std::chrono::steady_clock::time_point cp_A4;

    double time_A1 = 0.;
    double time_A2 = 0.;

    //Eigen::Triplet<double> T;
    std::vector<Eigen::Triplet<double>> tripleList;
    int n_lbb = 0;
    int particle_counter;
    std::vector<int> to_build_list;

    int i_map;
    int j_map;
    int total_num_bonds;

    std::chrono::steady_clock::time_point unf_cp_1;
    std::chrono::steady_clock::time_point unf_cp_2;
    std::chrono::steady_clock::time_point unf_cp_3;
    std::chrono::steady_clock::time_point unf_cp_4;
    std::chrono::steady_clock::time_point unf_cp_5;
    std::chrono::steady_clock::time_point unf_cp_6;
    std::chrono::steady_clock::time_point unf_cp_7;

    double unf_time_1 = 0.;
    double unf_time_2 = 0.;
    double unf_time_3 = 0.;
    double unf_time_4 = 0.;
    double unf_time_5 = 0.;
    double unf_time_6 = 0.;

    int unf_checkpoint = 0;
    struct rusage myusage;
    long baseline;


};

}

#endif
