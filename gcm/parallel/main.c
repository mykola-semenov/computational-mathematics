#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <math.h>

#include <mpi.h>

#define SETTINGS_FILE "settings.ini"
#define INIT_FILE "init.dat"


void quit() {
	MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

void finalize() {
	if (MPI_Finalize() != MPI_SUCCESS) {
		fprintf(stderr, "ERROR: MPI_Finalize\n");
		quit();
	}
}

void init(int* rank, int* size, int* argc, char*** argv) {
	int task;
    
	if (MPI_Initialized(&task) != MPI_SUCCESS) {
		fprintf(stderr, "ERROR: MPI_Initialized\n");
		quit();
	}
    
	if (task == 0) {
		if (MPI_Init(argc, argv) != MPI_SUCCESS) {
			fprintf(stderr, "ERROR: MPI_Init\n");
			quit();
		}
	}
    
	if (MPI_Comm_rank(MPI_COMM_WORLD, rank) != MPI_SUCCESS) {
		fprintf(stderr, "ERROR: MPI_Comm_rank\n");
		finalize();
		exit(EXIT_FAILURE);
	}
    
	if (MPI_Comm_size(MPI_COMM_WORLD, size) != MPI_SUCCESS) {
		fprintf(stderr, "ERROR: MPI_Comm_size\n");
		finalize();
		exit(EXIT_FAILURE);
	}
}

void bcast(void *buffer, int count, MPI_Datatype dtype, int root) {
    if (MPI_Bcast(buffer, count, dtype, root, MPI_COMM_WORLD) != MPI_SUCCESS) {
		fprintf(stderr, "ERROR: MPI_Bcast\n");
		finalize();
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[]) {
	int task, rank, size;
	
    init(&rank, &size, &argc, &argv);

	int N_x, overlap, write_delay;
    double x_0, x_1, T;

	{ // Getting initial data
		if (rank == 0) {
			FILE* settings_file = fopen(SETTINGS_FILE, "r");
            
            fscanf(
                settings_file, 
                "%d %d %lf %lf %lf %d", 
                &N_x, &overlap, &x_0, &x_1, &T, &write_delay
            );
			
            // Здесь должна быть проверка входных данных
            
            fclose(settings_file);
		}
        
        bcast(&N_x, 1, MPI_INT, 0);
        bcast(&overlap, 1, MPI_INT, 0);
        bcast(&write_delay, 1, MPI_INT, 0);
        bcast(&x_0, 1, MPI_DOUBLE, 0);
        bcast(&x_1, 1, MPI_DOUBLE, 0);
        bcast(&T, 1, MPI_DOUBLE, 0);
	}
    
    int N = N_x / size + (rank == 0 ? N_x % size : overlap);
    printf("%d: %d %d %lf %lf %lf %d\n", rank, N, overlap, x_0, x_1, T, write_delay);

    double* u = malloc(N * sizeof(double));
    if (u == NULL) {
		fprintf(stderr, "Can't allocate memory for u\n");
		finalize();
		return EXIT_FAILURE;
	}
    
    double* unew = malloc((rank == 0 ? N_x : N) * sizeof(double));
	if (unew == NULL) {
		fprintf(stderr, "Can't allocate memory for unew\n");
		free(u);
		finalize();
		return EXIT_FAILURE;
	}

/*


#define exit_failure(error) { \
	fprintf(stderr, error); \
	free(u); \
	free(unew); \
	finalize(); \
	return EXIT_FAILURE; \
}



	
	
	double *unew = malloc(((rank == 0 ? Nfull : N) + 1) * sizeof(double));
	if (unew == NULL) {
		fprintf(stderr, "Can't allocate memory for unew\n");
		free(u);
		finalize();
		return EXIT_FAILURE;
	}

	// Initial & boundary conditions	
	memset(u, 0, (N + 1) * sizeof(double));
	
	if (rank == 0) {
		u[0] = 1.0;
		unew[0] = 1.0;
		unew[Nfull] = 0;
	}
	
	double h = 1.0 / Nfull;
	double tau = 0.5 * (h * h);
	double eps = 1e-6;
	
	double delta, maxdelta;
	
/*
	printf("rank = %d\n\
			world_size = %d\n\
			Nfull = %d\n\
			N = %d\n\
			h = %le\n\
			tau = %le\n\
			u = %lf %lf %lf ...\n\
			unew = %lf %lf %lf ...\n\n", rank, world_size, Nfull, N, h, tau, u[0], u[1], u[2], unew[0], unew[1], unew[2]);


	while (true) { // Solving
		for (size_t i = 1; i < N; ++i) {
			unew[i] = u[i] + (tau/(h*h)) * (u[i-1] - 2*u[i] + u[i+1]); // substitute tau/(h*h) with 0.5?
		}

		maxdelta = 0;

		for (size_t i = 1; i < N; ++i) {
			delta = fabs(unew[i] - u[i]);
			if (delta > maxdelta) {
				maxdelta = delta;
			}
		}
		
		if (MPI_Allreduce(MPI_IN_PLACE, &maxdelta, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD) != MPI_SUCCESS) {
			exit_failure("ERROR: MPI_Allreduce\n");
		}

		if (maxdelta < eps) {
			break;
		}

		if (rank != 0) {
			if (MPI_Send(&unew[1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
				exit_failure("ERROR: MPI_Send\n");
			}
			if (MPI_Recv(&u[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
				exit_failure("ERROR: MPI_Recv\n");
			}
		}
		
		if (rank != world_size - 1) {
			if (MPI_Recv(&u[N], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
				exit_failure("ERROR: MPI_Recv\n");
			}
			if (MPI_Send(&unew[N - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
				exit_failure("ERROR: MPI_Send\n");
			}
		}
		
		memcpy(u + 1, unew + 1, (N - 1) * sizeof(double));
	}
	
	if (rank == 0) { // Write to file
		size_t block_size = Nfull / world_size;
		for (size_t i = 1; i < world_size; ++i) {
			if (MPI_Recv(unew + N + (i - 1) * block_size, block_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
				exit_failure("ERROR: MPI_Recv\n");
			}
		}
		
		FILE *ff = fopen("resfile", "w+");
		if (ff == NULL) { // TODO: Add perror
			exit_failure("Can't open file\n");
		}
		for (size_t i = 0; i < Nfull + 1; ++i) {
			fprintf(ff, "%lf ", unew[i]);
		}
		fclose(ff);
	} else {
		if (MPI_Send(unew + 1, N - 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
			exit_failure("ERROR: MPI_Send\n");
		}
	}
    
    */
	
    
	finalize();
	return EXIT_SUCCESS;
}
