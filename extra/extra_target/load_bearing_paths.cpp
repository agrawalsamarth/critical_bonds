#include <post.h>

int main(int argc, char *argv[])
{
    //post_p::postprocessing test(argv[1]);

    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);

    //test->init_unfolding_for_lbp();

    test->martin_test(argv[2]);

    /*if (argc < 3)
        test->dump_load_bearing_paths_file();

    else
        test->dump_load_bearing_paths_file(argv[2]);*/

    return 0;
}