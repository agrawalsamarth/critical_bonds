#include <post.h>

int main(int argc, char *argv[])
{
    //post_p::postprocessing test(argv[1]);

    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);

    test->lbp_brute_force();

    return 0;
}