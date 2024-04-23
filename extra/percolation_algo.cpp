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

        for (int axis = 0; axis < dim(); axis++)
            unfolded_coords(j,axis) = 0.;

    }

    for (int axis = 0; axis < dim(); axis++)
        temp_lbp_[axis] = 0;

}

void postprocessing::init_unfolding_for_lbp()
{

    build_bond_map();

}

void postprocessing::martin_test(char *lb_file)
{

    reset_bond_map(true);

    std::ifstream parser(lb_file, std::ifstream::in);
    std::string str;
    std::vector<std::string> results;
    std::vector<std::pair<int,int>> lb_bonds;
    getline(parser,str);

    //std::vector<std::pair<int,int>> test_bonds;
    
    while (getline(parser,str)){

        results = split_string_by_delimiter(str, ',');
        str = results[0];
        //std::cout<<str<<std::endl;
        results = split_string_by_delimiter(str, '-');
        std::cout<<stoi(results[0])<<"\t"<<(results[1])<<std::endl;
        lb_bonds_str.push_back(str);
        lb_bonds.push_back({stoi(results[0]), stoi(results[1])});

    }

    parser.close();

    

    for (int i = 0; i < lb_bonds.size(); i++)
        switch_off_bonds(lb_bonds[i]);

    path_percolation = false;
    reset_unfolding_params();

    int test_cluster_number = 0;

    for (int i = 0; i < numParticles(); i++){

        if (is_placed(i) == false){
            //std::cout<<"i="<<i<<std::endl;
            unfold_for_lbp(i, i, false);
            test_cluster_number++;
        }

    }

    std::cout<<"total clusters = "<<test_cluster_number<<std::endl;

    if (path_percolation)
        std::cout<<"still percolating"<<std::endl;

    if (!path_percolation)
        std::cout<<"no percolation at all"<<std::endl;


}

void postprocessing::makeCombiUtil(int n, int left, int k)
{
	// Pushing this vector to a vector of vector

    //std::cout<<"axis = "<<axis<<std::endl;

	if (k == 0) {

        if (path_percolation){
            
            reset_bond_map(true);
            
            for (int index = 0; index < tmp.size(); index++)
                switch_off_bonds(unique_bonds[tmp[index]]);

            path_percolation = false;
            reset_unfolding_params();

            for (int index = 0; index < numParticles(); index++){

                if (is_placed(index) == false){
                    //to_build_list.clear();
                    num_particles_for_cluster = 0;
                    to_build_list[num_particles_for_cluster] = index;
                    num_particles_for_cluster++;
                    is_placed(index) = true;
                    
                    //unfold_for_lbp(index, index, false);
                    unfold_for_lbp_without_recursion(index);

                    for (int iter_index = 1; iter_index < num_particles_for_cluster; iter_index++)
                        unfold_for_lbp_without_recursion(to_build_list[iter_index]);

                }

                if (path_percolation)
                    break;

            }

            if (!path_percolation){

                for (int index = 0; index < tmp.size(); index++)
                    ans.push_back(tmp[index]);

            }

        }
        
		return;
	}

	// i iterates from left to n. First time
	// left will be 1
	for (int i = left; i < n; ++i)
	{
        if (!path_percolation)
            return;
        
		tmp.push_back(i);
		makeCombiUtil(n, i + 1, k - 1);

		// Popping out last inserted element
		// from the vector
		tmp.pop_back();

	}

}

// Prints all combinations of size k of numbers
// from 1 to n.
void postprocessing::makeCombi(int n, int k)
{
	makeCombiUtil(n, 0, k);
}

