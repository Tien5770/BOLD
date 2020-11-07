#pragma once
#include"BOLDVector.h"


BVector::BVector()
{
    for (int i = 0; i < VECTOR_SIZE; ++i)
        a[i] = 0;
}

BVector::BVector(double* array)
{
    for (int i = 0; i < VECTOR_SIZE; ++i)
        a[i] = array[i];
}

BVector::BVector(double a1, double a2, double a3)
{
    a[0] = a1;
    a[1] = a2;
    a[2] = a3;
}

void BVector::set(double a1, double a2, double a3)
{
    a[0] = a1;
    a[1] = a2;
    a[2] = a3;
}

void BVector::set(BVector b)
{
    a[0] = b.getElement(0);
    a[1] = b.getElement(1);
    a[2] = b.getElement(2);
}

void BVector::setElement(int index, double value)
{
    a[index] = value;
}

double BVector::getElement(int index)
{
    if (index >= VECTOR_SIZE)
    {
        std::cout << "BOLD::Vector::getElement() error: request index out of range..\n";
        std::abort();
    }
    return a[index];
}

double BVector::dot(BVector b)
{
    double answer = 0;
    for (int i = 0; i < VECTOR_SIZE; ++i)
        answer += a[i] * b.getElement(i);
    return answer;
}

BVector BVector::cross(BVector b)
{
    if (VECTOR_SIZE != 3)
    {
        std::cout << "BOLD::Vector::cross(BVector*) error: VECTOR_SIZE is not set to 3\n";
        std::abort();
    }
    BVector ans(a[1] * b.getElement(2) - a[2] * b.getElement(1), a[2] * b.getElement(0) - a[0] * b.getElement(2), a[0] * b.getElement(1) - a[1] * b.getElement(0));
    return ans;
}

double BVector::abs()
{
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

double BVector::abs2D()
{
    return sqrt(a[0] * a[0] + a[1] * a[1]);
}


BVector BVector::minus(BVector b)
{
    BVector c(a[0] - b.getElement(0), a[1] - b.getElement(1), a[2] - b.getElement(2));
    return c;
}

BVector BVector::minus2D(BVector b)
{
    BVector c(a[0] - b.getElement(0), a[1] - b.getElement(1), 0);
    return c;
}

BVector BVector::plus(BVector b)
{
    BVector c(a[0] + b.getElement(0), a[1] + b.getElement(1), a[2] + b.getElement(2));
    return c;
}

BVector BVector::plus2D(BVector b)
{
    BVector c(a[0] + b.getElement(0), a[1] + b.getElement(1), 0);
    return c;
}

BVector BVector::divByScalar(double b)
{
    BVector c(a[0] / b, a[1] / b, a[2] / b);
    return c;
}

BVector BVector::timesScalar(double b)
{
    BVector c(a[0] * b, a[1] * b, a[2] * b);
    return c;
}

