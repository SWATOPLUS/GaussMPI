#pragma once
#include <cstdlib>
#include <algorithm>
#include <fstream>

using namespace std;

class Matrix
{
public:
	double** a;
	int n;

	Matrix(const int n)
	{
		this->n = n;
		this->a = new double*[n];
		for (auto i = 0;i < n;i++)
		{
			a[i] = new double[n + 1];
		}
	}

	Matrix(const Matrix &mat)
	{
		this->n = mat.n;
		this->a = new double*[n];
		for (auto i = 0;i < n;i++)
		{
			a[i] = new double[n + 1];

			for (int j = 0;j< n + 1; j++)
			{
				a[i][j] = mat.a[i][j];
			}
		}
	}

	Matrix(Matrix &&mat)
	{
		this->a = mat.a;
		this->n = mat.n;
		mat.a = nullptr;
	}

	void randomize(const double min = -100.0, const double max = 100.0)
	{
		for (auto i = 0;i < n;i++)
		{
			for (int j = 0;j< n + 1; j++)
			{
				a[i][j] = random(min, max);
			}
		}
	}

	~Matrix()
	{
		if (a == nullptr)
		{
			return;
		}

		for (auto i = 0;i < n;i++)
		{
			delete[] a[i];
		}

		delete[] a;
	}
private:
	double random(const double min,const double max)
	{
		return static_cast<double>(rand()) / RAND_MAX * (max - min) + min;
	}
};

Matrix read_matrix(const string &file_name)
{
	ifstream fin(file_name);
	int n;
	fin >> n;

	Matrix mat(n);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0;j < n + 1;j++)
		{
			fin >> mat.a[i][j];
		}
	}

	return mat;
}

void write_matrix(const string &file_name, const Matrix mat)
{
	ofstream fout(file_name);
	fout << mat.n << endl;

	for (int i = 0; i < mat.n; i++)
	{
		for (int j = 0;j < mat.n + 1;j++)
		{
			fout << mat.a[i][j] << '\t';
		}

		fout << endl;
	}
}