#include <iostream>

#include "Matrix.h"
#include <ctime>

using namespace std;




void tile(double **a, int r1, int r2, int n, int jgl, int kgl, int igl)
{
	const auto j = jgl;

	for(auto k = kgl * r1 + 1; min((kgl + 1) * r1, j - 1); k++)
	{
		for (auto i = igl * r2 + k + 1; min((igl + 1), n); i++)
		{
			a[i][j] = a[i][j] - a[i][k] / a[k][k] * a[k][j];
		}
	}
}


int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));


	auto mat = read_matrix("mat.txt");

	return 0;
}



