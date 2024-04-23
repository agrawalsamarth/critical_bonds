#include <post.h>

namespace post_p
{

void postprocessing::dump_percolation_via_invA_file()
{
    percolation_via_inverse();
    dump_percolation_file();
}

void postprocessing::dump_percolation_via_invA_file(char *filename)
{
    percolation_via_inverse();
    dump_percolation_file(filename);
}

void postprocessing::copy_positions()
{

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){

            modified_folded_x(i,axis)  = pos(i,axis);
            modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis)); 

        }
    }



}

void postprocessing::modify_coords()
{

    for (int axis = 0; axis < dim(); axis++)
        ref_pos(axis) = modified_folded_x(numParticles()-1, axis);

    for (int i = 0; i < (numParticles()); i++){

        for (int axis = 0; axis < dim(); axis++){

            modified_folded_x(i,axis)  = modified_folded_x(i,axis) - ref_pos(axis);
            modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis));

            //std::cout<<pos(i,axis)<<" ";

        }

        //std::cout<<"\n";


    }


}

void postprocessing::modify_coords_after_minimization(int axis)
{

    for (int i = 0; i < numParticles() - 1; i++)
        modified_folded_x(i,axis) = (L(axis) * x(i)) + ref_pos(axis);

    modified_folded_x(numParticles()-1,axis) = ref_pos(axis);

    for (int i = 0; i < numParticles(); i++)
        modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis));


}

void postprocessing::build_A()
{

    int i,j;

    for (int index = 0; index < num_bonds; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        if (bond_map_status[{i,j}] == 1){

            if (i == (numParticles()-1)){
                A.coeffRef(j,j) -= 1;
            }

            else if (j == (numParticles()-1)){
                A.coeffRef(i,i) -= 1;
            }

            else{

                A.coeffRef(i,j) += 1;
                A.coeffRef(j,i) += 1;
                A.coeffRef(i,i) -= 1;
                A.coeffRef(j,j) -= 1;

            }

        }


    }


}

void postprocessing::build_b(int axis)
{

    int i,j;
    double dx;

    for (int index = 0; index < num_bonds; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        if (bond_map_status[{i,j}] == 1){

            dx = round((modified_folded_x(i,axis) - modified_folded_x(j,axis))/L(axis));

            if (i == (numParticles()-1)){
                b(j) += dx;
            }

            else if (j == (numParticles()-1)){
                b(i) -= dx;
            }

            else {
                b(i) -= dx;
                b(j) += dx;
            }

        }


    }

}

void postprocessing::calculate_bond_lengths_direction_wise(int axis)
{

    //bond_distance.setZero();
    int i,j;

    for (int index = 0; index < num_bonds; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        if (bond_map_status[{i,j}] == 1){
            //bond_lengths(index) = get_periodic_image(pos(i,axis)-pos(j,axis), axis) * get_periodic_image(pos(i,axis)-pos(j,axis), axis);
            bond_lengths_direction_wise(index, axis)  = (modified_folded_x(i, axis) - modified_folded_x(j, axis));
            bond_lengths_direction_wise(index, axis) -= (L(axis) * round(bond_lengths_direction_wise(index, axis)/L(axis)));
            bond_lengths_direction_wise(index, axis) *= bond_lengths_direction_wise(index, axis); 

            /*if (bond_lengths_direction_wise(index,axis) > (0.2 * L(axis) * L(axis))){
                std::cout<<"faaaaaiiiilllll!!!!"<<std::endl;
            }*/

        }

        else {
            bond_lengths_direction_wise(index, axis)  = 0.;
        }

    }


}

void postprocessing::calculate_bond_lengths()
{

    bond_lengths_direction_wise.setZero();
    bond_lengths.setZero();

    for (int axis = 0; axis < dim(); axis++){
        calculate_bond_lengths_direction_wise(axis);
    }

    for (int i = 0; i < num_bonds; i++){
        for (int axis = 0; axis < dim(); axis++) {

            bond_lengths(i) += bond_lengths_direction_wise(i,axis);

        }

        bond_lengths(i) = sqrt(bond_lengths(i));

    }


}

