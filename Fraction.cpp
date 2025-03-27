#include "Fraction.h"


Fraction::Fraction(int n=0, int d=1) : num(n), den(d) {};

std::ostream &operator<<(std::ostream &stream, const Fraction &frac) {
  stream << frac.num << "/" << frac.den;
  return stream;
}

Fraction operator +(const Fraction &f1, const Fraction &f2) {
  int num = f1.num * f2.den + f2.num * f1.den;
  int den = f1.den * f2.den;
  return Fraction(num, den);
}

Fraction operator -(const Fraction &f1, const Fraction &f2) {
  int num = f1.num * f2.den - f2.num * f1.den;
  int den = f1.den * f2.den;
  return Fraction(num, den);
}

Fraction operator *(const Fraction &f1, const Fraction &f2) {
  int num = f1.num * f2.num;
  int den = f1.den * f2.den;
  return Fraction(num, den);
}

Fraction operator /(const Fraction &f1, const Fraction &f2) {
  int num = f1.num * f2.den;
  int den = f1.den * f2.num;
  return Fraction(num, den);
}

// helper function to get lowest term possible
int gcd(int i, int j) {
  while (i % j != 0) {
    int prev_i = i;
    int prev_j = j;

    i = prev_j;
    j = prev_i % prev_j;
  }

  return j;
}