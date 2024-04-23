#include <post.h>

namespace post_p
{

void postprocessing::build_bond_map()
{

    for (int i = 0; i < numParticles(); i++){

        for (int j = 0; j < numAttachments(i); j++)
            bond_map_status.insert({{i,attachment(i,j)}, 1});
    }

}

void postprocessing::switch_off_bonds(std::pair<int,int> bond)
{

    bond_map_status[bond] = 0;

    temp_pair.first = bond.second;
    temp_pair.second = bond.first;

    bond_map_status[temp_pair] = 0;

}

void postprocessing::switch_on_bonds(std::pair<int,int> bond)
{

    bond_map_status[bond] = 1;

    temp_pair.first = bond.second;
    temp_pair.second = bond.first;

    bond_map_status[temp_pair] = 1;

}

void postprocessing::reset_bond_map(bool status)
{

    if (status) {

        for (int i = 0; i < numParticles(); i++){
            for (int j = 0; j < numAttachments(i); j++) {
                switch_on_bonds({i, attachment(i,j)});
            }
        }
        
    }

    else {

        for (int i = 0; i < numParticles(); i++){
            for (int j = 0; j < numAttachments(i); j++) {
                switch_off_bonds({i, attachment(i,j)});
            }
        }       

    }


}

void postprocessing::reset_unfolding_params()
{
    for (int j = 0; j < numParticles(); j++){
        is_placed(j) = false;
        attachments_placed(j) = false;
    }
}

void postprocessing::dump_lb_bonds_for_cluster_via_invA(char *filename)
{
    determine_LB_bonds_clusterwise(filename);
}

bool postprocessing::check_if_particles_placed()
{

    bool all_particles_placed = true;

    for (int i = unf_checkpoint; i < numParticles(); i++) {
        if (is_placed(i) == false)
            return false;
    }

    return true;

}

void postprocessing::set_lbb_params(char *filename)
{    
    reset_unfolding_params();
    build_bond_map();
    reset_bond_map(true);

    ref_pos.resize(dim());
    f_lbp = fopen(filename, "w");
    lbp_tolerance = 1e-6;
}

void postprocessing::init_lbb_unfolding_without_recursion()
{
    num_bonds_for_cluster = 0;
    num_particles_for_cluster = 0;
    unique_bonds.clear();
    index_to_particles.clear();
    particles_to_index.clear();
    to_build_list.clear();

    for (int i = unf_checkpoint; i < numParticles(); i++){
        if (is_placed(i) == false){
            particles_to_index[i] = num_particles_for_cluster;
            index_to_particles[num_particles_for_cluster] = i;

            to_build_list[num_particles_for_cluster] = i;
            num_particles_for_cluster++;

            unfold_for_clusterwise_without_recursion(i);
            is_placed(i) = true;            
            
            for (int temp_index = 1; temp_index < num_particles_for_cluster; temp_index++){
                unfold_for_clusterwise_without_recursion(to_build_list[temp_index]);
            }

            unf_checkpoint = i;
            break;
        }
    }
}

void postprocessing::unfold_for_clusterwise_without_recursion(int i)
{
    for (int att = 0; att < numAttachments(i); att++)
    {
        index_j = attachment(i, att);

        if ((attachments_placed(index_j) == false) && (bond_map_status[{i,index_j}] == 1))
        {

            if (is_placed(index_j) == false){
                particles_to_index[index_j] = num_particles_for_cluster;
                index_to_particles[num_particles_for_cluster] = index_j;
                is_placed(index_j) = true;
                to_build_list[num_particles_for_cluster] = index_j;
                num_particles_for_cluster++;
            }

            i_map = particles_to_index[i];
            j_map = particles_to_index[index_j];

            unique_bonds[num_bonds_for_cluster] = {i_map,j_map};
            num_bonds_for_cluster += 1;


        }

    }

    attachments_placed(i) = true;
}

void postprocessing::init_lbb_cluster_matrices()
{

    modified_folded_x.resize(num_particles_for_cluster,dim());
    copy_positions_for_cluster();

    nnz = 2 * num_bonds_for_cluster + num_particles_for_cluster;
    A.reserve(nnz);
    A.resize((num_particles_for_cluster-1), (num_particles_for_cluster-1));

    b.resize((num_particles_for_cluster-1));
    x.resize((num_particles_for_cluster-1));
    bond_lengths_direction_wise.resize(num_bonds_for_cluster, dim());
    bond_lengths.resize(num_bonds_for_cluster);

    A.setZero();
    build_A_for_cluster();
}

void postprocessing::print_lbb_info()
{

    max_length = bond_lengths.maxCoeff(&max_row);

    if (max_length > lbp_tolerance){

        switch_off_bonds({index_to_particles[unique_bonds[max_row].first], index_to_particles[unique_bonds[max_row].second]});
        modify_A_matrix();

        fprintf(f_lbp, "%d %d\n",index_to_particles[unique_bonds[max_row].first],index_to_particles[unique_bonds[max_row].second]);
        n_lbb++;

    }

}

void postprocessing::determine_LB_bonds_clusterwise(char *filename)
{
    
    int min_cluster_size = 3;

    set_lbb_params(filename);

    int num_A_constructions = 0;
    int num_invAb_ops = 0;

    to_build_list.resize(numParticles());
    unique_bonds.resize(num_bonds);
    
    while (!check_if_particles_placed()) {

        init_lbb_unfolding_without_recursion();

        if (num_particles_for_cluster >= min_cluster_size) {

            init_lbb_cluster_matrices();
            
            do {

                modify_coords_for_cluster();
                
                if (!A.isCompressed()){
                    A.makeCompressed();
                }
                solver.analyzePattern(A);
                solver.factorize(A);

                for (int axis = 0; axis < dim(); axis++){

                    b.setZero();
                    build_b_for_cluster(axis);
                    x = solver.solve(b);
                    num_invAb_ops++;
                    modify_coords_after_minimization_for_cluster(axis);

                }
                
                calculate_bond_lengths_for_cluster();
                print_lbb_info();
                

            } while(max_length > lbp_tolerance);

        }
    }
    
    fclose(f_lbp);
}

void postprocessing::copy_positions_for_cluster()
{

    int index;

    for (int i = 0; i < num_particles_for_cluster; i++){
        for (int axis = 0; axis < dim(); axis++){

            index = index_to_particles[i];

            modified_folded_x(i,axis)  = pos(index,axis);
            modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis)); 

        }
    }

}

