#include "Chain.h"
#include <iostream>

using namespace physics;

void Chain::UpdateForce(PhysicsParticle* particle, float time)
{
	//Get the current position of the particle
	MyVector pos = particle->Position;

	//Create a vector Pos -> Anchor Point
	MyVector delta = this->anchorPoint - pos;

	//Get the magnitude of the vector above
	float mag = delta.Magnitude();

	//std::cout << mag << std::endl;

	//Get the direction from the particle to the anchor point
	if (mag >= restLength) {
		auto dot = (particle->Velocity).DotProd(delta.direction());
		auto velocityChange = delta.direction().ScalarMultiplication(-dot);

		particle->Velocity += velocityChange;
		particle->Position = this->anchorPoint + delta.direction().ScalarMultiplication(restLength).ScalarMultiplication(-1);
	}

}

void Chain::Update(MyVector pos)
{
	this->anchorPoint = pos;
	//std::cout << "anchorpoint position" << std::endl;
	//std::cout << anchorPoint.x << std::endl;
	//std::cout << anchorPoint.y << std::endl;
	//std::cout << anchorPoint.z << std::endl;
}

void Chain::AddrestLength(float toAdd)
{
	this->restLength += toAdd;
}


