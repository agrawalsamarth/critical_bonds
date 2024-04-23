#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    if (argc < 3)
        test.dump_unfolded_file();

    else
        test.dump_unfolded_file(argv[2]);

    return 0;
}