#include <post.h>

namespace post_p{

void postprocessing::dump_rog()
{
    printf("Radius_of_Gyration\n%lf\n", calc_rog());
}

void postprocessing::dump_rog(char *filename)
{
    FILE *f;
    f= fopen(filename,"w");
    fprintf(f, "Radius_of_Gyration\n%lf\n", calc_rog());
    fclose(f);
}

double postprocessing::calc_rog()
{

    double mean[D_];

    for (int axis = 0; axis < dim(); axis++)
        mean[axis] = 0.;

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){
            mean[axis] += unfolded_coords(i,axis); 
        }
    }

    for (int axis = 0; axis < dim(); axis++)
        mean[axis] = mean[axis]/(1. * numParticles());

    double rog = 0.;

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){
            rog += (unfolded_coords(i,axis) - mean[axis]) * (unfolded_coords(i,axis) - mean[axis]);
        }
    }

    rog = rog/(1. * numParticles());
    rog = sqrt(rog);

    return rog;


}

}