void postprocessing::lbp_brute_force()
{

    //makeCombi(5,3);

    reset_bond_map(true);
    to_build_list.resize(numParticles());

    int num_bonds = 0;
    int bond_sum = 0;

    for (int i = 0; i < numParticles(); i++){
        for (int j = 0; j < numAttachments(i); j++){

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(attachment(i,j),i)) == unique_bonds.end()) {

                unique_bonds.push_back({i, attachment(i,j)});
                num_bonds++;

            }

        }
    }

    path_percolation = true;

    std::cout<<"bond"<<std::endl;

    //for (int axis = 0; axis < dim(); axis++){

        reset_bond_map(true);
        path_percolation = true;
        ans.clear();

        for (int i = 0; i < num_bonds; i++) {

            //std::cout<<"i = "<<i<<std::endl;
            //makeCombi(num_bonds, i);
            comb(num_bonds, i);

            if (!path_percolation){

                for (int k = 0; k < ans.size(); k++)
                    std::cout<<unique_bonds[ans[k]].first<<"-"<<unique_bonds[ans[k]].second<<"\n";

                break;

            }

        }

    //}

}

void postprocessing::comb(int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
 
    // print integers and permute bitmask
    do {

        reset_bond_map(true);
        ans.clear();

        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]){ 
                switch_off_bonds(unique_bonds[i]);
                ans.push_back(i);
            }
        }

        path_percolation = false;
        reset_unfolding_params();

        for (int index = 0; index < numParticles(); index++){

            if (is_placed(index) == false){
                //to_build_list.clear();
                num_particles_for_cluster = 0;
                to_build_list[num_particles_for_cluster] = index;
                num_particles_for_cluster++;
                is_placed(index) = true;
                
                //unfold_for_lbp(index, index, false);
                unfold_for_lbp_without_recursion(index);

                for (int iter_index = 1; iter_index < num_particles_for_cluster; iter_index++)
                    unfold_for_lbp_without_recursion(to_build_list[iter_index]);

            }

            if (path_percolation)
                break;

        }

        if (!path_percolation){
            break;
        }   

        //std::cout << std::endl;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

}

void postprocessing::unfold_for_lbp_without_recursion(int i)
{
    
    for (int att = 0; att < numAttachments(i); att++)
    {

        index_j = attachment(i, att);

        if (bond_map_status[{i, index_j}] == 1)
        {
            if (is_placed(index_j) == false)
            {
                    is_placed(index_j) = true;

                    for (int axis = 0; axis < dim(); axis++){
                        posDiff(axis) = get_periodic_image(pos(index_j,axis) - pos(i,axis), axis);
                        unfolded_coords(index_j, axis) = unfolded_coords(i,axis) + posDiff(axis);
                    }

                    to_build_list[num_particles_for_cluster] = index_j;
                    num_particles_for_cluster++;
            }

            else
            {

                for (int axis = 0; axis < dim(); axis++){

                    posDiff(axis) = unfolded_coords(index_j, axis) - unfolded_coords(i,axis);

                    if ((posDiff(axis) > halfL(axis)) || (posDiff(axis) < -halfL(axis)))
                        path_percolation = true;

                }

            }
        }

    }

}

void postprocessing::unfold_for_lbp(const int prev, const int next, bool build_pb)
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

    if (build_pb)
        path_components.push_back(next);

    for (int att = 0; att < numAttachments(next); att++)
    {

        //if (path_percolation)
            //break;

        temp_next = attachment(next, att);

        if (bond_map_status[{next, temp_next}] == 1){

            if ((build_pb == true)) {

                    if(std::find(current_path.begin(), current_path.end(), std::make_pair(temp_next, next)) == current_path.end()) {
                        current_path.push_back({next, temp_next});
                    } 

            }
        
            if (is_placed(temp_next) == false){
                unfoldedAttachments(next)      = unfoldedAttachments(next) + 1;
                unfoldedAttachments(temp_next) = unfoldedAttachments(temp_next) + 1;
                unfold_for_lbp(next, temp_next, build_pb);
            }

            else {
                
                for (int axis = 0; axis < dim(); axis++) {

                    posDiff(axis) = unfolded_coords(temp_next, axis) - unfolded_coords(next, axis);

                    if ((posDiff(axis) > halfL(axis)) || (posDiff(axis) < -halfL(axis))){
                        cluster_percolation(total_lbp,axis) = 1;
                        temp_lbp_[axis]++;

                        //if (axis == per_axis){
                            path_percolation = true;
                            //std::cout<<"axis = "<<axis<<"per_axis = "<<per_axis<<std::endl;
                        //}
                    }

                }

            }

        }

    }

}

}