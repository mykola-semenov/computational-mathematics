#pragma once
#include <vector>

double lambda1(const double& u, const double& v);
double lambda2(const double& u, const double& v);

void CourantIz(const std::vector<double>& u,
			   const std::vector<double>& v,
			   const std::vector<double>& Rp,
			   const std::vector<double>& Rm,
			   std::vector<double>& Rp_next,
			   std::vector<double>& Rm_next,
			   const double& dx, const double& dt) {

	/*TODO: Can be united later*/
	for (size_t i = 1; i < u.size(); ++i) {
		Rp_next[i] = Rp[i] - dt / dx * lambda1(u[i], v[i]) * (Rp[i] - Rp[i - 1]);
	}
	for (size_t i = 0; i < u.size() - 1; ++i) {
		Rm_next[i] = Rm[i] - dt / dx * lambda2(u[i], v[i]) * (Rm[i + 1] - Rm[i]);
	}
}

void LaxWendroff(const std::vector<double>& u,
				 const std::vector<double>& v,
				 const std::vector<double>& Rp,
				 const std::vector<double>& Rm,
				 std::vector<double>& Rp_next,
				 std::vector<double>& Rm_next,
				 const double& dx, const double& dt) {

	for (size_t i = 1; i < u.size() - 1; ++i) {
		Rp_next[i] = Rp[i] + 0.5 * dt / dx * lambda1(u[i], v[i]) *
			(dt / dx * lambda1(u[i], v[i]) * (Rp[i + 1] - 2 * Rp[i] - Rp[i - 1])
			 - Rp[i] + Rp[i - 1]);

		Rm_next[i] = Rm[i] + 0.5 * dt / dx * lambda2(u[i], v[i]) *
			(dt / dx * lambda2(u[i], v[i]) * (Rm[i + 1] - 2 * Rm[i] - Rm[i - 1])
			 - Rm[i + 1] + Rm[i]);
	}
}