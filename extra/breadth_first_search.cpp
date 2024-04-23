#include <post.h>

namespace post_p
{

void postprocessing::find_paths_for_all_bonds(char *lb_file, char *cd_filename)
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

    parser.close();

    in_path = (bool*)malloc(sizeof(bool)*numParticles());

    for (int i = 0; i < numParticles(); i++)
        in_path[i] = false;

    reset_bond_map(true);

    for (int j = 0; j < lb_bonds.size(); j++){
        switch_off_bonds(lb_bonds[j]);
    }

    f_bfs = fopen(cd_filename, "w");
    fprintf(f_bfs, "bond,path_number,lbp\n");

    for (int i = 0; i < lb_bonds.size(); i++){
        find_all_paths(lb_bonds[i].first, lb_bonds[i].second);
    }

    fclose(f_bfs);


}

void postprocessing::find_all_paths(int source, int destination)
{

    for (int i = 0; i < numParticles(); i++)
        in_path[i] = false;

    source_to_destination_path.clear();
    current_path_index = 0;

    find_all_paths_recursion(source, destination);


}

void postprocessing::find_all_paths_recursion(int source, int destination)
{

    in_path[source] = true;
    source_to_destination_path.push_back(source);

    if (destination == source){

        fprintf(f_bfs, "%d-%d,%d,", source_to_destination_path[0], source_to_destination_path.back(), current_path_index);

        for (int index = 0; index < (source_to_destination_path.size() - 1); index++)
            fprintf(f_bfs, "%d-", source_to_destination_path[index]);

        fprintf(f_bfs, "%d\n", source_to_destination_path.back());

        current_path_index++;

    }

    else {

        for (int i = 0; i < numAttachments(source); i++){

            if ((in_path[attachment(source, i)] == false) && (bond_map_status[{source, attachment(source, i)}] == 1))
                find_all_paths_recursion(attachment(source,i), destination);

        }

    }

    in_path[source] = false;
    source_to_destination_path.pop_back();

}


}