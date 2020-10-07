#ifndef MATH_BASICS_H
#define MATH_BASICS_H

void RandomSeed();
double RandomPercent();
double RandomPercentPosNeg();
double NormalisedRandom(const double, const double);

int SignInt(const int);
double SignDouble(const double);
double AbsDouble(const double);

double InverseErfNewtonian(const double, const int);

#endif /* !MATH_BASICS_H */