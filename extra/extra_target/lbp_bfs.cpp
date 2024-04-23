#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);

    if (argc == 4)
        test->find_paths_for_all_bonds(argv[2], argv[3]);

    else
        std::cout<<"please use program in following format lbp_bfs <config_filename> <lb_bonds_filename> <cd_filename>"<<std::endl;

    return 0;
}