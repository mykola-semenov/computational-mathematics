#pragma once
#include <mpi.h>

class Comm {
	int _flag, _rank, _size;
	const MPI_Comm comm;

	void _check(const int retval, const char* message);
public:
	Comm(int* argc, char*** argv, MPI_Comm comm = MPI_COMM_WORLD);

	~Comm();

	const int& rank() const;
	const int& size() const;
};