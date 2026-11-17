#pragma once


class VecN
{
	float* data = nullptr;

public:
	int N = 0;
	VecN() = default;
	VecN(int N);
	VecN(const VecN& v);
	~VecN();

	// v1 = v2
	VecN& operator=(const VecN& v);
	// v1 + v2
	VecN operator+(const VecN& v) const;
	// v1 - v2
	VecN operator-(const VecN& v) const;
	// v1 * n
	VecN operator*(float n) const;

	// v1 += v2
	const VecN& operator+=(const VecN& v);
	// v1 -= v2
	const VecN& operator-=(const VecN& v);
	// v1 *= n
	const VecN& operator*=(float n);
	// v1[i]
	float operator[](int index) const;
	// also v1[i]
	float& operator[](int index);
	// .Dot(v)
	float Dot(const VecN& v) const;
	// v1.Zero()
	void Zero();
};
