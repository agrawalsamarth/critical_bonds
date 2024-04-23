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

        /*for (int axis = 0; axis < dim(); axis++)
            unfolded_coords(j,axis) = 0.;*/

    }

    //for (int axis = 0; axis < dim(); axis++)
        //temp_lbp_[axis] = 0;

}

void postprocessing::dump_lb_bonds_for_cluster_via_invA(char *filename)
{
    determine_LB_bonds_clusterwise(filename);
}

void postprocessing::calc_total_bonds()
{
    total_num_bonds = 0;

    for (int i = 0; i < numParticles(); i++)
        total_num_bonds += numAttachments(i);

    total_num_bonds /= 2;
}

bool postprocessing::check_if_particles_placed()
{

    bool all_particles_placed = true;

    for (int i = unf_checkpoint; i < numParticles(); i++) {
        //all_particles_placed = all_particles_placed && is_placed(i);
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

    total_lbp = 0;
    ref_pos.resize(dim());
    totalClusters_ = 0;
    f_lbp = fopen(filename, "w");
    lbp_tolerance = 1e-6;

}

/*void postprocessing::init_lbb_unfolding()
{

    num_bonds_for_cluster = 0;
    num_particles_for_cluster = 0;
    unique_bonds.clear();
    index_to_particles.clear();
    particles_to_index.clear();

    for (int i = 0; i < numParticles(); i++){
        if (is_placed(i) == false){
            particles_to_index[i] = 0;
            index_to_particles[0] = i;
            num_particles_for_cluster += 1;
            unfold_for_clusterwise(i,i);
            break;
        }
    }

    //std::cout<<"num_particles_for_cluster = "<<num_particles_for_cluster<<"\t num_bonds = "<<num_bonds_for_cluster<<std::endl;

}*/

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

/*void postprocessing::unfold_for_clusterwise(const int prev, const int next)
{

    for (int axis = 0; axis < dim(); axis++)
        posDiff(axis) = get_periodic_image(pos(next,axis) - pos(prev,axis), axis);

    for (int axis = 0; axis < dim(); axis++){
        if (prev == next)
            unfolded_coords(next,axis) = pos(prev,axis) + posDiff(axis);
        else
            unfolded_coords(next,axis) = unfolded_coords(prev,axis) + posDiff(axis);
    }

    is_placed(next) = true;

    for (int att = 0; att < numAttachments(next); att++)
    {
        temp_next = attachment(next, att);
        
        
        if (is_placed(temp_next) == false){

            particles_to_index[temp_next] = num_particles_for_cluster;
            index_to_particles[num_particles_for_cluster] = temp_next;
            num_particles_for_cluster++;

            /*if (particles_to_index.count(next) != 1){
                std::cout<<"unfold 1"<<std::endl;
            }


            if (particles_to_index.count(temp_next) != 1){
                std::cout<<"unfold 2"<<std::endl;
            }            

            index_i = particles_to_index[next];
            index_j = particles_to_index[temp_next];

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(index_j,index_i)) == unique_bonds.end()){
                unique_bonds.push_back({index_i,index_j});
                num_bonds_for_cluster += 1;
            }

            unfoldedAttachments(next)      = unfoldedAttachments(next) + 1;
            unfoldedAttachments(temp_next) = unfoldedAttachments(temp_next) + 1;
            unfold_for_clusterwise(next, temp_next);
        }

        else {

            index_i = particles_to_index[next];
            index_j = particles_to_index[temp_next];

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(index_j,index_i)) == unique_bonds.end()){
                unique_bonds.push_back({index_i,index_j});
                num_bonds_for_cluster += 1;
            }            
            
            for (int axis = 0; axis < dim(); axis++) {
                posDiff(axis) = unfolded_coords(temp_next, axis) - unfolded_coords(next, axis);
                if ((posDiff(axis) > halfL(axis)) || (posDiff(axis) < -halfL(axis))){
                    cluster_percolation(totalClusters_,axis) = 1;
                    load_bearing_paths_[axis]++;
                }
            }

        }

    }


}*/


void postprocessing::init_lbb_cluster_matrices()
{

    modified_folded_x.resize(num_particles_for_cluster,dim());
    copy_positions_for_cluster();

    nnz = 2 * num_bonds_for_cluster + num_particles_for_cluster;
    A.reserve(nnz);
    A.resize((num_particles_for_cluster-1), (num_particles_for_cluster-1));

    b.resize((num_particles_for_cluster-1));
    old_b.resize((num_particles_for_cluster-1));
    x.resize((num_particles_for_cluster-1));
    bond_lengths_direction_wise.resize(num_bonds_for_cluster, dim());
    bond_lengths.resize(num_bonds_for_cluster);

    A.setZero();
    build_A_for_cluster();
    //A.makeCompressed();

}

void postprocessing::print_lbb_info()
{

    max_length = bond_lengths.maxCoeff(&max_row);

    if (max_length > lbp_tolerance){

        switch_off_bonds({index_to_particles[unique_bonds[max_row].first], index_to_particles[unique_bonds[max_row].second]});
        modify_A_matrix();

        fprintf(f_lbp, "%d %d\n",index_to_particles[unique_bonds[max_row].first],index_to_particles[unique_bonds[max_row].second]);

        //for (int print_axis = 0; print_axis < dim(); print_axis++)
            //fprintf(f_lbp, "%lf,", sqrt(bond_lengths_direction_wise(max_row,print_axis)));

        //fprintf(f_lbp, "%lf\n", max_length);
        n_lbb++;

        //std::cout<<"num_c = "<<num_particles_for_cluster<<"\t n_lbb = "<<n_lbb<<std::endl;

    }

}

/*void postprocessing::print_minimized_coords(char *filename, int filenum)
{

    std::string file_test(filename);
    std::vector<std::string> results;
    
    results = split_string_by_delimiter(file_test, '.');

    std::string full_filename = results[0] + "_" + std::to_string(filenum) + "." + results[1];
    
    for (int i = 0; i < results.size(); i++)
        std::cout<<"results "<<i<<" = "<<results[i]<<std::endl;

    file_test = file_test.substr(0, file_test.size()-4);
    std::string full_filename = file_test + "_" + std::to_string(filenum) + ".csv";

    std::ofstream f_coords;
    f_coords.open(full_filename);
    f_coords << "id,";

    for (int axis = 0; axis < dim(); axis++){

        if (axis == (dim() - 1)){
            f_coords<<"x"<<axis<<"\n";
        }

        else{
            f_coords<<"x"<<axis<<",";
        }

    }

    int p_index;

    for (int i = 0; i < num_particles_for_cluster; i++){

        //p_index = particles_to_index[i];
        p_index = index_to_particles[i];
        //std::cout<<"i = "<<i<<"\t p_index = "<<std::endl;
        f_coords<<p_index<<",";

        for (int axis = 0; axis < dim(); axis++){

            if (axis == (dim()-1)){
                //f_coords<<modified_folded_x(p_index,axis)<<"\n";
                f_coords<<modified_folded_x(i,axis)<<"\n";
            }

            else{
                //f_coords<<modified_folded_x(p_index,axis)<<",";
                f_coords<<modified_folded_x(i,axis)<<",";
            }

        }

    }

    f_coords.close();
}*/

/*void postprocessing::print_cluster_vals(char *filename)
{
    std::string file_test(filename);
    file_test = file_test.substr(0, file_test.size()-4);
    std::string full_filename = file_test + "_clusters.csv";

    std::ofstream f_coords;
    f_coords.open(full_filename, std::ios_base::app);

    for (int i = 0; i < num_particles_for_cluster; i++)
        f_coords<<to_build_list[i]<<","<<totalClusters_<<"\n";

    f_coords.close();

}*/

void postprocessing::determine_LB_bonds_clusterwise(char *filename)
{
    
    int min_cluster_size = 3;

    set_lbb_params(filename);

    int num_A_constructions = 0;
    int num_invAb_ops = 0;

    to_build_list.resize(numParticles());
    calc_total_bonds();
    unique_bonds.resize(total_num_bonds);
    totalClusters_ = 0;
    
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

        totalClusters_++;
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

void postprocessing::put_particles_back_in_box(int axis)
{

    for (int i = 0; i < num_particles_for_cluster; i++)
        modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis));


}

