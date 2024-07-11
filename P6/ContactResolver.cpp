#include "ContactResolver.h"
#include "ParticleContact.h"
#include <iostream>

using namespace physics;

void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
{
	contacts[0]->Resolve(time);
}
