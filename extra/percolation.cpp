#include <post.h>

namespace post_p
{

bool postprocessing::check_percolation()
{
    init_unfolding();
    bool percolation_test = false;

    for (int i = 0; i < totalClusters_; i++){
        for (int axis = 0; axis < dim(); axis++) {
            percolation_test = (percolation_test || cluster_percolation(i,axis));
        }
    }

    return percolation_test;

}

void postprocessing::dump_percolation_file(char *filename)
{

    FILE *f;

    f=fopen(filename, "w");

    fprintf(f,"cluster_number,");

    for (int axis = 0; axis < dim(); axis++){
        if (axis == (dim() -1))
            fprintf(f, "percolation_x%d\n",axis);
        else
            fprintf(f, "percolation_x%d,",axis);
    }

    for (int i = 0; i < totalClusters_; i++){
        fprintf(f, "%d,",i);
        for (int axis = 0; axis < dim(); axis++){
            if (axis == (dim() -1))
                fprintf(f, "%d\n",cluster_percolation(i,axis));
            else
                fprintf(f, "%d,",cluster_percolation(i,axis));
        }

    }

    fclose(f);

}

void postprocessing::dump_percolation_file()
{

    printf("cluster_number,");

    for (int axis = 0; axis < dim(); axis++){
        if (axis == (dim() -1))
            printf("percolation_x%d\n",axis);
        else
            printf("percolation_x%d,",axis);
    }

    for (int i = 0; i < totalClusters_; i++){
        printf("%d,",i);
        for (int axis = 0; axis < dim(); axis++){
            if (axis == (dim() -1))
                printf("%d\n",cluster_percolation(i,axis));
            else
                printf("%d,",cluster_percolation(i,axis));
        }

    }


}

void postprocessing::dump_load_bearing_paths_file(char *filename)
{

    FILE *f;

    f=fopen(filename, "w");

    fprintf(f,"id,");

    for (int axis = 0; axis < dim(); axis++)
        fprintf(f, "axis_%d,", axis);

    fprintf(f, "weak_links\n");
    
    for (int i = 0; i < total_lbp; i++){
        
        fprintf(f, "%d,", i);

        for (int axis = 0; axis < dim(); axis++){
            fprintf(f,"%d,",cluster_percolation(i,axis));
        }

        for (int j = 0; j < weak_links[i].size(); j++){
            
            if (j != (weak_links[i].size()-1)){
                fprintf(f, "%d-%d,",weak_links[i][j].first,weak_links[i][j].second);
            }

            else {
                fprintf(f, "%d-%d\n",weak_links[i][j].first,weak_links[i][j].second);
            }


        }

    }

    fclose(f);

}


void postprocessing::dump_load_bearing_paths_file()
{

    printf("id,");

    for (int axis = 0; axis < dim(); axis++)
        printf("axis_%d,", axis);

    printf("weak_links\n");
    
    for (int i = 0; i < total_lbp; i++){
        
        printf("%d,", i);

        for (int axis = 0; axis < dim(); axis++){
            printf("%d,",cluster_percolation(i,axis));
        }

        for (int j = 0; j < weak_links[i].size(); j++){
            
            if (j != (weak_links[i].size()-1)){
                printf("%d-%d,",weak_links[i][j].first,weak_links[i][j].second);
            }

            else {
                printf("%d-%d\n",weak_links[i][j].first,weak_links[i][j].second);
            }


        }

    }

}


/*void postprocessing::dump_load_bearing_paths_file(char *filename)
{

    FILE *f;

    f=fopen(filename, "w");

    fprintf(f,"id,");
    
    for (int axis = 0; axis < dim(); axis++){
        if (axis != (dim()-1))
            fprintf(f, "axis_%d,",axis);
        else
            fprintf(f, "axis_%d\n",axis);
    }

    for (int i = 0; i < numParticles(); i++){
        
        fprintf(f, "%d,", i);

        for (int axis = 0; axis < dim(); axis++){
            if (axis != (dim()-1))
                fprintf(f, "%d,",load_bearing_paths(i,axis));
            else
                fprintf(f, "%d\n",load_bearing_paths(i,axis));
        }

    }

    fclose(f);

}


void postprocessing::dump_load_bearing_paths_file()
{
    printf("id,");
    
    for (int axis = 0; axis < dim(); axis++){
        if (axis != (dim()-1))
            printf("axis_%d,",axis);
        else
            printf("axis_%d\n",axis);
    }

    for (int i = 0; i < numParticles(); i++){
        
        printf("%d,", i);

        for (int axis = 0; axis < dim(); axis++){
            if (axis != (dim()-1))
                printf("%d,",load_bearing_paths(i,axis));
            else
                printf("%d\n",load_bearing_paths(i,axis));
        }

    }

}*/


}