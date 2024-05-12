#include "../../headers/parsing/file_read.h"

void LoadEnemyAiManagerInitStruct(const string& path, ai::EnemyAIManagerInitStruct& target)
{
	mINI::INIFile file("../../" + path);
	mINI::INIStructure ini;

	file.read(ini);

	target.choked_tentacles = stoi(ini["EnemyAiManagerInitStruct"]["choked_tentacles"]);
	target.multi_chokes = stoi(ini["EnemyAiManagerInitStruct"]["multi_chokes"]);
	target.choke_threshold = stoi(ini["EnemyAiManagerInitStruct"]["choke_threshold"]);
	target.multi_threshold = stoi(ini["EnemyAiManagerInitStruct"]["multi_threshold"]);

	target.wall_proximity_threshold = std::stof(ini["EnemyAiManagerInitStruct"]["wall_proximity_threshold"]);
	target.attack_damage = std::stof(ini["EnemyAiManagerInitStruct"]["attack_damage"]);
	target.attack_range = std::stof(ini["EnemyAiManagerInitStruct"]["attack_range"]);
	target.sense_range = std::stof(ini["EnemyAiManagerInitStruct"]["sense_range"]);

	ini.clear();
	file.~INIFile();
}

void LoadVehicleStruct(const string& path, Vehicle& target)
{
	mINI::INIFile file("../../" + path);
	mINI::INIStructure ini;

	file.read(ini);

	target.max_speed = std::stof(ini["VehicleStruct"]["max_speed"]);

	target.rest_lenght = std::stof(ini["VehicleStruct"]["rest_lenght"]);

	target.wander_target = glm::vec3(0.0f);

	target.wander_radius = std::stof(ini["VehicleStruct"]["wander_radius"]);
	target.wander_distance = std::stof(ini["VehicleStruct"]["wander_distance"]);
	target.wander_weight = std::stof(ini["VehicleStruct"]["wander_weight"]);
	target.wander_jitter = std::stof(ini["VehicleStruct"]["wander_jitter"]);
	target.wander_speed_ = std::stof(ini["VehicleStruct"]["wander_speed_"]);

	target.wall_avoidance_distance = std::stof(ini["VehicleStruct"]["wall_avoidance_distance"]);
	target.wall_avoidance_weight = std::stof(ini["VehicleStruct"]["wall_avoidance_weight"]);

	target.pursuit_distance = std::stof(ini["VehicleStruct"]["pursuit_distance"]);
	target.pursuit_weight = std::stof(ini["VehicleStruct"]["pursuit_weight"]);
	target.pursuit_speed_ = std::stof(ini["VehicleStruct"]["pursuit_speed_"]);

	target.extrapolation_distance = std::stof(ini["VehicleStruct"]["extrapolation_distance"]);
	target.extrapolation_weight = std::stof(ini["VehicleStruct"]["extrapolation_weight"]);
	target.extrapolation_speed_ = std::stof(ini["VehicleStruct"]["extrapolation_speed_"]);

	target.evade_distance = std::stof(ini["VehicleStruct"]["evade_distance"]);
	target.evade_weight = std::stof(ini["VehicleStruct"]["evade_weight"]);
	target.evade_speed_ = std::stof(ini["VehicleStruct"]["evade_speed_"]);

	ini.clear();
	file.~INIFile();
}

void LoadRoomLayoutGenerationSettingsInitStruct(const string& path, generation::RoomLayoutGenerationSettings& target)
{
	mINI::INIFile file("../../" + path);
	mINI::INIStructure ini;

	file.read(ini);

	target.angle = std::stof(ini["RoomLayoutGenerationSettings"]["angle"]);
	target.span = std::stof(ini["RoomLayoutGenerationSettings"]["span"]);
	target.branch_division_count = std::stoi(ini["RoomLayoutGenerationSettings"]["branch_division_count"]);
	target.branch_division_min_length = std::stof(ini["RoomLayoutGenerationSettings"]["branch_division_min_length"]);
	target.branch_division_max_length = std::stof(ini["RoomLayoutGenerationSettings"]["branch_division_max_length"]);
	target.sub_branch_count = std::stoi(ini["RoomLayoutGenerationSettings"]["sub_branch_count"]);
	target.sub_branch_span = std::stof(ini["RoomLayoutGenerationSettings"]["sub_branch_span"]);
	target.sub_branch_min_length = std::stof(ini["RoomLayoutGenerationSettings"]["sub_branch_min_length"]);
	target.sub_branch_max_length = std::stof(ini["RoomLayoutGenerationSettings"]["sub_branch_max_length"]);

	ini.clear();
	file.~INIFile();
}
