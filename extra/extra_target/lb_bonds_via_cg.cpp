#include <post.h>
#include <chrono>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);
    FILE *f_time;
    clock_t begin;
    clock_t end;

    if (argc < 3){
        std::cout<<"please provide config_filename results_filename"<<std::endl;
        return 0;
    }

    else if (argc == 3)
        test->dump_lb_bonds_for_cluster_via_cg(argv[2]);

    else{

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        test->dump_lb_bonds_for_cluster_via_cg(argv[2]);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

        double time_diff = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        time_diff        = time_diff * 1e-3;

        f_time = fopen(argv[3],"w");
        fprintf(f_time, "cpu_time\n");
        fprintf(f_time, "%lf\n", time_diff);
        fclose(f_time);

    }

    return 0;
}