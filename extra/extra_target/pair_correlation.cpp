#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    if (argc < 3){
        std::cout<<"Too few arguments, please provide <config_filename> <bin_size>"<<std::endl;
        return 0;
    }

    double bin_size = atof(argv[2]);

    if (argc < 4)
        test.dump_rij_hist_file(bin_size);
    else
        test.dump_rij_hist_file(bin_size, argv[3]);
    
    return 0;
}