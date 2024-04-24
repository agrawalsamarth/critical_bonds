#include <post.h>

namespace post_p
{

void postprocessing::check_if_line(std::ifstream &temp_parser, std::string &temp_str, int &temp_count)
{
    try{
        getline(temp_parser,temp_str);
    }

    catch (...){
        std::cout<<"line number "<<temp_count<<" is missing"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

void postprocessing::read_config_parser(char *config_filename)
{
    std::ifstream parser(config_filename, std::ifstream::in);
    std::string str;
    std::vector<std::string> results;
    int count = 1;
    
    check_if_line(parser, str, count);
    
    try{
        D_ = stoi(str);
        count++;
    }

    catch (...) {
        std::cout<<"Line 1 should be number of dimensions only"<<std::endl;
        exit(EXIT_FAILURE);
    }

    check_if_line(parser, str, count);
    count++;
    results   = split_string_by_delimiter(str, ' ');

    lo_hi_    = (double*)malloc(sizeof(double)*D_*2);
    L_        = (double*)malloc(sizeof(double)*D_);
    halfL_    = (double*)malloc(sizeof(double)*D_);
    periodic_ = (int*)malloc(sizeof(int)*D_);

    for (int i = 0; i < D_; i++){

        try{
            lo_hi_[2*i] = stod(results[2*i]);
        }

        catch (...){
            std::cout<<"lo value of direction "<<(i+1)<<" is either not specified or not a float value"<<std::endl;
        }

        try{
            lo_hi_[2*i+1] = stod(results[2*i+1]);
        }

        catch (...){
            std::cout<<"hi value of direction "<<(i+1)<<" is either not specified or not a float value"<<std::endl;
        }        

        //lo_hi_[2*i]     = stod(results[2*i]);
        //lo_hi_[2*i + 1] = stod(results[2*i+1]);
        L_[i]           = stod(results[2*i+1]) - stod(results[2*i]);
        halfL_[i]       = 0.5*L_[i];
        periodic_[i]    = 1;
    }

    check_if_line(parser, str, count);
    
    try{
        N_   = stoi(str);
        count++;
    }

    catch(...){
        std::cout<<"Line "<<count<<" should be the number of nodes in integer value only"<<std::endl;
        exit(EXIT_FAILURE);
    }

    pos_ = (double*)malloc(sizeof(double)*N_*D_);
    
    for (int i = 0; i < N_; i++){
        check_if_line(parser, str, count);
        results = split_string_by_delimiter(str, ' ');

        for (int axis = 0; axis < D_; axis++){
            try{
                pos(i, axis) = stod(results[axis]);
            }
            catch(...){
                std::cout<<"column "<<(axis+1)<<" of Line "<<count<<" is either not specified or not a float value";
                exit(EXIT_FAILURE);
            }
        }

        count++;
    }

    check_if_line(parser, str, count);

    try{
        num_bonds = stoi(str);
        count++;
    }

    catch(...){
        std::cout<<"Line number "<<count<<" should be bumber of bonds in integer value only"<<std::endl;
        exit(EXIT_FAILURE);
    }

    attachment_.resize(N_);
    num_attachments_ = (int*)malloc(sizeof(int)*N_);

    int att_i, att_j;

    for (int i = 0; i < num_bonds; i++){
        check_if_line(parser, str, count);
        results = split_string_by_delimiter(str, ' ');

        try{
            att_i = stoi(results[0]);
        }
        catch(...){
            std::cout<<"column 1 of Line number "<<count<<" is either not specified or not an integer"<<std::endl;
            exit(EXIT_FAILURE);
        }

        try{
            att_j = stoi(results[1]);
        }
        catch(...){
            std::cout<<"column 2 of Line number "<<count<<" is either not specified or not an integer"<<std::endl;
            exit(EXIT_FAILURE);
        }

        attachment_[att_i].push_back(att_j);
        attachment_[att_j].push_back(att_i);
        count++;
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