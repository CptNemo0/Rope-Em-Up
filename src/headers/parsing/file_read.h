#ifndef FILE_READ_H
#define FILE_READ_H

#include "ini.h"
#include "../global.h"
#include "../ai/EnemyAIManager.h"
#include "../Vehicle.h"
#include "glm/glm.hpp"
#include "../generation/RoomGenerator.h"

void LoadEnemyAiManagerInitStruct(const string& path, ai::EnemyAIManagerInitStruct& target);
void LoadVehicleStruct(const string& path, Vehicle& target); \
void LoadRoomLayoutGenerationSettingsInitStruct(const string& path, generation::RoomLayoutGenerationSettings& target);

#endif // !FILE_READ_H
