#include "PhysicsParticle.h"
#include <random>
#include <iostream>

using namespace physics;

physics::PhysicsParticle::PhysicsParticle()
{
	this->lifespan = 0;
	//this->AddLifeSpan();
	
}

void PhysicsParticle::AddForceAtPoint(MyVector force, MyVector p)
{
	this->AddForce(force);

	this->accumulatedTorque = p.crossProd(force);

}

float PhysicsParticle::MomentOfInertia()
{
	//Formula for MoI for spheres
	return ((float)2 /5) * mass * radius * radius;
}

void PhysicsParticle::UpdatePosition(float time)
{
	float newTime = time * time;
	this->Position = this->Position + (this->Velocity.ScalarMultiplication(time)) + ((this->Acceleration.ScalarMultiplication(newTime)).ScalarMultiplication(0.5f));

	MyVector angularV = AngularVelocity.ScalarMultiplication(time);

	float angleMag = angularV.Magnitude();

	MyVector MagDir = angularV.direction();

	if (angleMag != 0)
	{
		glm::quat rotBy = glm::rotate(
			glm::mat4(1.f),
			angleMag,
			(glm::vec3)MagDir
		);

		this->Rotation = glm::toMat4(glm::toQuat(this->Rotation) * rotBy);
	}


}

void PhysicsParticle::UpdateVelocity(float time)
{
	this->Acceleration += accumulatedForce.ScalarMultiplication(1 / mass);
	this->Velocity = this->Velocity + (this->Acceleration.ScalarMultiplication(time));

	this->Velocity = this->Velocity.ScalarMultiplication(powf(damping, time));

	float mI = MomentOfInertia();

	AngularVelocity += accumulatedTorque.ScalarMultiplication(time).ScalarMultiplication(((float)1 / mI));

	AngularVelocity = AngularVelocity.ScalarMultiplication(powf(AngularDamping, time));

}

void PhysicsParticle::AddForce(MyVector force)
{
	this->accumulatedForce += force;
}

void physics::PhysicsParticle::ResetForce()
{
	this->accumulatedForce = MyVector(0, 0, 0);
	this->Acceleration = MyVector(0, 0, 0);
	this->accumulatedTorque = MyVector(0, 0, 0);
}

void PhysicsParticle::update(float time)
{
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
	//this->UpdateLifeSpan(time);

	this->ResetForce();
}

void PhysicsParticle::AddLifeSpan()
{
	std::random_device rd;

	// Use Mersenne Twister engine
	std::mt19937 gen(rd());

	// Define the range [1, 10]
	std::uniform_int_distribution<> dis(1.f, 10.f);

	// Generate and print a random number in the range [1, 10]
	//std::cout << dis(gen) << ' ';
	this->lifespan = dis(gen);

}

void physics::PhysicsParticle::UpdateLifeSpan(float time)
{
	this->lifespan -= 1 * time;
	//std::cout << lifespan << std::endl;
	//std::cout << time << std::endl;

	if (this->lifespan <= 0) {
		this->Destroy();
	}
}

void PhysicsParticle::Destroy()
{
	this->isDestroyed = true;
}

bool PhysicsParticle::GetAffectedByForce()
{
	return this->affectedByForce;
}
