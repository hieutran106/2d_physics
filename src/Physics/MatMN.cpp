#include "MatMN.h"
#include <cassert>

MatMN::MatMN(int M, int N)
	: M(M)
	, N(N)
{
	rows = new VecN[M];
	for (int i = 0; i < M; i++)
	{
		rows[i] = VecN(N);
	}
}
MatMN::MatMN(const MatMN& m)
{
	*this = m;
}
MatMN::~MatMN()
{
	delete[] rows;
}
void MatMN::Zero()
{
	for (int i = 0; i < M; i++)
	{
		rows[i].Zero();
	}
}
MatMN MatMN::Transpose() const
{
	MatMN result(N, M);
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			result.rows[j][i] = rows[i][j];
		}
	}
	return result;
}
const MatMN& MatMN::operator=(const MatMN& m)
{
	if (this == &m)
	{
		return *this;
	}
	delete[] rows;
	M = m.M;
	N = m.N;
	rows = new VecN[M];
	for (int i = 0; i < M; i++)
	{
		rows[i] = m.rows[i];
	}
	return *this;
}
VecN MatMN::operator*(const VecN& v) const
{
	assert(N == v.N);
	VecN result(M);
	for (int i = 0; i < M; i++)
	{
		result[i] = v.Dot(rows[i]);
	}
	return result;
}
MatMN MatMN::operator*(const MatMN& m) const
{
	// inner dimensions must match: (M x N) * (N x m.N)
	assert(N == m.M);
	MatMN result(M, m.N);

	MatMN transposed = m.Transpose();
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < m.N; j++)
		{
			result.rows[i][j] = rows[i].Dot(transposed.rows[j]);
		}
	}

	return result;
}
