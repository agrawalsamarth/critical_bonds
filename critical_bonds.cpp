#include <post.h>
#include <ctime>

int main(int argc, char *argv[])
{
    if (argc < 3){
        std::cout<<"please use the program as -- critical_bonds <configuration_filename> <results_filename>"<<std::endl;
        return 0;
    }

    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);
    test->dump_lb_bonds_for_cluster_via_invA(argv[2]);
    

    delete test;
    return 0;
}