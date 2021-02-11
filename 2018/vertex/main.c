#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#define COUNT 5

typedef struct {
	double x;
	double y;
} vector_t;

int main() {
	vector_t A[COUNT];
	for (size_t n = 0; n < COUNT; ++n) {
		A[n].x = cos(M_2_PI*n/COUNT - pi);
		A[n].y = sin(M_2_PI*n/COUNT - PI);
	}
	
	for (size_t n = 0; n < COUNT; ++n) {
		printf("%lf %lf\n", A[n].x, A[n].y);
	}

	return EXIT_SUCCESS;
}