#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>

const double ZERO = 1e-10;
const unsigned N = 10000;
const double alpha = 0.5;

typedef struct {
	const double min;
	const double max;
	const double step;
	const size_t count;
} range_t;

range_t range(const double min, const double max, const size_t count) {
	return (range_t) {min, max, (max - min) / (double) (count - 1), count};
}

double** new_grid_function(const range_t x, const range_t y) {
	double** F = (double**) malloc(x.count * sizeof(double*));
	for (size_t i = 0; i < x.count; ++i) {
		F[i] = (double*) malloc(y.count * sizeof(double)); 
		for (size_t j = 0; j < y.count; ++j) {
			F[i][j] = 0;
		}
	}
	return F;
}

void fill(double** F, const range_t x, const range_t y, double f(const double, const double)) {
	for (size_t i = 0; i < x.count; ++i) {
		for (size_t j = 0; j < y.count; ++j) {
			F[i][j] = f(x.min + i * x.step, y.min + j * y.step);
		}
	}
}

void delete_grid_function(double** F, const range_t x, const range_t y) {
	for (size_t i = 0; i < x.count; ++i) {
		free(F[i]); 
	}
	free(F);
}

void print_mesh(double** F, double** U, double** V, const range_t x, const range_t y, const char* filename) {
	FILE* mesh = fopen(filename, "w");
	if (mesh == NULL) {
		perror("fopen");
	} else {
		for (size_t i = 0; i < x.count; ++i) {
			for (size_t j = 0; j < y.count; ++j) {
				fprintf(mesh ,"%le %le %le %le %le\n", x.min + i * x.step, y.min + j * y.step, F[i][j], U[i][j], V[i][j]);
			}
		}
		fclose(mesh);
	}
}

#if defined DIRICHLET
// I. Внутренняя задача Дирихле для уравнения Лапласа.
#define DIR "Dirichlet/"

const double a00 = 1;
const double b00 = 0; 

double g00(const double x) {
	return sin(5*x);
}

const double a01 = 1;
const double b01 = 0; 

double g01(const double x) {
	return cos(5*x);
}

const double a10 = 1;
const double b10 = 0;

double g10(const double y) {
	return cosh(y);
}

const double a11 = 1;
const double b11 = 0; 

double g11(const double y) {
	return sinh(y);
}

double rho(const double x, const double y) {
	return 0;
}

#elif defined POISSON
// II. Внутренняя задача Дирихле для уравнения Пуассона.
#define DIR "Poisson/"

const double a00 = 1;
const double b00 = 0; 

double g00(const double x) {
	return 0;
}

const double a01 = 1;
const double b01 = 0; 

double g01(const double x) {
	return 0;
}

const double a10 = 1;
const double b10 = 0;

double g10(const double y) {
	return 0;
}

const double a11 = 1;
const double b11 = 0; 

double g11(const double y) {
	return 0;
}

signed char sign(double x) {
	if (x < 0) {
		return -1;
	} else {
		return 1;
	}
}

double rho(const double x, const double y) {
	return 100 * sign(x) * sign(y);
}

#elif defined NEUMANN
// III. Внутренняя задача Неймана для уравнения Лапласа (Задача кондиционирования).
#define DIR "Neumann/"

const double a00 = 0;
const double b00 = 1; 

double g00(const double x) {
	return 0;
}

const double a01 = 0;
const double b01 = 1; 

double g01(const double x) {
	return 0;
}

const double a10 = 0;
const double b10 = 1;

double g10(const double y) {
	if (y > 0 && y < 0.1) {
		return 1;
	} else {
		return 0;
	}
}

const double a11 = 0;
const double b11 = 1; 

double g11(const double y) {
	if (y > 0.9 && y < 1) {
		return 1;
	} else {
		return 0;
	}
}

double rho(const double x, const double y) {
	return 0;
}

#endif

double f_init(const double x, const double y) {
	return 0.2 * (a00 * g00(x) + a01 * g01(x) + a10 * g10(y) + a11 * g11(y) + rho(x, y));
}

unsigned streamline_number = 0;
void print_stream_line(double ** F, double ** U, double ** V, const range_t x, const range_t y, const double x0, const double y0, const double dt, const size_t points) {
	streamline_number++;
	char filename[32];
	sprintf(filename, DIR"sl_%d.dat", streamline_number);
	FILE* line = fopen(filename, "w");
	if (line == NULL) {
		perror("fopen");
	} else {
		double x_curr = x0;
		double y_curr = y0;
		size_t i, j;
		for (unsigned n = 0; n < points; ++n) {
			fprintf(line, "%le %le\n", x_curr, y_curr);
			for (i = 0; i < x.count && x_curr > x.min + i * x.step; ++i);
			for (j = 0; j < y.count && y_curr > y.min + j * y.step; ++j);
			if (i == x.count || j == x.count || i == 0 || j == 0) {
				break;
			}
			x_curr += 0.25 * (U[i - 1][j - 1] + U[i - 1][j] + U[i][j - 1] + U[i][j]) * dt;
			y_curr += 0.25 * (V[i - 1][j - 1] + V[i - 1][j] + V[i][j - 1] + V[i][j]) * dt;
		}
		fclose(line);
	}
	
}

