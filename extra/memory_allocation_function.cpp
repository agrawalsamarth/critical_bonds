#include <post.h>

namespace post_p
{

    void postprocessing::memory_allocation_function()
    {

        pos_             = (double*)malloc(sizeof(double) * numParticles() * dim());
        num_attachments_ = (int*)malloc(sizeof(int) * numParticles());
        original_seed_   = (int*)malloc(sizeof(int) * numParticles());
        current_seed_    = (int*)malloc(sizeof(int) * numParticles());

        if (max_attachments_ > 0)
            attachment_      = (int*)malloc(sizeof(int) * numParticles() * maxAttachments());
        
        diameter_        = (double*)malloc(sizeof(double) * numParticles());
        radius_          = (double*)malloc(sizeof(double) * numParticles());

        posDiff_         = (double*)malloc(sizeof(double) * dim());

        is_placed_                = (bool*)malloc(sizeof(bool) * numParticles());
        attachments_placed_       = (bool*)malloc(sizeof(bool) * numParticles());
        unfolded_coords_          = (double*)malloc(sizeof(double) * numParticles() * dim());
        unfolded_num_attachments_ = (int*)malloc(sizeof(int) * numParticles());
        cluster_percolation_      = (int*)malloc(sizeof(int) * numParticles() * dim());
        load_bearing_paths_       = (int*)malloc(sizeof(int) * numParticles() * dim());
        temp_lbp_                 = (int*)malloc(sizeof(int) * dim());

        r_ij_ = (double*)malloc(sizeof(double) * N_pairs_);

    }


}