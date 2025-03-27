#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <ostream>

class Fraction {
  int num = 0;
  int den = 1;

  public:
    Fraction(int n, int d);
    friend std::ostream &operator << (std::ostream &stream, const Fraction &frac);  

    friend Fraction operator +(const Fraction &f1, const Fraction &f2);
    friend Fraction operator -(const Fraction &f1, const Fraction &f2);
    friend Fraction operator *(const Fraction &f1, const Fraction &f2);
    friend Fraction operator /(const Fraction &f1, const Fraction &f2);
    Fraction operator +=(const Fraction &other) {
      int top = num * other.den + den * other.num;
      int bot = den * other.den;
      return Fraction(top, bot);
    };
    Fraction operator -=(const Fraction &other) {
      int top = num * other.den - den * other.num;
      int bot = den * other.den;
      return Fraction(top, bot);
    };
    Fraction operator *=(const Fraction &other) {
      int top = num * other.num;
      int bot = den * other.den;
      return Fraction(top, bot);
    };
    Fraction operator /=(const Fraction &other) {
      int top = num * other.den;
      int bot = den * other.num;
      return Fraction(top, bot);
    };

    double toNumeral() {
      return double(num / den);
    }
};

#endif