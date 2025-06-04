#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <limits>
#include <memory>

// #include "GLMatrix.hpp"
#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"

inline GLVector operator*(const GLVector &lhs, double scale) {
  return GLVector(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLVector operator*(double scale, const GLVector &rhs) {
  return GLVector(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator*(const GLPoint &lhs, double scale) {
  return GLPoint(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLPoint operator*(double scale, const GLPoint &rhs) {
  return GLPoint(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator+(const GLPoint &p1, const GLVector &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}


inline GLPoint operator+(const GLPoint &p1, const GLPoint &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator+(const GLVector &p1, const GLVector &p2) {
  return GLVector(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator-(const GLVector &p1, const GLVector &p2) {
  return GLVector(p1(0) - p2(0), p1(1) - p2(1), p1(2) - p2(2));
}

inline GLVector operator-(const GLPoint &p1, const GLPoint &p2) {
  return GLVector(p1(0) - p2(0), p1(1) - p2(1), p1(2) - p2(2));
}

inline GLVector crossProduct(const GLVector &lhs, const GLVector &rhs) {
  return GLVector(lhs(1) * rhs(2) - lhs(2) * rhs(1),
                  lhs(2) * rhs(0) - lhs(0) * rhs(2),
                  lhs(0) * rhs(1) - lhs(1) * rhs(0));
}

inline double dotProduct(const GLVector &lhs, const GLVector &rhs) {
  return lhs(0) * rhs(0) + lhs(1) * rhs(1) + lhs(2) * rhs(2);
}

inline GLVector operator*(const GLMatrix &lhs, const GLVector &rhs) {
	GLVector updated = GLVector();
	for (int i=0; i<3; i++) {
		updated(i) = lhs.getColumn(0)(i) * rhs(0) + lhs.getColumn(1)(i) * rhs(1) + lhs.getColumn(2)(i) * rhs(2);
	}
	return updated; 
}

inline GLPoint operator*(const GLMatrix &lhs, const GLPoint &rhs) {
	GLPoint updated = GLPoint();
	for (int i=0; i<3; i++) {
		updated(i) = lhs.getColumn(0)(i) * rhs(0) + lhs.getColumn(1)(i) * rhs(1) + lhs.getColumn(2)(i) * rhs(2) + lhs.getColumn(3)(i);
	}
	return updated;
}

inline GLMatrix operator*(const GLMatrix &lhs, const GLMatrix &rhs) {
	GLMatrix updated = GLMatrix();
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			float sum = 0;
			for (int k=0; k<4; k++) {
				sum += lhs(i, k) * rhs(k, j);
			}
			// std::cout<<i<<";"<<sum<<"\n";
			updated.setValue(i, j, sum);
		}
	}
	return updated;
}

inline int sgn(int x) { return (x > 0) ? 1 : (x < 0) ? -1 : 0; }

/** Aufgabenblatt 2, Aufgabe 2 **/