void postprocessing::percolation_via_inverse()
{

    reset_bond_map(true);

    num_bonds = 0;
    
    for (int i = 0; i < numParticles(); i++){
        for (int j = 0; j < numAttachments(i); j++){

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(attachment(i,j),i)) == unique_bonds.end()) {

                unique_bonds.push_back({i, attachment(i,j)});
                num_bonds++;

            }

        }
    }

    modified_folded_x.resize(numParticles(),dim());
    ref_pos.resize(dim());
    copy_positions();
    modify_coords();

    A.resize((numParticles()-1), (numParticles()-1));
    //int total_zeroes = ((numParticles()-1) * (numParticles()-1)) - (numParticles() - 1) - (2 * num_bonds);
    //A.reserve(total_zeroes);

    build_A();

    //std::cout<<"A built"<<std::endl;

    if (!A.isCompressed()){
        A.makeCompressed();
    }

    /*for (int i = 0; i < A.rows(); i++) {
        for (int j = 0; j < A.rows(); j++){
            std::cout<<A.coeff(i,j)<<" ";
        }
        std::cout<<"\n";
    }

    std::cout<<"-----------"<<std::endl;*/

    solver.analyzePattern(A);

    //std::cout<<"analyzed"<<std::endl;

    solver.factorize(A);

    //std::cout<<"factorized"<<std::endl;

    b.resize((numParticles()-1));

    //std::cout<<"b resized"<<std::endl;

    x.resize((numParticles()-1));
    bond_lengths_direction_wise.resize(num_bonds, dim());
    bond_lengths.resize(num_bonds);

    //std::cout<<"x resized"<<std::endl;

        //std::cout<<"A analyzed"<<std::endl;

    double max_length;
    int max_row, max_col;

    for (int axis = 0; axis < dim(); axis++){


        //std::cout<<"loop enetered"<<std::endl;

        b.setZero();

        //std::cout<<"b set to 0"<<std::endl;

        build_b(axis);

        /*for (int i = 0; i < b.rows(); i++)
            std::cout<<b(i)<<" ";

        std::cout<<"\n";*/

            //std::cout<<"b built"<<std::endl;

        x.setZero();
        x = solver.solve(b);

        /*std::cout<<"inv(A)*b="<<std::endl;

        for (int i = 0; i < numParticles()-1; i++)
            std::cout<<x(i)<<" ";

        std::cout<<"\n------------\n";*/

        //std::cout<<"norm = "<<x.norm()<<std::endl;

        modify_coords_after_minimization(axis);

        /*std::cout<<"x_new="<<std::endl;

        for (int i = 0; i < numParticles(); i++)
            std::cout<<modified_folded_x(i)<<" ";

        std::cout<<"\n------------\n";*/

        bond_lengths_direction_wise.setZero();
        calculate_bond_lengths_direction_wise(axis);

        max_length = bond_lengths_direction_wise.maxCoeff(&max_row, &max_col);

        if (max_length > 1e-10){
            cluster_percolation(0,axis) = 1;
        }

        else {
            cluster_percolation(0,axis) = 0;
        }


    }

    //std::cout<<"---------------"<<std::endl;

    totalClusters_ = 1;


}

void postprocessing::determine_LB_bonds()
{

    build_bond_map();
    reset_bond_map(true);

    num_bonds = 0;
    
    for (int i = 0; i < numParticles(); i++){
        for (int j = 0; j < numAttachments(i); j++){

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(attachment(i,j),i)) == unique_bonds.end()) {

                unique_bonds.push_back({i, attachment(i,j)});
                num_bonds++;

            }

        }
    }

    modified_folded_x.resize(numParticles(),dim());
    ref_pos.resize(dim());
    copy_positions();

    //modify_coords();

    A.resize((numParticles()-1), (numParticles()-1));
    b.resize((numParticles()-1));
    x.resize((numParticles()-1));
    //check_for_solver_mk.resize((numParticles()-1));
    bond_lengths_direction_wise.resize(num_bonds, dim());
    bond_lengths.resize(num_bonds);

    double max_length;
    int max_row, max_col;

    int total_lbp = 0;


    do {

        A.setZero();
        modify_coords();
        build_A();

        if (!A.isCompressed()){
            A.makeCompressed();
        }

        solver.analyzePattern(A);
        solver.factorize(A);

        for (int axis = 0; axis < dim(); axis++){

            b.setZero();
            build_b(axis);
            x = solver.solve(b);
            modify_coords_after_minimization(axis);

        }

        calculate_bond_lengths();
        max_length = bond_lengths.maxCoeff(&max_row);

        if (max_length > 1e-10){

            for (int print_i = 0; print_i < unique_bonds.size(); print_i++)
                std::cout<<bond_lengths[print_i]<<std::endl;

            switch_off_bonds(unique_bonds[max_row]);
            std::cout<<unique_bonds[max_row].first<<"-"<<unique_bonds[max_row].second<<std::endl;
            total_lbp++;

        }


    } while(max_length > 1e-10);


}


}