#include <post.h>
#include <ctime>

int main(int argc, char *argv[])
{
    /*std::chrono::steady_clock::time_point cp_1;
    std::chrono::steady_clock::time_point cp_2;
    std::chrono::steady_clock::time_point cp_2;
    std::chrono::steady_clock::time_point cp_3;
    std::chrono::steady_clock::time_point cp_4;

    double total_time = 0.;
    double read_time = 0.;

    cp_1 = std::chrono::steady_clock::now();

    cp_3 = std::chrono::steady_clock::now();*/
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);
    //cp_4 = std::chrono::steady_clock::now();

    //read_time  += std::chrono::duration_cast<std::chrono::nanoseconds>(cp_4 - cp_3).count();

    //std::cout<<"read time = "<<read_time * 1e-9<<std::endl;

    //FILE *f_time;

    //cp_1 = std::chrono::steady_clock::now();
    
    if (argc < 3){
        std::cout<<"please provide config_filename results_filename"<<std::endl;
        return 0;
    }

    else{
        test->dump_lb_bonds_for_cluster_via_invA(argv[2]);
        //test->dump_LB_bonds_for_network(argv[2]);
    }

    //cp_2 = std::chrono::steady_clock::now();    
    //double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cp_2 - cp_1).count();
    //std::cout<<"total time = "<<total_time * 1e-9<<std::endl;

    delete test;
    return 0;
}