void postprocessing::modify_coords_for_cluster()
{

    int index;

    for (int axis = 0; axis < dim(); axis++){
        ref_pos(axis) = modified_folded_x(num_particles_for_cluster-1, axis);
    }

    for (int i = 0; i < num_particles_for_cluster; i++){

        for (int axis = 0; axis < dim(); axis++){

            modified_folded_x(i,axis)  = modified_folded_x(i,axis) - ref_pos(axis);
            modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis));

        }

    }


}

void postprocessing::modify_coords_after_minimization_for_cluster(int axis)
{

    for (int i = 0; i < num_particles_for_cluster-1; i++)
        modified_folded_x(i,axis)  = (L(axis) * x(i)) + ref_pos(axis);

    modified_folded_x(num_particles_for_cluster-1,axis) = ref_pos(axis);

    for (int i = 0; i < num_particles_for_cluster; i++)
        modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis));


}

void postprocessing::build_A_for_cluster()
{

    int i,j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        if (i == (num_particles_for_cluster-1)){
            tripleList.push_back(Eigen::Triplet<double>(j,j,-1));
        }

        else if (j == (num_particles_for_cluster-1)){
            tripleList.push_back(Eigen::Triplet<double>(i,i,-1));
        }

        else{
            tripleList.push_back(Eigen::Triplet<double>(i,j,1));
            tripleList.push_back(Eigen::Triplet<double>(j,i,1));
            tripleList.push_back(Eigen::Triplet<double>(i,i,-1));
            tripleList.push_back(Eigen::Triplet<double>(j,j,-1));
        }

    }

    A.setFromTriplets(tripleList.begin(), tripleList.end());
    tripleList.clear();

}

void postprocessing::build_b_for_cluster(int axis)
{

    int i,j;
    int index_i, index_j;
    double dx;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        if (bond_map_status[{index_i,index_j}] == 1){

            dx = round((modified_folded_x(i,axis) - modified_folded_x(j,axis))/L(axis));

            if (i == (num_particles_for_cluster-1)){
                b(j) += dx;
            }

            else if (j == (num_particles_for_cluster-1)){
                b(i) -= dx;
            }

            else {
                b(i) -= dx;
                b(j) += dx;
            }

        }


    }

}

void postprocessing::modify_A_matrix()
{
    a_i = unique_bonds[max_row].first;
    a_j = unique_bonds[max_row].second;

    if (a_i == (num_particles_for_cluster-1)){
        A.coeffRef(a_j,a_j) += 1;
    }

    else if (a_j == (num_particles_for_cluster-1)){
        A.coeffRef(a_i, a_i) += 1;
    }

    else{

        A.coeffRef(a_i,a_j)  = 0;
        A.coeffRef(a_j,a_i)  = 0;
        A.coeffRef(a_i,a_i) += 1;
        A.coeffRef(a_j,a_j) += 1;

    }

}

void postprocessing::calculate_bond_lengths_direction_wise_for_cluster(int axis)
{
    int i,j;
    int index_i, index_j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        if (bond_map_status[{index_i,index_j}] == 1){
            bond_lengths_direction_wise(index, axis)  = (modified_folded_x(i, axis) - modified_folded_x(j, axis));
            bond_lengths_direction_wise(index, axis) -= (L(axis) * round(bond_lengths_direction_wise(index, axis)/L(axis)));
            bond_lengths_direction_wise(index, axis) *= bond_lengths_direction_wise(index, axis);
        }

        else {
            bond_lengths_direction_wise(index, axis)  = 0.;
        }

    }

}

void postprocessing::calculate_bond_lengths_for_cluster()
{

    bond_lengths_direction_wise.setZero();
    bond_lengths.setZero();

    for (int axis = 0; axis < dim(); axis++){
        calculate_bond_lengths_direction_wise_for_cluster(axis);
    }

    for (int i = 0; i < num_bonds_for_cluster; i++){
        for (int axis = 0; axis < dim(); axis++) {

            bond_lengths(i) += bond_lengths_direction_wise(i,axis);

        }

        bond_lengths(i) = sqrt(bond_lengths(i));
        

    }


}
   

}