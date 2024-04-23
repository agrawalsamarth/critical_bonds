#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);

    if (argc == 4)
        test->dump_chemical_distance(argv[2], argv[3]);

    else
        std::cout<<"please use program in following format chemical_distance <config_filename> <lb_bonds_filename> <cd_filename>"<<std::endl;

    return 0;
}