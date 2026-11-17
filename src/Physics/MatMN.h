#pragma once
#include "VecN.h"

class MatMN
{
	int M; // rows
	int N; // cols
	// TODO: Use std::vector<VecN> so that copy ctor, operator=, destructor: can all be = default or removed
	VecN* rows = nullptr; // the rows of the matrix with N columns inside

public:
	MatMN(int M, int N);
	MatMN(const MatMN& m);
	~MatMN();

	void  Zero();
	MatMN Transpose() const;

	// m1 = m2
	const MatMN& operator=(const MatMN& m);
	// m1 * v
	VecN operator*(const VecN& v) const;
	// m1 * m2
	MatMN operator*(const MatMN& m) const;
};
