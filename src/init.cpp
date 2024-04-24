#include <post.h>

namespace post_p{

postprocessing::postprocessing(char *config_filename)
{
    read_config_parser(config_filename);
}

postprocessing::~postprocessing()
{

    free(lo_hi_);
    free(L_);
    free(halfL_);
    free(periodic_);
    free(pos_);
    free(num_attachments_);
    free(posDiff_);
    free(is_placed_);
    free(attachments_placed_);

}

}