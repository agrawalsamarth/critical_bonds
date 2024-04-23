#include <post.h>

namespace post_p{

void postprocessing::dump_rij_file()
{
    calc_rij();    
    printf("r_ij\n");

    for (int i = 0; i < N_pairs_; i++)
        printf("%lf\n", r_ij_[i]);
    
}

void postprocessing::dump_rij_file(char *filename)
{
    calc_rij();
    FILE *f;
    f= fopen(filename,"w");
    
    fprintf(f, "r_ij\n");

    for (int i = 0; i < N_pairs_; i++)
        fprintf(f, "%lf\n", r_ij_[i]);
    
    fclose(f);


}

void postprocessing::dump_rij_hist_file(double bin_size)
{
    calc_rij_hist(bin_size);

    printf("r,F(r)\n");

    for (int i = 0; i < r_ij_hist_bins_; i++){
        printf("%lf,%lf\n", (1.*i*bin_size + 0.5 * bin_size), r_ij_hist_[i]/(1. * N_pairs_));
    }

    free(r_ij_hist_);
}

void postprocessing::dump_rij_hist_file(double bin_size, char *filename)
{
    calc_rij_hist(bin_size);

    FILE *f;
    f= fopen(filename,"w");
    fprintf(f, "r,F(r)\n");

    for (int i = 0; i < r_ij_hist_bins_; i++){
        fprintf(f, "%lf,%lf\n", (1.*i*bin_size + 0.5 * bin_size), r_ij_hist_[i]/(1. * N_pairs_));
    }

    fclose(f);
    free(r_ij_hist_);
}

void postprocessing::calc_rij()
{
    int counter = 0;
    double r_temp;

    if (folded_ == 1) {

        for (int i = 0; i < numParticles(); i++) {
            for (int j = i+1; j < numParticles(); j++){

                for (int axis = 0; axis < dim(); axis++){
                    posDiff(axis) = get_periodic_image(1.*(pos(i,axis) - pos(j,axis)), axis);
                }

                r_temp = 0.;

                for (int axis = 0; axis < dim(); axis++)
                    r_temp += (posDiff(axis) * posDiff(axis));

                r_temp = sqrt(r_temp);

                r_ij_[counter] = r_temp;
                counter++;

            }
        }

    }

    else {

        for (int i = 0; i < numParticles(); i++) {
            for (int j = i+1; j < numParticles(); j++){

                for (int axis = 0; axis < dim(); axis++){
                    posDiff(axis) = 1.*(pos(i,axis) - pos(j,axis));
                }

                r_temp = 0.;

                for (int axis = 0; axis < dim(); axis++)
                    r_temp += (posDiff(axis) * posDiff(axis));

                r_temp = sqrt(r_temp);

                r_ij_[counter] = r_temp;
                counter++;

            }
        }

    }

}

void postprocessing::calc_rij_hist(double bin_size)
{
    calc_rij();
    double r_max = 0.;

    for (int i = 0; i < N_pairs_; i++){
        if (r_ij_[i] > r_max){
            r_max = r_ij_[i];
        }
    }

    if (folded_ == 1){

        for (int axis = 0; axis < dim(); axis++){
            if (periodic(axis) == 1){
                if (halfL(axis) < r_max){
                    r_max = halfL(axis);
                }
            }
        }

    }

    r_ij_hist_bins_ = (((int)(r_max/bin_size))+1);
    r_ij_hist_      = (double*)malloc(sizeof(double)*r_ij_hist_bins_);

    for (int i = 0; i < r_ij_hist_bins_; i++)
        r_ij_hist_[i] = 0.;

    int bin;
    r_max=r_max+(0.5*bin_size);

    for (int i = 0; i < N_pairs_; i++){

        if (r_ij_[i] < r_max){
            bin              = r_ij_[i]/bin_size;
            r_ij_hist_[bin] += 1.;
        }


    }

    //for (int i = 1; i < r_ij_hist_bins_; i++)
        //r_ij_hist_[i] += r_ij_hist_[i-1];



}

void postprocessing::dump_lr_scattering_function(double q_min, double q_max, int num_q, double bin_size, char *filename)
{

    calc_rij_hist(bin_size);

    double q;
    double sq_lr_temp;
    double dq = log(q_max/q_min)/(1 * num_q);
    double r_temp;
    double form_factor;

    sq_ = (double*)malloc(sizeof(double)*num_q);

    for (int i = 0; i < r_ij_hist_bins_; i++){
        r_temp        = 1.*i*bin_size + 0.5 * bin_size;
        r_ij_hist_[i] = (2. * r_ij_hist_[i])/(1. * N_ * 4. * M_PI * r_temp * r_temp * bin_size);
    }

    FILE *f;

    f = fopen(filename, "w");
    fprintf(f, "q,I(q)\n");

    for (int i = 0; i < num_q; i++)
    {
        q = exp((1.*i*dq)+log(q_min));

        sq_lr_temp = 0.;

        for (int j = 0; j < r_ij_hist_bins_; j++) {

            r_temp      = 1.*j*bin_size + 0.5 * bin_size;
            sq_lr_temp += (r_ij_hist_[j] - 1.) * (sin(q * r_temp)/(q * r_temp)) * (4. * M_PI * r_temp * r_temp) * bin_size; 

        }

        sq_lr_temp  = phi_ * sq_lr_temp;
        sq_lr_temp += 1.;

        form_factor = (24./q*q*q) * ((sin(0.5 * q) - (0.5 * cos(0.5 * q))));
        form_factor = form_factor * form_factor;

        fprintf(f, "%lf,%lf\n", q, sq_lr_temp * form_factor);

    }

    fclose(f);

    free(sq_);

}


}