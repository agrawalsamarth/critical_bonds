#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    bool percolation_test;
    percolation_test = test.check_percolation();

    if(percolation_test){
        std::cout<<"cluster is percolating, radius of gyration is undefined."<<std::endl;
        return 0;
    }

    else {
        
        if (test.totalClusters() == 1){
            if (argc < 3)
                test.dump_rog();
            else
                test.dump_rog(argv[2]);
        }

        else {
            std::cout<<"there are multiple clusters, radius of gyration is undefined"<<std::endl;
        }

    }

    return 0;
}