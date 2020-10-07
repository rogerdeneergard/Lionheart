#include"math_basics.h"
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<iostream>

const double PI = 3.14159;
const double EULER = 2.71828;
const int percent = 10000;

void RandomSeed() {
	std::srand((unsigned int) std::time(nullptr));
}

double RandomPercent() {
	double result = std::rand() / (RAND_MAX + 1.0);
	return result;
}

double RandomPercentPosNeg() {
	double result = (std::rand() - (RAND_MAX / 2.0)) / ((RAND_MAX / 2.0) + 1);
	return result;
}

double NormalisedRandom(const double mean, const double stdev) {
	double probability = RandomPercentPosNeg();
	double standardScore = InverseErfNewtonian(AbsDouble(probability), 2);
	double result = mean + (SignDouble(probability) * (stdev * standardScore));
	return result;
}

int SignInt(const int x) {
	return (x >= 0) - (x < 0);
}
double SignDouble(const double x) {
	return (x >= 0) - (x < 0);
}

double AbsDouble(const double x) {
	return SignDouble(x) * x;
}

double InverseErfNewtonian(const double x, const int refinements = 1) {
	double result;
	double a[] = { 0.886226899, -1.645349621, 0.914624893, -0.140543331 };
	double b[] = { 1, -2.118377725, 1.442710462, -0.329097515, 0.012229801 };
	double c[] = { -1.970840454, -1.62490649, 3.429567803, 1.641345311 };
	double d[] = { 1, 3.543889200, 1.637067800 };

	double z = AbsDouble(x);

	if (z <= 0.7) {
		double x2 = z * z;
		result = z * (((a[3] * x2 + a[2]) * x2 + a[1]) * x2 + a[0]);
		result /= (((b[4] * x2 + b[3]) * x2 + b[2]) * x2 + b[1])* x2 + b[0];
	}
	else {
		double y = sqrt(-log((1 - z) / 2));
		result = (((c[3] * y + c[2]) * y + c[1]) * y + c[0]);
		result /= ((d[2] * y + d[1]) * y + d[0]);
	}

	result = result * SignDouble(x);
	z = z * SignDouble(x);

	for (int i = 0; i < refinements; ++i) {
		result -= (erf(result) - z) / (2 / sqrt(PI) *exp(-result * result));
	}

	return result;
}