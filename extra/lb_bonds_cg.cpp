#include <post.h>

namespace post_p
{

void postprocessing::dump_lb_bonds_for_cluster_via_cg(char *filename)
{

    std::cout<<"1"<<std::endl;
    determine_LB_bonds_clusterwise_via_cg(filename);
    std::cout<<"2"<<std::endl;
}

void postprocessing::determine_LB_bonds_clusterwise_via_cg(char *filename)
{
    std::cout<<"3"<<std::endl;

    reset_unfolding_params();
    build_bond_map();
    reset_bond_map(true);

    double max_length;
    int max_row, max_col;

    int total_lbp = 0;
    //int totalClusters;
    ref_pos.resize(dim());
    totalClusters_ = 0;

    FILE *f;

    f = fopen(filename, "w");

    fprintf(f,"clusterNumber,bond,");

    for (int axis = 0; axis < dim(); axis++)
        fprintf(f,"bond_component_x%d,",axis);

    fprintf(f,"bond_length\n");

    int nnz;

    double max_length_cutoff = 1e-8;

    while (!check_if_particles_placed()) {

        std::cout<<"4"<<std::endl;

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

        //std::cout<<"num_particles_for_cluster = "<<num_particles_for_cluster<<std::endl;
        //std::cout<<"num_bonds_for_cluster = "<<num_bonds_for_cluster<<std::endl;

        if (num_particles_for_cluster > 1) {

            std::cout<<"5"<<std::endl;

            modified_folded_x.resize(num_particles_for_cluster,dim());
            copy_positions_for_cluster();

            A.resize((num_particles_for_cluster-1), (num_particles_for_cluster-1));

            nnz = 2 * num_bonds_for_cluster + num_particles_for_cluster;
            A.reserve(nnz);

            b.resize((num_particles_for_cluster-1));
            x.resize((num_particles_for_cluster-1));
            bond_lengths_direction_wise.resize(num_bonds_for_cluster, dim());
            bond_lengths.resize(num_bonds_for_cluster);

            std::cout<<"6"<<std::endl;

            do {

                std::cout<<"7"<<std::endl;

                A.setZero();

                std::cout<<"8"<<std::endl;

                modify_coords_for_cluster();

                std::cout<<"9"<<std::endl;

                build_A_for_cluster_for_cg();

                std::cout<<"10"<<std::endl;

                std::cout<<"start cg compute"<<std::endl;

                cg.compute(A);

                std::cout<<"end cg compute"<<std::endl;

                for (int axis = 0; axis < dim(); axis++){

                    b.setZero();
                    build_b_for_cluster_for_cg(axis);

                    std::cout<<" start cg solve"<<std::endl;

                    x = cg.solve(b);

                    std::cout << "estimated error: " << cg.error()      << std::endl;


                    modify_coords_after_minimization_for_cluster(axis);

                }

                calculate_bond_lengths_for_cluster();
                max_length = bond_lengths.maxCoeff(&max_row);

                //std::cout<<"max_length = "<<max_length<<std::endl;

                if (max_length > max_length_cutoff){

                    switch_off_bonds({index_to_particles[unique_bonds[max_row].first], index_to_particles[unique_bonds[max_row].second]});
                    //std::cout<<totalClusters_<<","<<index_to_particles[unique_bonds[max_row].first]<<"-"<<index_to_particles[unique_bonds[max_row].second]<<std::endl;
                    //total_lbp++;

                    fprintf(f, "%d,%d-%d,",totalClusters_,index_to_particles[unique_bonds[max_row].first],index_to_particles[unique_bonds[max_row].second]);
                    printf("%d,%d-%d\n",totalClusters_,index_to_particles[unique_bonds[max_row].first],index_to_particles[unique_bonds[max_row].second]);

                    for (int print_axis = 0; print_axis < dim(); print_axis++)
                        fprintf(f, "%lf,", sqrt(bond_lengths_direction_wise(max_row,print_axis)));

                    fprintf(f, "%lf\n", max_length);

                }


            } while(max_length > max_length_cutoff);

        }

        totalClusters_++;


    }

    fclose(f);

}


void postprocessing::build_A_for_cluster_for_cg()
{

    int i,j;
    int index_i, index_j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        /*if (bond_map_status.count({index_i,index_j}) == 0){
            std::cout<<"problem with A"<<std::endl;
        }*/

        if (bond_map_status[{index_i,index_j}] == 1){

            if (i == (num_particles_for_cluster-1)){
                A.coeffRef(j,j) += 1;
            }

            else if (j == (num_particles_for_cluster-1)){
                A.coeffRef(i,i) += 1;
            }

            else{

                A.coeffRef(i,j) -= 1;
                A.coeffRef(j,i) -= 1;
                A.coeffRef(i,i) += 1;
                A.coeffRef(j,j) += 1;

            }

        }


    }

}

void postprocessing::build_b_for_cluster_for_cg(int axis)
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
                b(j) -= dx;
            }

            else if (j == (num_particles_for_cluster-1)){
                b(i) += dx;
            }

            else {
                b(i) += dx;
                b(j) -= dx;
            }

        }


    }

}




}