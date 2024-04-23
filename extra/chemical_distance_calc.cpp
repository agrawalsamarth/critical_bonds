#include <post.h>

namespace post_p
{

void postprocessing::dump_chemical_distance(char *lb_file, char *cd_filename)
{

    std::ifstream parser(lb_file, std::ifstream::in);
    std::string str;
    std::vector<std::string> results;
    std::vector<std::pair<int,int>> lb_bonds;

    getline(parser,str);
    
    while (getline(parser,str)){

        results = split_string_by_delimiter(str, ',');
        str = results[1];
        results = split_string_by_delimiter(str, '-');
        lb_bonds_str.push_back(str);
        lb_bonds.push_back({stoi(results[0]), stoi(results[1])});

    }

    chemical_distance_index = (int*)malloc(sizeof(int)*numParticles());
    in_path = (bool*)malloc(sizeof(bool)*numParticles());

    for (int i = 0; i < numParticles(); i++)
        in_path[i] = false;

    build_bond_map();

    //for (int i = 0; i < lb_bonds.size(); i++)
        //switch_off_bonds(lb_bonds[i]);

    for (int j = 0; j < lb_bonds.size(); j++){
            switch_off_bonds(lb_bonds[j]);
    }

    for (int i = 0; i < lb_bonds.size(); i++){
        
        //std::cout<<"1"<<std::endl;
        reset_chemical_distance_index();
        //std::cout<<"2"<<std::endl;
        //switch_off_bonds(lb_bonds[i]);
        //std::cout<<"3"<<std::endl;
        find_chemical_distance(lb_bonds[i].first, lb_bonds[i].second);
        //std::cout<<"4"<<std::endl;
        //switch_on_bonds(lb_bonds[i]);
        //std::cout<<"5"<<std::endl;
    
    }

    std::ofstream f;
    f.open(cd_filename);

    f << "bond,lbp\n";

    for (int i = 0; i < lb_bonds_str.size(); i++){
        f << lb_bonds_str[i] << "," << full_lb_list[i] << "\n";
    }

    f.close();

    free(chemical_distance_index);
    free(in_path);

}

void postprocessing::reset_chemical_distance_index()
{

    for (int i = 0; i < numParticles(); i++)
        chemical_distance_index[i] = -1;

}

void postprocessing::find_chemical_distance(int i, int j)
{

    //std::cout<<"1"<<std::endl;

    ref_nodes.clear();
    neighbour_nodes.clear();

    ref_nodes.push_back(i);
    chemical_distance_index[i] = 0;

    int current_index = 1;
    int ref_particle;
    int neigh_particle;

    //std::cout<<"2"<<std::endl;

    while (chemical_distance_index[j] == -1){

        for (int index = 0; index < ref_nodes.size(); index++){

            ref_particle = ref_nodes[index];

            for (int att = 0; att < numAttachments(ref_particle); att++){

                neigh_particle = attachment(ref_particle, att);

                if (bond_map_status[{ref_particle, neigh_particle}] == 1){

                    if (chemical_distance_index[neigh_particle] == -1){
                        chemical_distance_index[neigh_particle] = current_index;
                        neighbour_nodes.push_back(neigh_particle);
                    }

                }

            }

        }

        ref_nodes.clear();

        for (int index = 0; index < neighbour_nodes.size(); index++)
            ref_nodes.push_back(neighbour_nodes[index]);

        neighbour_nodes.clear();
        current_index++;

    }

    //std::cout<<"3"<<std::endl;

    find_shortest_path(i, j);

    //std::cout<<"4"<<std::endl;

}

void postprocessing::find_shortest_path(int i, int j)
{

    //std::cout<<"1"<<std::endl;

    int last_index = chemical_distance_index[j];

    //std::cout<<"last index at start = "<<last_index<<std::endl;

    std::vector<int> node_series;

    int ref_particle = j;
    node_series.push_back(ref_particle);
    int neigh_particle;

    //std::cout<<"2"<<std::endl;

    while (last_index != 0){

        for (int att = 0; att < numAttachments(ref_particle); att++){

            neigh_particle = attachment(ref_particle, att);

            if (bond_map_status[{ref_particle, neigh_particle}] == 1){

                if ((chemical_distance_index[neigh_particle] != -1) && (chemical_distance_index[neigh_particle] < last_index)){

                    //switch_off_bonds({ref_particle, neigh_particle});
                    ref_particle = neigh_particle;
                    last_index = chemical_distance_index[neigh_particle];
                    node_series.push_back(ref_particle);
                    break;
                }

            }

        }

        //std::cout<<"last_index = "<<last_index<<std::endl;

    }

    //std::cout<<"3"<<std::endl;

    std::string lb_list = "";    

    for (int index = 0; index < (node_series.size() - 1); index++){
        lb_list += std::to_string(node_series[index])+"-";
    }

    lb_list += std::to_string(node_series[(node_series.size()-1)]);

    full_lb_list.push_back(lb_list);

    //std::cout<<"4"<<std::endl;

}



}