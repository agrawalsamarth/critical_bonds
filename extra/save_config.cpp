#include <post.h>

namespace post_p
{

void postprocessing::save_unfolded_config(char *filename)
{
    FILE *f;
    int num_atts;
  
    f= fopen(filename,"w");

    get_headers();

    fprintf(f, "headers=%d\n", headers_);
    
    //if (lattice_flag_){                         
        fprintf(f, "lattice=%d\n",lattice_);
    //}

    //if (N_flag_){
        fprintf(f, "N=%d\n", numParticles());
    //}

    //if (D_flag_){
        fprintf(f, "D=%d\n", dim());
    //}

    //if (max_attachments_flag_){
        fprintf(f, "maxAttachments=%d\n", maxAttachments());
    //}

    //if (folded_flag_)
        fprintf(f, "folded=0\n");


    //if (L_flag_){
        for (int axis = 0; axis < dim(); axis++){
            fprintf(f, "x%d_lo=%lf\n",axis,lo_hi(axis,0));
            fprintf(f, "x%d_hi=%lf\n",axis,lo_hi(axis,1));
        }
    //}

    //if (periodic_flag_){
        for (int axis = 0; axis < dim(); axis++){
            fprintf(f, "x%d_periodic=%d\n",axis,periodic(axis));
        }
    //}

    if (seed_mass_flag_){
        fprintf(f, "seedMass=%lf\n", seed_mass_);
    }

    if (phi_flag_)
        fprintf(f, "phi=%lf\n", phi_);

    if (iters_flag_)
    {
        fprintf(f, "iters=%d\n", iters_);
    }

    if (alpha_flag_)
    {
        fprintf(f, "alpha=%lf\n", alpha_);
    }

    if (columns_flag_){
        fprintf(f, "columns=%d\n", columns_);
    }

    //fprintf(f, "id,x,y,z,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

    fprintf(f, "id,");

    for (int axis = 0; axis < dim(); axis++)
        fprintf(f, "x%d,", axis);
    
    fprintf(f, "assignedSeedStatus,");
    fprintf(f, "currentSeedStatus,");
    fprintf(f, "diameter,");
    if (max_attachments_ > 0)
        fprintf(f, "attachments,");
    else
        fprintf(f, "attachments\n");

    for (int att = 1; att <= maxAttachments(); att++){
        if (att == maxAttachments())
            fprintf(f, "att_%d\n", att);
        else
            fprintf(f, "att_%d,", att);
    }

    for (int i = 0; i < numParticles(); i++){

        fprintf(f, "%d,", i);

        for (int axis = 0; axis < dim(); axis++)
            fprintf(f, "%lf,", unfolded_coords(i,axis));

        fprintf(f,"%d,",original_seed(i));
        fprintf(f,"%d,",current_seed(i));
        fprintf(f,"%lf,",diameter(i));
        if (max_attachments_ > 0)
            fprintf(f,"%d,",numAttachments(i));
        else
            fprintf(f,"%d",numAttachments(i));

        num_atts = numAttachments(i);

        for (int j = 0; j < maxAttachments(); j++){

            if (num_atts == maxAttachments()){

                if (j != (num_atts-1))
                    fprintf(f, "%d,", attachment(i,j));
                else
                    fprintf(f, "%d", attachment(i,j));

            }

            else {

                if (j < num_atts)
                    fprintf(f, "%d,", attachment(i,j));
                else if ((j >= num_atts) && (j != (maxAttachments()-1)))
                    fprintf(f, "NaN,");
                else
                    fprintf(f, "NaN");

            }

        }

        fprintf(f, "\n");



    }

    fclose(f);
}

void postprocessing::save_unfolded_config()
{
    int num_atts;

    get_headers();
    printf("headers=%d\n", headers_);
    
    //if (lattice_flag_){                         
        printf("lattice=%d\n",lattice_);
    //}

    //if (N_flag_){
        printf("N=%d\n", numParticles());
    //}

    //if (D_flag_){
        printf("D=%d\n", dim());
    //}

    //if (max_attachments_flag_){
        printf("maxAttachments=%d\n", maxAttachments());
    //}

    //if (folded_flag_)
        printf("folded=0\n");


    //if (L_flag_){
        for (int axis = 0; axis < dim(); axis++){
            printf("x%d_lo=%lf\n",axis,lo_hi(axis,0));
            printf("x%d_hi=%lf\n",axis,lo_hi(axis,1));
        }
    //}

    //if (periodic_flag_){
        for (int axis = 0; axis < dim(); axis++){
            printf("x%d_periodic=%d\n",axis,periodic(axis));
        }
    //}

    //if (seed_mass_flag_){
        printf("seedMass=%lf\n", seed_mass_);
    //}


    if (phi_flag_)
        printf("phi=%lf\n", phi_);


    if (iters_flag_)
    {
        printf("iters=%d\n", iters_);
    }

    if (alpha_flag_)
    {
        printf("alpha=%lf\n", alpha_);
    }

    if (columns_flag_){
        printf("columns=%d\n", columns_);
    }

    //fprintf(f, "id,x,y,z,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

    printf("id,");

    for (int axis = 0; axis < dim(); axis++)
        printf("x%d,", axis);
    
    printf("assignedSeedStatus,");
    printf("currentSeedStatus,");
    printf("diameter,");
    if (max_attachments_ > 0)
        printf("attachments,");
    else
        printf("attachments\n");

    for (int att = 1; att <= maxAttachments(); att++){
        if (att == maxAttachments())
            printf("att_%d\n", att);
        else
            printf("att_%d,", att);
    }

    for (int i = 0; i < numParticles(); i++){

        printf("%d,", i);

        for (int axis = 0; axis < dim(); axis++)
            printf("%lf,", unfolded_coords(i,axis));

        printf("%d,",original_seed(i));
        printf("%d,",current_seed(i));
        printf("%lf,",diameter(i));
        if (max_attachments_ > 0)
            printf("%d,",numAttachments(i));
        else
            printf("%d",numAttachments(i));

        num_atts = numAttachments(i);

        for (int j = 0; j < maxAttachments(); j++){

            if (num_atts == maxAttachments()){

                if (j != (num_atts-1))
                    printf("%d,", attachment(i,j));
                else
                    printf("%d", attachment(i,j));

            }

            else {

                if (j < num_atts)
                    printf("%d,", attachment(i,j));
                else if ((j >= num_atts) && (j != (maxAttachments()-1)))
                    printf("NaN,");
                else
                    printf("NaN");

            }

        }

        printf("\n");



    }

}



}