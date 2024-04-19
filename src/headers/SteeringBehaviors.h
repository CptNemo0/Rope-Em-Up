#ifndef STEERING_BEHAVIORS_H
#define STEERING_BEHAVIORS_H

#include "headers/collisions/CollisionManager.h"
#include "headers/components/Transform.h"
#include "headers/physics/PBD.h"
#include "headers/Vehicle.h"

#include<iostream>

#include "glm/glm.hpp"

glm::vec3 Seek(glm::vec3 target, glm::vec3 actor);

glm::vec3 Flee(glm::vec3 target, glm::vec3 actor);

glm::vec3 Wander(std::shared_ptr<components::Transform> actor, Vehicle& vehicle, float t);

glm::vec3 WallAvoidance(std::shared_ptr<components::Transform> actor, Vehicle& vehicle, float t);

glm::vec3 ObstacleAvoidance(std::shared_ptr<components::PBDParticle> actor);

glm::vec3 Pursuit(std::shared_ptr<components::Transform>, std::shared_ptr<components::Transform>, Vehicle& vehicle, float t);

glm::vec3 OffsetPursuit(std::shared_ptr<components::PBDParticle> leader, std::shared_ptr<components::PBDParticle> actor, float offset);

glm::vec3 ExtrapolatedPursuit(std::shared_ptr<components::PBDParticle> target, std::shared_ptr<components::PBDParticle> actor);

glm::vec3 Separation(std::shared_ptr<components::PBDParticle> actor);

#endif // !STEERING_BEHAVIORS_H
