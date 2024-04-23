#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);

    if (argc < 3)
        test->dump_percolation_via_invA_file();

    else
        test->dump_percolation_via_invA_file(argv[2]);

    return 0;
}