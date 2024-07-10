#include "ContactResolver.h"
#include "ParticleContact.h"
#include <iostream>

using namespace physics;

void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
{

    while (current_iteration < max_iterations)
    {
        // Initialize minimum separating speed to a large value
        float minSeparatingSpeed = std::numeric_limits<float>::max();
        ParticleContact* minContact = nullptr;

        // Find the contact with the least separating speed
        for (unsigned i = 0; i < contacts.size(); i++) {
            float separatingSpeed = contacts[i]->GetSeparatingSpeed();
            if (separatingSpeed < minSeparatingSpeed) {
                minSeparatingSpeed = separatingSpeed;
                minContact = contacts[i];
            }
        }

        // If no valid contact is found or the separating speed is non-negative, end
        if (minContact == nullptr) {
            std::cout << "micontact is null" << std::endl;
            break;
        }
        std::cout << minSeparatingSpeed << std::endl;
        // Resolve the contact with the least separating speed
        minContact->Resolve(time);
        current_iteration++;
    }
}
