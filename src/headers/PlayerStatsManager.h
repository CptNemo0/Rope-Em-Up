#ifndef PLAYER_STATS_MANAGER
#define PLAYER_STATS_MANAGER

#include "GameObject.h"
#include "typedef.h"
#include "physics/Rope.h"
#include "components/ParticleEmitter.h"
class PlayerStatsManager
{
public:
    static PlayerStatsManager* i_;
private:
    PlayerStatsManager(Rope* rope, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2);
    ~PlayerStatsManager() = default;

public:

    static void Initialize(Rope* rope, std::shared_ptr<GameObject> player_1, std::shared_ptr<GameObject> player_2)
    {
        if (i_ == nullptr)
        {
            i_ = new PlayerStatsManager(rope, player_1, player_2);
        }
    }

    static void Destroy()
    {
        if (i_ != nullptr)
        {
            delete i_;
            i_ = nullptr;
        }
    }

    float kLevelUpThreshold = 1000.0f;
    const float kLevelUpThresholdRaise = 500.0f;
    const float kSpeedLevelUp = 75.0f;
    const float kPullPowerLevelUp = 50.0f;
    const float kDragLevelUp = 0.001f;
    const float kWeightLevelUp = -0.001f;
    const float kHealthLevelUp = 10.0f;
    const int kSegmentLevelUp = 3;

    Rope* rope_;
    std::shared_ptr<GameObject> player_1_;
    std::shared_ptr<GameObject> player_2_;

    float speed_;
    float pull_power_;
    float rope_drag_;
    float rope_weight_;
    float max_health_;
    int segments_num_;
    int player_level_ = 0;
    int rope_level_ = 0;
    
    float p_speed_;
    float p_pull_power_;
    float p_rope_drag_;
    float p_rope_weight_;
    float p_max_health_;
    int p_segments_num_;

    int p_player_level_ = 0;
    int p_rope_level_ = 0;

    float exp_;
    int level_;
    int unspent_levels_;

    void Apply();
    void LevelUp();
    void AddExp(float exp);

    void LevelUpPlayer();
    void LevelDownPlayer();

    void LevelUpRope();
    void LevelDownRope();

    void LevelUpSpeed();
    void LevelDownSpeed();

    void LevelUpPull();
    void LevelDownPull();

    void LevelUpDrag();
    void LevelDownDrag();

    void LevelUpWeight();
    void LevelDownWeight();

    void LevelUpSegments();
    void LevelDownSegments();

    void LevelUpHealth();
    void LevelDownHealth();
};

#endif // !PLAYER_STATS_MANAGER
