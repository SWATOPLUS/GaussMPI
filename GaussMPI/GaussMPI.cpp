#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {
	int rank;
	double *a = nullptr;
	int n;
	int q1, q2, r1, r2;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &q1);

	if (rank == 0) {
		std::ifstream fin("input.txt");
		fin >> n;
		a = new double[n * (n + 1)];
		for (int i = 0; i < n * (n + 1); i++) {
			fin >> a[i];
		}
		r2 = std::atol(argv[1]);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	r1 = std::ceil((n - 1) / double(q1));
	MPI_Bcast(&r2, 1, MPI_INT, 0, MPI_COMM_WORLD);
	q2 = std::ceil((n - 1) / double(r2));
	if (rank != 0) {
		a = new double[n * (n + 1)];
	}
	MPI_Bcast(a, n * (n + 1), MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// Tile
	for (int i_gl = 0; i_gl <= q2 - 1; i_gl++) {
		if (rank > 0) {
			int num_to_recv = i_gl != q2 - 1 ? r2 : (n - 1) - r2 * i_gl;
			MPI_Recv(&a[(1 + i_gl * r2) * (n + 1)], num_to_recv * (n + 1), MPI_DOUBLE, rank - 1, (rank - 1) * q2 + i_gl, MPI_COMM_WORLD, &status);
		}
		for (int k = rank * r1; k < std::min((rank + 1) * r1, n - 1); k++) {
			for (int i = std::max(1 + i_gl * r2, k + 1); i < std::min(1 + (i_gl + 1) * r2, n); i++) {
				for (int j = k + 1; j < n + 1; j++) {
					a[i * (n + 1) + j] -= a[i * (n + 1) + k] / a[k * (n + 1) + k] * a[k * (n + 1) + j];
				}
			}
		}
		if (rank < q1 - 1) {
			int num_to_send = i_gl != q2 - 1 ? r2 : (n - 1) - r2 * i_gl;
			MPI_Send(&a[(1 + i_gl * r2) * (n + 1)], num_to_send * (n + 1), MPI_DOUBLE, rank + 1, rank * q2 + i_gl, MPI_COMM_WORLD);
		}
	}

	if (rank == q1 - 1) {
		std::ofstream fout("answer.txt");
		std::vector<double> x(n);
		x[n - 1] = a[(n - 1) * (n + 1) + n] / a[(n - 1) * (n + 1) + n - 1];
		for (int i = n - 2; i >= 0; i--) {
			x[i] = a[i * (n + 1) + n];
			for (int j = n - 1; j >= i + 1; j--) {
				x[i] = x[i] - a[i * (n + 1) + j] * x[j];
			}
			x[i] = x[i] / a[i * (n + 1) + i];
		}

		for (int i = 0; i < n; i++) {
			fout << x[i] << ' ';
		}
		fout << std::endl;
	}

	delete[]a;
	MPI_Finalize();
	return 0;
}