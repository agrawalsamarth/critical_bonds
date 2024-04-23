#include <post.h>

namespace post_p
{

void postprocessing::get_headers()
{
    headers_ = 6+(3*dim()); // for the header itself

    if (phi_flag_)
        headers_++;

    if (seed_mass_flag_)
        headers_++;

    if (columns_flag_)
        headers_++;

    if (iters_flag_)
        headers_++;

    if (alpha_flag_)
        headers_++;
}




}