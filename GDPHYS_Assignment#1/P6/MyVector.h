#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace P6 {
    class MyVector {
    public:
        float x, y, z;

        MyVector() : x(0), y(0), z(0) {}
        MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

        explicit operator glm::vec3() const { return glm::vec3(x, y, z); }

        //Vector Magnitude
        float Magnitude() const;

        //Vector Direction
        MyVector Direction() const;

        //Vector Addition
        void operator+= (const MyVector v);

        //Vector Subtraction
        void operator-= (const MyVector v);
        
        //Scalar Multiplication
        MyVector scalarMultiplication(float scale);

        //Component Product
        void operator*= (const MyVector v);

        //Dot Product
        float dotProduct(const MyVector v) const;

        //Cross Product
        MyVector crossProduct(const MyVector v) const;
    };
}