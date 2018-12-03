#include "Matrix.h"
#include <ctime>

using namespace std;

int main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	auto n = 1000;

	if(argc > 1)
	{
		n = atoi(argv[1]);
	}

	Matrix matrix(n);
	matrix.randomize();
	write_matrix("input.txt", matrix);

	return 0;
}
