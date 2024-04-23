#include <post.h>

namespace post_p{

double postprocessing::get_periodic_image(double x, const int axis)

{return (x + periodic(axis) * (L(axis) * (x < -halfL(axis)) - L(axis) * (x > halfL(axis))));}

}
