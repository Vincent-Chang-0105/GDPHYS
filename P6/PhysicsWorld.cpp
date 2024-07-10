#include "PhysicsWorld.h"
#include "PhysicsParticle.h"
#include "ParticleContact.h"

using namespace physics;

void PhysicsWorld::AddParticle(PhysicsParticle* toAdd)
{
	Particles.push_back(toAdd);

	forceRegistry.Add(toAdd, &Gravity);
}

void PhysicsWorld::Update(float time)
{
	UpdateParticleList();

	forceRegistry.UpdateForces(time);
	
	for (std::list<PhysicsParticle*>::iterator p = Particles.begin(); p != Particles.end(); p++) {
		(*p)->update(time);
	}

	if (Contacts.size() > 0)
	{
		contactResolver.ResolveContacts(Contacts, time);
	}
}

void PhysicsWorld::UpdateParticleList()
{
	Particles.remove_if([](physics::PhysicsParticle* p) {
		return p->IsDestroyed();
		}
	);
}

void PhysicsWorld::AddContact(PhysicsParticle* p1, PhysicsParticle* p2, float restitution, MyVector contactNormal)
{
	//Create a Particle Contact
	ParticleContact* toAdd = new ParticleContact();

	//Assign the needed variables and values
	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;

	//Similar to a ist you can just call push_back
	Contacts.push_back(toAdd);
}
