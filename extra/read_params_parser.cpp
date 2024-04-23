#include <post.h>

namespace post_p
{

void postprocessing::read_params_parser(char *config_filename)
{

    std::ifstream parser(config_filename, std::ifstream::in);
    int count = 0;

    if (parser.fail()){
        std::cout<<"either file does not exist or doesn't have permission"<<std::endl;
        exit(EXIT_FAILURE);
    }

    std::string str;
    std::vector<std::string> results;

    getline(parser, str);
    
    results  = split_string_by_delimiter(str, '=');
    headers_ = stoi(results[1]);
    count++;

    while (count < headers_){

        getline(parser, str);
        results = split_string_by_delimiter(str, '=');

        if (results[0] == "N"){
            N_      = stoi(results[1]);
            N_flag_ = true;
        }

        if (results[0] == "D"){
            D_      = stoi(results[1]);
            D_flag_ = true;
        }

        if (results[0] == "maxAttachments"){
            max_attachments_      = stoi(results[1]);
            max_attachments_flag_ = true;
        }

        if (results[0] == "folded"){
            folded_      = stoi(results[1]);
            folded_flag_ = true;
        }

        if (results[0] == "lattice"){
            lattice_      = stoi(results[1]);
            lattice_flag_ = true;
        }

        if (results[0] == "iters"){
            iters_      = stoi(results[1]);
            iters_flag_ = true;
        }

        if (results[0] == "phi"){
            phi_      = stod(results[1]);
            phi_flag_ = true;
        }

        if (results[0] == "alpha"){
            alpha_      = stod(results[1]);
            alpha_flag_ = true;
        }

        if (results[0] == "seedMass"){
            seed_mass_      = stod(results[1]);
            seed_mass_flag_ = true;
        }

        if (results[0] == "stiffness"){
            stiffness_inv_new = stod(results[1]);
        }


        count++;


    }

    parser.close();

    std::vector<std::string> periodic_names;
    std::vector<std::string> lo_names;
    std::vector<std::string> hi_names;

    lo_hi_           = (double*)malloc(sizeof(double) * dim() * 2);
    L_               = (double*)malloc(sizeof(double) * dim());
    halfL_           = (double*)malloc(sizeof(double) * dim());
    periodic_        = (int*)malloc(sizeof(int) * dim());

    for (int axis = 0; axis < dim(); axis++){
        periodic_names.push_back("x"+std::to_string(axis)+"_periodic");
        lo_names.push_back("x"+std::to_string(axis)+"_lo");
        hi_names.push_back("x"+std::to_string(axis)+"_hi");
    }


    parser.open(config_filename, std::ifstream::in);
    count = 0;

    while (count < headers_){

        getline(parser, str);
        results = split_string_by_delimiter(str, '=');

        for (int axis = 0; axis < dim(); axis++){

            if (results[0] == periodic_names[axis])
                periodic_[axis] = stoi(results[1]);

        }

        for (int axis = 0; axis < dim(); axis++){

            if (results[0] == lo_names[axis])
                lo_hi(axis,0) = stod(results[1]);

        }

        for (int axis = 0; axis < dim(); axis++){

            if (results[0] == hi_names[axis])
                lo_hi(axis,1) = stod(results[1]);

        }

        count++;
    }

    parser.close();

    for (int axis = 0; axis < dim(); axis++){
        L(axis)     = lo_hi(axis,1) - lo_hi(axis,0);
        halfL(axis) = 0.5 * L(axis); 
    }

    N_pairs_ = (0.5 * numParticles() * (numParticles() -1));

}


}
