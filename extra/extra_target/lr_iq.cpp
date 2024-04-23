#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    if (argc < 6){
        std::cout<<"Too few arguments, please provide <config_filename> <q_min> <q_max> <num_q> <bin_size>"<<std::endl;
        return 0;
    }

    double q_min    = atof(argv[2]);
    double q_max    = atof(argv[3]);
    double num_q    = atoi(argv[4]);
    double bin_size = atof(argv[5]);
    
    //if (argc < 7)
        //test.dump_lr_scattering_function(q_min, q_max, num_q, bin_size);
    //else
    test.dump_lr_scattering_function(q_min, q_max, num_q, bin_size, argv[6]);
        
    return 0;
}