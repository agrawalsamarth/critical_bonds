#include <post.h>

namespace post_p
{

void postprocessing::read_config_parser(char *config_filename)
{
    std::ifstream parser(config_filename, std::ifstream::in);
    std::string str;
    std::vector<std::string> results;
    int count = 1;
    
    getline(parser,str);

    try{
        D_ = stoi(str);
        count++;
    }

    catch (...) {
        std::cout<<"Line 1 should be number of dimensions only"<<std::endl;
        exit(EXIT_FAILURE);
    }

    getline(parser, str);
    results   = split_string_by_delimiter(str, ' ');

    

    lo_hi_    = (double*)malloc(sizeof(double)*D_*2);
    L_        = (double*)malloc(sizeof(double)*D_);
    halfL_    = (double*)malloc(sizeof(double)*D_);
    periodic_ = (int*)malloc(sizeof(int)*D_);

    for (int i = 0; i < D_; i++){
        lo_hi_[2*i]     = stod(results[2*i]);
        lo_hi_[2*i + 1] = stod(results[2*i+1]);
        L_[i]           = stod(results[2*i+1]) - stod(results[2*i]);
        std::cout<<L_[i]<<std::endl;
        halfL_[i]       = 0.5*L_[i];
        periodic_[i]    = 1;
    }

    getline(parser, str);
    N_   = stoi(str);
    pos_ = (double*)malloc(sizeof(double)*N_*D_);
    
    node_info = (bool*)malloc(sizeof(bool)*N_);

    for (int i = 0; i < N_; i++)
        node_info[i] = false;

    int id;

    for (int i = 0; i < N_; i++){
        getline(parser, str);
        results = split_string_by_delimiter(str, ' ');

        if (results.size() != (D_+1)){
            std::cout<<""<<std::endl;
        }


        id = stoi(results[0]);

        for (int axis = 0; axis < D_; axis++){
            pos(id, axis) = stod(results[axis+1]);
        }
    }

    getline(parser, str);
    num_bonds = stoi(str);

    attachment_.resize(N_);
    num_attachments_ = (int*)malloc(sizeof(int)*N_);

    int att_i, att_j;

    for (int i = 0; i < num_bonds; i++){
        getline(parser, str);
        results = split_string_by_delimiter(str, ' ');

        att_i = stoi(results[0]);
        att_j = stoi(results[1]);

        attachment_[att_i].push_back(att_j);
        attachment_[att_j].push_back(att_i);
    }

    for (int i = 0; i < N_; i++){
        num_attachments_[i] = attachment_[i].size();
    }

    parser.close();

    posDiff_                  = (double*)malloc(sizeof(double) * dim());
    is_placed_                = (bool*)malloc(sizeof(bool) * numParticles());
    attachments_placed_       = (bool*)malloc(sizeof(bool) * numParticles());


 
}


}