int main(int argc, char* argv[]) {
	#ifdef NEUMANN
	const range_t x = range(0, 1, 100);
	const range_t y = range(0, 1, 100);
	#else	
	const range_t x = range(-1, 1, 100);
	const range_t y = range(-1, 1, 100);
	#endif
	const double c00 = - b00 / y.step;
	const double c01 = b01 / y.step;
	const double c10 = - b10 / x.step;
	const double c11 = b11 / x.step;
	
	if ((fabs(a00 + c00) <= ZERO) || (fabs(a01 + c01) <= ZERO) || 
		(fabs(a10 + c10) <= ZERO) || (fabs(a11 + c11) <= ZERO)) {
		fprintf(stderr, "Respecify border coefficients!\n");
		return EXIT_FAILURE;
	}
	
	const double d00 = 1 / (a00 + c00);
	const double d01 = 1 / (a01 + c01);
	const double d10 = 1 / (a10 + c10);
	const double d11 = 1 / (a11 + c11);
	
	double** F = new_grid_function(x, y);
	fill(F, x, y, f_init);

	double** R = new_grid_function(x, y);
	fill(R, x, y, rho);

	const double C0 = alpha * y.step * y.step / (2 * (x.step * x.step + y.step * y.step));
	const double C1 = alpha * x.step * x.step / (2 * (x.step * x.step + y.step * y.step));
	const double C2 = alpha * x.step * x.step * y.step * y.step / (2 * (x.step * x.step + y.step * y.step));
	const double C3 = -alpha;
	
	FILE* errors = fopen(DIR"errors.dat", "w");
	if (errors == NULL) {
		perror("fopen");
	} else {
		double error;
		double temp;
		for (unsigned n = 0; n < N; ++n) {
			error = 0;
			for (size_t i = 0; i < x.count; ++i) {
				F[i][0] = d00 * (g00(x.min + i * x.step) + c00 * F[i][1]);
				F[i][y.count - 1] = d01 * (g01(x.min + i * x.step) + c01 * F[i][y.count - 2]);
			}
			for (size_t j = 0; j < y.count; ++j) {
				F[0][j] = d10 * (g10(y.min + j * y.step) + c10 * F[1][j]);
				F[x.count - 1][j] = d11 * (g11(y.min + j * y.step) + c11 * F[x.count - 2][j]);
			}
			for (size_t i = 1; i < x.count - 1; ++i) {
				for (size_t j = 1; j < y.count - 1; ++j) {
					temp = C0 * (F[i - 1][j] + F[i + 1][j]) + C1 * (F[i][j - 1] + F[i][j + 1]) + C2 * R[i][j] + C3 * F[i][j]; 
					error += temp * temp;
					F[i][j] += temp;
				}
			}
			fprintf(errors, "%d %le\n", n, sqrt(error/(x.count * y.count)));
		}
		fclose(errors);
	}
	
	double** U = new_grid_function(x, y);
	double** V = new_grid_function(x, y);

	for (size_t i = 0; i < x.count; ++i) {
		V[i][0] = (F[i][1] - F[i][0]) / y.step;
		V[i][y.count - 1] = (F[i][x.count - 1] - F[i][y.count - 2]) / y.step;
	}	
	
	for (size_t j = 0; j < y.count; ++j) {
		U[0][j] = (F[1][j] - F[0][j]) / x.step;
		U[x.count - 1][j] = (F[x.count - 1][j] - F[x.count - 2][j]) / x.step;
	}
	
	for (size_t i = 1; i < x.count - 1; ++i) {
		for (size_t j = 1; j < y.count - 1; ++j) {
			U[i][j] = (F[i + 1][j] - F[i - 1][j]) / (2 * x.step);
			V[i][j] = (F[i][j + 1] - F[i][j - 1]) / (2 * y.step);
		}
	}

	print_mesh(F, U, V, x, y, DIR"mesh.dat");
	
	FILE* circulation = fopen(DIR"circulation.dat", "w");
	if (circulation == NULL) {
		perror("fopen");
	} else {
		for (size_t i = 0; i < x.count - 1; ++i) {
			for (size_t j = 0; j < y.count - 1; ++j) {
				fprintf(circulation, "%le %le %le\n", x.min + (0.5 + (double) i) * x.step, y.min + (0.5 + (double) j) * y.step, U[i][j] + V[i][j] - U[i][j + 1] - V[i + 1][j]);
			}
		}
		fclose(circulation);
	}
	
	#if (defined DIRICHLET) || (defined NEUMANN)
	for (size_t i = 0; i < 10; ++i) {
		print_stream_line(F, U, V, x, y, x.min + (x.max - x.min) / ((double) i + 1), y.min + 2 * y.step, 0.05, 1000);
		print_stream_line(F, U, V, x, y, x.min + 2 * x.step, y.min + (y.max - y.min) / ((double) i + 1), 0.05, 1000);
		print_stream_line(F, U, V, x, y, x.min + (x.max - x.min) / ((double) i + 1), y.max - 2 * y.step, 0.05, 1000);
		print_stream_line(F, U, V, x, y, x.max - 2 * x.step, y.min + (y.max - y.min) / ((double) i + 1), 0.05, 1000);
	}
	#endif
	
	//print_stream_line(F, U, V, x, y, 0, 0, 0.05, 1000);
	
	delete_grid_function(F, x, y);
	delete_grid_function(R, x, y);
	delete_grid_function(U, x, y);
	delete_grid_function(V, x, y);
	return EXIT_SUCCESS;
}