#include <post.h>

namespace post_p
{

void postprocessing::dump_LB_bonds_for_network(char *filename)
{
    determine_LB_bonds_network(filename);
}

void postprocessing::reset_unfolding_for_network()
{
    unf_checkpoint = 0;

    for (int i = 0; i < numParticles(); i++){
        is_placed(i) = false;
        attachments_placed(i) = false;
    }

}

void postprocessing::set_lbb_params_for_network(char *filename)
{

    FILE *f_network;

    reset_unfolding_params();
    build_bond_map();
    reset_bond_map(true);

    f_network = fopen(filename, "w");
    fprintf(f_network,"bond,bond_length\n");
    fclose(f_network);

    lbp_tolerance = 1e-6;

}

void postprocessing::print_lbb_info_for_network(char *filename)
{

    FILE *f_network;

    f_network = fopen(filename, "a");
    max_length = bond_lengths.maxCoeff(&max_row);

    if (max_length > lbp_tolerance){

        switch_off_bonds({index_to_particles[unique_bonds[max_row].first], index_to_particles[unique_bonds[max_row].second]});
        fprintf(f_network, "%d-%d,%lf\n",index_to_particles[unique_bonds[max_row].first],index_to_particles[unique_bonds[max_row].second],max_length);
        n_lbb++;

    }

    fclose(f_network);

}

/*void postprocessing::build_A_for_network_cluster()
{

    int i,j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        tripleList.push_back(Eigen::Triplet<double>(i,j,1));
        tripleList.push_back(Eigen::Triplet<double>(j,i,1));
        tripleList.push_back(Eigen::Triplet<double>(i,i,-1));
        tripleList.push_back(Eigen::Triplet<double>(j,j,-1));

    }

    A.setFromTriplets(tripleList.begin(), tripleList.end());
    tripleList.clear();

}*/

void postprocessing::build_A_for_network_cluster()
{

    int i,j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        if ((current_seed(index_to_particles[i]) == 0) && (current_seed(index_to_particles[j]) == 0)){
            tripleList.push_back(Eigen::Triplet<double>(i,j,1));
            tripleList.push_back(Eigen::Triplet<double>(j,i,1));
            tripleList.push_back(Eigen::Triplet<double>(i,i,-1));
            tripleList.push_back(Eigen::Triplet<double>(j,j,-1));
        }

        else{
            tripleList.push_back(Eigen::Triplet<double>(i,j,stiffness_inv_new));
            tripleList.push_back(Eigen::Triplet<double>(j,i,stiffness_inv_new));
            tripleList.push_back(Eigen::Triplet<double>(i,i,(-1.*stiffness_inv_new)));
            tripleList.push_back(Eigen::Triplet<double>(j,j,(-1.*stiffness_inv_new)));
        }

        /*tripleList.push_back(Eigen::Triplet<double>(i,j,1));
        tripleList.push_back(Eigen::Triplet<double>(j,i,1));
        tripleList.push_back(Eigen::Triplet<double>(i,i,-1));
        tripleList.push_back(Eigen::Triplet<double>(j,j,-1));*/

    }

    A.setFromTriplets(tripleList.begin(), tripleList.end());
    tripleList.clear();

}


void postprocessing::build_b_for_network_cluster()
{
    for (int i = 0; i < num_particles_for_cluster; i++)
    {
        b(i) = current_seed(index_to_particles[i]);
    }
}

bool postprocessing::check_sink_and_source()
{

    bool sink   = false;
    bool source = false;

    for (int i = 0; i < num_particles_for_cluster; i++){

        if (current_seed(index_to_particles[i]) == 1)
            sink = true;

        if (current_seed(index_to_particles[i]) == -1)
            source = true;

    }

    if (sink && source)
        return true;
    
    else
        return false;

}

void postprocessing::fix_A_for_network_cluster()
{

    int i, j;

    for (int i = 0; i < num_particles_for_cluster; i++){

        if (current_seed(index_to_particles[i]) != 0)
            A.coeffRef(i,i) -= 1.;

    }

    //A.setFromTriplets(tripleList.begin(), tripleList.end());
    //tripleList.clear();

}

void postprocessing::init_lbb_cluster_matrices_for_network_cluster()
{
    nnz = (2 * num_bonds_for_cluster) + num_particles_for_cluster;
    A.reserve(nnz);
    A.resize(num_particles_for_cluster, num_particles_for_cluster);

    b.resize(num_particles_for_cluster);
    x.resize(num_particles_for_cluster);
    bond_lengths.resize(num_bonds_for_cluster);

    A.setZero();
    b.setZero();
    x.setZero();
    bond_lengths.setZero();

    build_A_for_network_cluster();
    build_b_for_network_cluster();

    fix_A_for_network_cluster();

}

void postprocessing::calculate_bond_lengths_for_network_cluster()
{

    int i, j;
    int index_i, index_j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        /*if ((current_seed(index_i) != 0) || (current_seed(index_j) != 0)){
            bond_lengths[index] = stiffness_inv_new * (x(i) - x(j)) * (x(i) - x(j));
            //bond_lengths[index] = 0.;
        }

        else
            bond_lengths[index] = (x(i) - x(j)) * (x(i) - x(j));*/

        bond_lengths[index] = (x(i) - x(j)) * (x(i) - x(j));

    }

}

void postprocessing::determine_LB_bonds_network(char *filename)
{

    set_lbb_params_for_network(filename);

    to_build_list.resize(numParticles());

    calc_total_bonds();
    unique_bonds.resize(total_num_bonds);

    totalClusters_ = 0;
    
    while (!check_if_particles_placed()) {

        init_lbb_unfolding_without_recursion();

        //std::cout<<"num particles = "<<num_particles_for_cluster<<"\t num_bonds = "<<num_bonds_for_cluster<<"\n";

        if (check_sink_and_source()) {

            init_lbb_cluster_matrices_for_network_cluster();
            
            do {
                
                if (!A.isCompressed()){
                    A.makeCompressed();
                }

                solver.analyzePattern(A);
                solver.factorize(A);

                x = solver.solve(b);
                
                calculate_bond_lengths_for_network_cluster();
                print_lbb_info_for_network(filename);

                if (max_length > lbp_tolerance){
                    reset_unfolding_for_network();
                    break;
                }
                

            } while(max_length > lbp_tolerance);


        }

    }
    
}



}