#ifndef STEERING_BEHAVIORS_H
#define STEERING_BEHAVIORS_H

#include "headers/collisions/CollisionManager.h"
#include "headers/components/Transform.h"
#include "headers/physics/PBD.h"
#include "headers/Vehicle.h"
#include "headers/Random.h"

#include <iostream>

#include "glm/glm.hpp"

glm::vec3 Seek(glm::vec3 target, glm::vec3 actor);

glm::vec3 Flee(glm::vec3 target, glm::vec3 actor);

glm::vec3 Wander(s_ptr<components::Transform> actor, Vehicle& vehicle, float t);

glm::vec3 WallAvoidance(s_ptr<components::Transform> actor, Vehicle& vehicle, float t);

glm::vec3 Pursuit(glm::vec3 target, glm::vec3 target_forward, s_ptr<components::Transform> pursuer, Vehicle& vehicle, float t);

glm::vec3 ExtrapolatedPursuit(glm::vec3 target, glm::vec3 target_forward, s_ptr<components::Transform> pursuer, Vehicle& vehicle, float t);

glm::vec3 Evade(glm::vec3 target, glm::vec3 target_forward, s_ptr<components::Transform> pursuer, Vehicle& vehicle, float t);

glm::vec3 ObstacleAvoidance(s_ptr<components::PBDParticle> actor);

glm::vec3 OffsetPursuit(s_ptr<components::PBDParticle> leader, s_ptr<components::PBDParticle> actor, float offset);

glm::vec3 Separation(s_ptr<components::PBDParticle> actor);

#endif // !STEERING_BEHAVIORS_H
