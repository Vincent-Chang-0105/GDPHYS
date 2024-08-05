#pragma once
#include "MyVector.h"

namespace physics{

	class PhysicsParticle
	{
		//Torque
		public:
			//Angular Velocity of the Particle
			MyVector AngularVelocity = MyVector(0, 0, 0);
			//Similar to the damping that was used for the position velocity
			float AngularDamping = 0.9f;
			//Current quaternion rotation
			glm::mat4 Rotation = glm::mat4(1.f);
			//Add Force on the specific point in the model
			void AddForceAtPoint(MyVector force, MyVector p);
		protected:
			//Accumulated Torque of the object
			MyVector accumulatedTorque = MyVector(0, 0, 0);
			//Implementation of Moment Of Inertia
			virtual float MomentOfInertia();

		protected:
			bool isDestroyed = false;

			bool affectedByForce = true;
			MyVector accumulatedForce = MyVector(0, 0, 0);
			
		public:
			float mass = 0;
			MyVector Velocity;
			MyVector Position;
			MyVector Acceleration;

			float damping = 1.f;

			float lifespan;

			bool affectedByGravity = true;
			bool affectedByContacts = true;

		protected:
			void UpdatePosition(float time);
			void UpdateVelocity(float time);

		public:
			PhysicsParticle();
			void update(float time);

			//Forces
			void AddForce(MyVector force);

			void ResetForce();

			//life
			void AddLifeSpan();
			void UpdateLifeSpan(float time);

		public:
			void Destroy();
			bool IsDestroyed() { return isDestroyed; }

		public:
			float radius;
			float restitution = 1.f;

			bool GetAffectedByForce();
			void SetAffectedByForce(bool force) { this->affectedByForce = force; };

	};
}

