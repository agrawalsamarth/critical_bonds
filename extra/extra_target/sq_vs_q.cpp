#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    if (argc < 5){
        std::cout<<"Too few arguments, please provide <config_filename> <q_min> <q_max> <num_q>"<<std::endl;
        return 0;
    }

    double q_min = atof(argv[2]);
    double q_max = atof(argv[3]);
    double num_q = atoi(argv[4]);
    
    if (argc < 6)
        test.dump_scattering_function(q_min, q_max, num_q);
    else
        test.dump_scattering_function(q_min, q_max, num_q, argv[5]);
        
    return 0;
}