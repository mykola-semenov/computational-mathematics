#include <cstdlib>
#include <iostream>

#include "mpi_bindings.h"

void Comm::_check(const int retval, const char* message) {
	if (retval != MPI_SUCCESS) {
		std::cerr << "ERROR: " << message << std::endl;
		MPI_Abort(comm, EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}
}

#define _CHECK(X) _check(X, #X)

Comm::Comm(int* argc, char*** argv, MPI_Comm comm): comm(comm) {

	_CHECK(MPI_Initialized(&_flag));

	if (_flag == 0) {
		_CHECK(MPI_Init(argc, argv));
	}

	_CHECK(MPI_Comm_rank(comm, &_rank));
	_CHECK(MPI_Comm_size(comm, &_size));
}

Comm::~Comm() {
	_CHECK(MPI_Finalize());
}

const int& Comm::rank() const {
	return _rank;
}

const int& Comm::size() const {
	return _size;
}