void postprocessing::build_A_for_cluster()
{

    int i,j;
    //int index_i, index_j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        //cp_A1 = std::chrono::steady_clock::now();

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        //cp_A2 = std::chrono::steady_clock::now();

        //time_A1 += std::chrono::duration_cast<std::chrono::nanoseconds>(cp_A2 - cp_A1).count();

        //index_i = index_to_particles[i];
        //index_j = index_to_particles[j];

        /*if (bond_map_status.count({index_i,index_j}) == 0){
            std::cout<<"problem with A"<<std::endl;
        }*/

        //if (bond_map_status[{index_i,index_j}] == 1){

        cp_A3 = std::chrono::steady_clock::now();

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

        //cp_A4 = std::chrono::steady_clock::now();

        //time_A2 += std::chrono::duration_cast<std::chrono::nanoseconds>(cp_A4 - cp_A3).count();


        //}


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

        /*if (bond_map_status.count({index_i,index_j}) == 0){
            std::cout<<"problem with b"<<std::endl;
        }*/

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


/*void postprocessing::copy_b()
{
    for (int i = 0; i < b.size(); i++)
        old_b(i) = b(i);
}

void postprocessing::calculate_b_diff()
{
    b_diff = b - old_b;

    if (b_diff.norm() > 1e-4)
        std::cout<<"b vector changes"<<std::endl;

}

void postprocessing::increase_stiffness()
{
    for (int i = 0; i < b.size(); i++){
        b(i) = 1. * b(i);
    }
}*/

void postprocessing::calculate_bond_lengths_direction_wise_for_cluster(int axis)
{

    //bond_distance.setZero();
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

/*void postprocessing::calculate_bond_lengths_for_cluster(int axis)
{

    //bond_distance.setZero();
    int i,j;
    int index_i, index_j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        if (bond_map_status[{index_i,index_j}] == 1){
            bond_lengths(index)  = (modified_folded_x(i, axis) - modified_folded_x(j, axis));
            bond_lengths(index) -= (L(axis) * round(bond_lengths(index)/L(axis)));
            bond_lengths(index)  = abs(bond_lengths(index));
        }

        else {
            bond_lengths(index)  = 0.;
        }

        //std::cout<<index_i<<"-"<<index_j<<" = "<<bond_lengths[index]<<std::endl;

    }    

    //std::cout<<"-----------------------------------------------------"<<std::endl;


}


void postprocessing::dump_xyz_stepwise(char *filename, int file_num)
{

    std::string ext_filename;

    ext_filename  = filename;
    ext_filename += "_" + std::to_string(file_num) + ".csv";

    std::ofstream test_f;
    test_f.open(ext_filename);

    for (int axis = 0; axis < dim(); axis++){
        if (axis != (dim()-1))
            test_f <<"x"+std::to_string(axis)+",";
        else
            test_f <<"x"+std::to_string(axis)+"\n";
    }

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){
            if (axis != (dim()-1))
                test_f <<std::to_string(modified_folded_x(i,axis))+",";
            else
                test_f <<std::to_string(modified_folded_x(i,axis))+"\n";
        }
    }

    test_f.close();
}*/
    

}