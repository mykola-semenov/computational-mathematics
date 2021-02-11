#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

// #include "mpi_bindings.h"
#include "num_schemes.h"

/* Sequential version */

double lambda1(const double& u, const double& v) {
	return (u + sqrt(u * u + 3 * v)) / 3;
}

double lambda2(const double& u, const double& v) {
	return (u - sqrt(u * u + 3 * v)) / 3;
}

struct Grid {
	std::vector<double> u;
	std::vector<double> v;
	std::vector<double> Rp;
	std::vector<double> Rm;
	const double x0, x1;
	const size_t N;
	const double dx;

	Grid(const double& x0, const double& x1, const size_t& N)
		: x0(x0), x1(x1), N(N), dx((x1 - x0) / (static_cast<double>(N) - 1)) {
		u.resize(N);
		v.resize(N);
		Rp.resize(N);
		Rm.resize(N);
	}

	template <class func>
	Grid(const double& x0, const double& x1, const size_t& N, func u0): Grid(x0, x1, N) {
		for (size_t i = 0; i < N; ++i) {
			u[i] = u0(x0 + static_cast<double>(i) * dx);
			v[i] = u[i] * u[i];
			Rp[i] = 0.5 * u[i] - lambda1(u[i], v[i]) * v[i];
			Rm[i] = 0.5 * u[i] - lambda2(u[i], v[i]) * v[i];
		}
	}

	void dump(std::ostream& os) const {
		for (const auto& i : u) {
			os << std::scientific << i << ' ';
		}
		os << std::endl;
	}
};

void swap(Grid& x, Grid& y) {
	std::swap(x.u, y.u);
	std::swap(x.v, y.v);
	std::swap(x.Rp, y.Rp);
	std::swap(x.Rm, y.Rm);
}

void copy_boundaries(const Grid& prev, Grid& next) {
	const auto& N = prev.N;
	/* TODO: Beautify */
	next.u[0] = prev.u[0];
	next.v[0] = prev.v[0];
	next.Rp[0] = prev.Rp[0];
	next.Rm[0] = prev.Rm[0];

	next.u[N - 1] = prev.u[N - 1];
	next.v[N - 1] = prev.v[N - 1];
	next.Rp[N - 1] = prev.Rp[N - 1];
	next.Rm[N - 1] = prev.Rm[N - 1];
}

/*TODO: Experiment with Newtons method*/
void update_uv(const std::vector<double>& u,
			   const std::vector<double>& v,
			   const std::vector<double>& Rp_next,
			   const std::vector<double>& Rm_next,
			   std::vector<double>& u_next,
			   std::vector<double>& v_next) {
	for (int i = 0; i < u.size(); ++i) {
		u_next[i] = -((u[i] - sqrt(u[i] * u[i] + 3 * v[i])) * Rp_next[i] - (u[i] + sqrt(u[i] * u[i] + 3 * v[i])) * Rm_next[i]) / sqrt(u[i] * u[i] + 3 * v[i]);
		v_next[i] = -1.5 *(Rp_next[i] - Rm_next[i]) / sqrt(u[i] * u[i] + 3 * v[i]);
	}
}

double phi(double x) {
	return 0.1 + (abs(x + 2) > 2 ? 0 : 2-abs(x + 2));
}

int main(int argc, char* argv[]) {
	/* Initialization */
	size_t N = 101;
	double T = 1.0;
	double x0 = -5;
	double x1 = 5;
	double sigma = 0.9;

	Grid curr(x0, x1, N, [](double x) {return 1 + exp(-x*x); });
//	Grid curr(x0, x1, N, phi);

	for (const auto& i : curr.u) {
		std::cout << std::scientific << std::setprecision(2) << i << ' ';
	}

	Grid next(x0, x1, N);
	copy_boundaries(curr, next);

	std::ofstream res("res.dat");
	if (!res.is_open()) {
		std::cerr << "IO Error: res.dat" << std::endl;
		return EXIT_FAILURE;
	}

	curr.dump(res);
	double t = 0;
	while (t < T) {
		std::cout << t << std::endl;
		double dt = sigma * curr.dx / *std::max_element(begin(curr.u), end(curr.u));
		std::cout << dt << std::endl;
		CourantIz(curr.u, curr.v, curr.Rp, curr.Rm, next.Rp, next.Rm, curr.dx, dt);
		update_uv(curr.u, curr.v, next.Rp, next.Rm, next.u, next.v);


		swap(curr, next);
		curr.dump(res);
		t += dt;
	}

	res.close();
	return EXIT_SUCCESS;
}