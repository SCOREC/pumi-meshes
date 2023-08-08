#include <stdio.h>
#include <stdlib.h>

#include "pcgnslib.h"
#include "mpi.h"

#define NODES_PER_SIDE 5

int main(int argc, char *argv[])
{
    int comm_size, comm_rank;
    int tot_nnodes, tot_nelems, nnodes, nelems;
    int F, B, Z, E, S, Fs, A, Cx, Cy, Cz;
    int i, j, k, n, nn, ne;
    float *x, *y, *z, *d;
    cgsize_t sizes[3], *e, start, end, ncells;
    static char *outfile = "cexample.cgns";

    /* initialize MPI */
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    /* total number of nodes and hex elements */
    tot_nnodes = NODES_PER_SIDE * NODES_PER_SIDE * NODES_PER_SIDE;
    tot_nelems = (NODES_PER_SIDE-1) * (NODES_PER_SIDE-1) * (NODES_PER_SIDE-1);
 
    /* open the file and create base and zone */
    sizes[0] = tot_nnodes;
    sizes[1] = tot_nelems;
    sizes[2] = 0;

    /* the default here is to use MPI_COMM_WORLD,
       but this allows assigning of another communicator
    cgp_mpi_comm(MPI_COMM_WORLD); */

    if (cgp_open(outfile, CG_MODE_WRITE, &F) ||
        cg_base_write(F, "Base", 3, 3, &B) ||
        cg_zone_write(F, B, "Zone", sizes, CG_Unstructured, &Z))
        cgp_error_exit();

    /* print info */
    if (comm_rank == 0) {
        printf("writing %d coordinates and %d hex elements to %s\n",
            tot_nnodes, tot_nelems, outfile);
    }

    /* create data nodes for coordinates */
    if (cgp_coord_write(F, B, Z, CG_RealSingle, "CoordinateX", &Cx) ||
        cgp_coord_write(F, B, Z, CG_RealSingle, "CoordinateY", &Cy) ||
        cgp_coord_write(F, B, Z, CG_RealSingle, "CoordinateZ", &Cz))
        cgp_error_exit();
 
    /* number of nodes and range this process will write */
    nnodes = (tot_nnodes + comm_size - 1) / comm_size;
    start  = nnodes * comm_rank + 1;
    end    = nnodes * (comm_rank + 1);
    if (end > tot_nnodes) end = tot_nnodes;
    
    /* create the coordinate data for this process */
    x = (float *)malloc(nnodes * sizeof(float));
    y = (float *)malloc(nnodes * sizeof(float));
    z = (float *)malloc(nnodes * sizeof(float));
    nn = 0;
    for (n = 1, k = 0; k < NODES_PER_SIDE; k++) {
        for (j = 0; j < NODES_PER_SIDE; j++) {
            for (i = 0; i < NODES_PER_SIDE; i++, n++) {
                if (n >= start && n <= end) {
                    x[nn] = (float)i;
                    y[nn] = (float)j;
                    z[nn] = (float)k;
                    nn++;
                }
            }
        }
    }

    /* write the coordinate data in parallel */
    if (cgp_coord_write_data(F, B, Z, Cx, &start, &end, x) ||
        cgp_coord_write_data(F, B, Z, Cy, &start, &end, y) ||
        cgp_coord_write_data(F, B, Z, Cz, &start, &end, z))
        cgp_error_exit();
    
    /* create data node for elements */
    if (cgp_section_write(F, B, Z, "Hex", CG_HEXA_8, 1, tot_nelems, 0, &E))
        cgp_error_exit();
 
    /* number of elements and range this process will write */
    nelems = (tot_nelems + comm_size - 1) / comm_size;
    start  = nelems * comm_rank + 1;
    end    = nelems * (comm_rank + 1);
    if (end > tot_nelems) end = tot_nelems;
    
    /* create the hex element data for this process */
    e = (cgsize_t *)malloc(8 * nelems * sizeof(cgsize_t));
    nn = 0;
    for (n = 1, k = 1; k < NODES_PER_SIDE; k++) {
        for (j = 1; j < NODES_PER_SIDE; j++) {
            for (i = 1; i < NODES_PER_SIDE; i++, n++) {
                if (n >= start && n <= end) {
                    ne = i + NODES_PER_SIDE*((j-1)+NODES_PER_SIDE*(k-1));
                    e[nn++] = ne;
                    e[nn++] = ne + 1;
                    e[nn++] = ne + 1 + NODES_PER_SIDE;
                    e[nn++] = ne + NODES_PER_SIDE;
                    ne += NODES_PER_SIDE * NODES_PER_SIDE;
                    e[nn++] = ne;
                    e[nn++] = ne + 1;
                    e[nn++] = ne + 1 + NODES_PER_SIDE;
                    e[nn++] = ne + NODES_PER_SIDE;
                }
            }
        }
    }

    /* write the element connectivity in parallel */
    if (cgp_elements_write_data(F, B, Z, E, start, end, e))
        cgp_error_exit();
if(0==1){
    /* create a centered solution */
    if (cg_sol_write(F, B, Z, "Solution", CG_CellCenter, &S) ||
        cgp_field_write(F, B, Z, S, CG_RealSingle, "CellIndex", &Fs))
        cgp_error_exit();

    /* create the field data for this process */
    d = (float *)malloc(nelems * sizeof(float));
    nn = 0;
    for (n = 1; n <= tot_nelems; n++) {
        if (n >= start && n <= end) {
            d[nn] = (float)n;
            nn++;
        }
    }

    /* write the solution field data in parallel */
    if (cgp_field_write_data(F, B, Z, S, Fs, &start, &end, d))
        cgp_error_exit();

    /* create user data under the zone and duplicate solution data */
    ncells = tot_nelems;
    if (cg_goto(F, B, "Zone_t", 1, NULL) ||
        cg_user_data_write("User Data") ||
        cg_gorel(F, "User Data", 0, NULL) ||
        cgp_array_write("CellIndex", CG_RealSingle, 1, &ncells, &A))
        cgp_error_exit();

    /* write the array data in parallel */
    if (cgp_array_write_data(A, &start, &end, d))
        cgp_error_exit();

}
    /* close the file and terminate MPI */
    cgp_close(F);    
    MPI_Finalize();
    return 0;
}
