#include <post.h>

namespace post_p{

postprocessing::postprocessing(char *config_filename)
{
    //read_params_parser(config_filename);
    //memory_allocation_function();
    read_config_parser(config_filename);

    //generator.seed(0);
    //dis.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));
}

postprocessing::~postprocessing()
{

        free(lo_hi_);
        free(L_);
        free(halfL_);
        free(periodic_);
        free(pos_);
        free(num_attachments_);
        /*free(original_seed_);
        free(current_seed_);
        if (max_attachments_ > 0)
            free(attachment_);
        free(diameter_);*/
        free(posDiff_);

        //free(r_ij_);
        free(is_placed_);
        /*free(unfolded_coords_);
        free(unfolded_num_attachments_);
        free(cluster_percolation_);
        free(radius_);*/
        free(attachments_placed_);
        //free(load_bearing_paths_);
        //free(temp_lbp_);

}

}