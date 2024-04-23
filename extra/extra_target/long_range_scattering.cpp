#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    if (argc < 7){
        std::cout<<"Too few arguments"<<std::endl;
        return 0;
    }

    test.calc_long_range_iq(std::stoi(argv[2]), std::stod(argv[3]), std::stod(argv[4]), std::stoi(argv[5]), argv[6]);
    
    return 0;
}