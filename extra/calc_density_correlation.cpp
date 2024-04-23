#include <post.h>

namespace post_p{

void postprocessing::dump_density_correlation(double bin_size){

    double r_max=0.;

    calc_rij();

    for (int i = 0; i < N_pairs_; i++){

        if (r_ij_[i] > r_max)
            r_max = r_ij_[i];

    }

    int bins  = (int)(r_max+1.);
    rho_hist_ = (int*)malloc(sizeof(int) * bins);

    for (int i = 0; i < bins; i++)
        rho_hist_[i] = 0;

    calc_density_correlation(bin_size);

    double prefactor;

    //prefactor = tgamma(0.5 * dim())/(2. * numParticles() * pow(M_PI, 0.5*dim()));
    prefactor = 1./(2. * M_PI * numParticles());

    printf("r,rho(r)\n");

    for (int i = i; i < bins; i++){
        //printf("%d,%lf\n", i, (prefactor * rho_hist_[i])/(1. * pow(1.*i, 1.*(dim()-1))));
        printf("%d,%lf\n", i, (prefactor * rho_hist_[i])/(1. * i));
    }

    free(rho_hist_);

}

void postprocessing::dump_density_correlation(double bin_size, char *filename){

    double r_max=0.;

    calc_rij();

    for (int i = 0; i < N_pairs_; i++){

        if (r_ij_[i] > r_max)
            r_max = r_ij_[i];

    }

    int bins  = (int)(r_max+1.);
    rho_hist_ = (int*)malloc(sizeof(int) * bins);

    for (int i = 0; i < bins; i++)
        rho_hist_[i] = 0;

    calc_density_correlation(bin_size);

    FILE *f;

    f = fopen(filename, "w");

    fprintf(f, "r,rho(r)\n");

    for (int i = 0; i < bins; i++)
        fprintf(f, "%d,%lf\n", i, rho_hist_[i]/(2. * numParticles()));

    fclose(f);

    free(rho_hist_); 

}

/*void postprocessing::calc_density_correlation(){

    bool xor_flag;
    double offset=1e-10;

    int counter=0;

    for (int i = 0; i < numParticles(); i++){
        for (int j = i+1; j < numParticles(); j++){

            for (int axis = 0; axis < dim(); axis++)
                posDiff_int_[axis] = (int)(fabs(get_periodic_image(pos(i,axis)-pos(j,axis), axis)) + offset);

            xor_flag=false;

            for (int axis=0; axis < dim(); axis++)
                xor_flag = (xor_flag ^ (bool)posDiff_int_[axis]);

            if (xor_flag){

                for (int axis = 0; axis < dim(); axis++){

                    if (posDiff_int_[axis] != 0)
                        rho_hist_[posDiff_int_[axis]] += 1;

                    if (counter < 10){

                        if (axis == (dim()-1))
                            std::cout<<posDiff_int_[axis]<<std::endl;

                        else
                            std::cout<<posDiff_int_[axis]<<"\t";

                    }

                }

                counter += 1;

            }


        }
    }


}*/

void postprocessing::calc_density_correlation(double bin_size){

    double half_bin_size;
    int r_int;
    double offset;

    for (int i = 0; i < N_pairs_; i++){

        r_int = (int)(r_ij_[i]);

        half_bin_size = 0.5 * bin_size * r_int;

        if (fabs(r_ij_[i] - (1. * r_int)) < half_bin_size){
            //std::cout<<"rij = "<<r_ij_[i]<<"\t r_int = "<<r_int<<"\t"<<rho_hist_[r_int]<<std::endl;
            rho_hist_[r_int] += 1;
        }

        r_int += 1;

        half_bin_size = 0.5 * bin_size * r_int;

        if (fabs(r_ij_[i] - (1. * r_int)) < half_bin_size){
            //std::cout<<"rij = "<<r_ij_[i]<<"\t r_int = "<<r_int<<"\t"<<rho_hist_[r_int]<<std::endl;
            rho_hist_[r_int] += 1;
        }

    }


}

}