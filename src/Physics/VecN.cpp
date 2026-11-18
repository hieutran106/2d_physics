#include "VecN.h"
#include <cstring>

VecN::VecN(int N)
	: N(N)
{
	// new float[N]() value-initializes the array. For built-in types, that zero-initializes every element to 0.0f.
	data = new float[N]();
}

VecN::VecN(const VecN& v)
	: N(v.N)
{
	if (N > 0)
	{
		data = new float[N];
		std::memcpy(data, v.data, N * sizeof(float));
	}
}
VecN::~VecN()
{
	delete[] data;
}

VecN& VecN::operator=(const VecN& v)
{
	delete[] data;
	N = v.N;
	data = new float[N];
	for (int i = 0; i < N; i++)
	{
		data[i] = v.data[i];
	}
	return *this;
}

VecN VecN::operator+(const VecN& v) const
{
	VecN result = *this;
	for (int i = 0; i < N; i++)
		result.data[i] += v.data[i];
	return result;
}
VecN VecN::operator-(const VecN& v) const
{
	VecN result = *this;
	for (int i = 0; i < N; i++)
		result.data[i] -= v.data[i];
	return result;
}
VecN VecN::operator*(float n) const
{
	VecN result = *this;
	result *= n;
	return result;
}
const VecN& VecN::operator+=(const VecN& v)
{
	for (int i = 0; i < N; i++)
		data[i] += v.data[i];
	return *this;
}
const VecN& VecN::operator-=(const VecN& v)
{
	for (int i = 0; i < N; i++)
		data[i] -= v.data[i];
	return *this;
}
const VecN& VecN::operator*=(float n)
{
	for (int i = 0; i < N; i++)
		data[i] *= n;
	return *this;
}
float VecN::operator[](int index) const
{
	return data[index];
}
float& VecN::operator[](int index)
{
	return data[index];
}

void VecN::Zero()
{
	for (int i = 0; i < N; i++)
	{
		data[i] = 0.0f;
	}
}

float VecN::Dot(const VecN& v) const
{
	float sum = 0.0f;
	for (int i = 0; i < N; i++)
	{
		sum += data[i] * v.data[i];
	}
	return sum;
}
