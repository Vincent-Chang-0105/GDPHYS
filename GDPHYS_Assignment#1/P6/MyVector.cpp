#include "MyVector.h"

using namespace P6;

float MyVector::Magnitude() const
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

MyVector MyVector::Direction() const
{
    float magnitude = this->Magnitude();
    if (magnitude == 0) {
        return MyVector(0, 0, 0);
    }
    return MyVector(x/magnitude , y/magnitude, z/magnitude);
}



void MyVector::operator+=(const MyVector v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
}

void MyVector::operator-=(const MyVector v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
}

MyVector MyVector::scalarMultiplication(float scale)
{
    return MyVector(this->x * scale, this-> y * scale, this->z * scale);
}

void MyVector::operator*=(const MyVector v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

float MyVector::dotProduct(const MyVector v) const
{
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

MyVector MyVector::crossProduct(const MyVector v) const
{
    float x = this->y * v.z - this->z * v.y;
    float y = this->z * v.x - this->x * v.z;
    float z = this->x * v.y - this->y * v.x;
    return MyVector(x, y ,z);
}
