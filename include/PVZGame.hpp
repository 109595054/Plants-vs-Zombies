#ifndef PVZGAME_HPP
#define PVZGAME_HPP

#include <memory>
#include <vector>
#include <random>
#include "Core/Context.hpp"
#include "Util/GameObject.hpp"
#include "Util/Transform.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "Util/BGM.hpp"
#include "Game_Define.h"
#include "Plant.hpp"
#include "Zombie.hpp"
#include "Bullet.hpp"
#include "CardManager.hpp"
#include "Sun.hpp"
#include "Game_Define.h"
#include "UIImage.hpp"
#include "UIText.hpp"
#include "LawnMower.hpp"

class PVZGame
{
public:
    static PVZGame& GetInstance();
    void Start(int level);
    void Update();

    int GetGameState() const { return m_game_status; }

    bool CheckBulletCollisions(float x, float y, bool is_slow_down);
    bool ShouldShoot(int grid_x);
    void GenSun(float x = 99999, float y = 99999);
    bool CheckCherryBombExplod(float x, float y, float radius);
    void CherryBombExplod(float x, float y, float radius, float damage);
    bool CheckPotatoMineExplod(float x, float y, float radius);
    void PotatoMineExplod(float x, float y, float radius, float damage);
    bool CheckChomperDigest(float x, float y);
    void CheckLawnerTrigger(int grid_y, float x, float radius);

private:
    PVZGame();
    PVZGame(const PVZGame&) = delete;
    PVZGame& operator=(const PVZGame&) = delete;

    void RefreshSun();
    void RecycleSun();
    void DrawSun();
    
    void GenZombie(ZombieType type);
    void RefreshZombie();
    void RecycleZombie();

    bool CanPlacePlant(int grid_x, int grid_y);
    void PlacePlant(int grid_x, int grid_y, float mouse_x, float mouse_y, PlantType type);
    void RecyclePlant();

    void CheckGameOver();
    void HandleInput(); 
    void CheckCollisions();

    void InitWave();
    void UpdateWave();
    int GetZombiesPerWave(int level, int wave);
    ZombieType GetZombieTypeForLevel(int level);

    void UpdateBGM();
    void PlayBackgroundMusic();
    void PasueBackgroundMusic();
    void ResumeBackgroundMusic();
    void PlayWaveWarning();
    void PlayLoseMusic();

    void InitLawnMower();
    void UpdateLawnMower();

    int m_level;
    int m_game_status;

    float m_last_gen_sun_time;
    std::mt19937 m_random;
    std::vector<std::unique_ptr<Sun>> m_suns;
    int m_collect_sun_score;

    float m_last_zombie_time;
    std::vector<std::unique_ptr<Zombie>> m_zombies;

    std::vector<std::unique_ptr<Plant>> m_plants;
    PlantType m_selected_plant;

    std::unique_ptr<UIText> m_score_text;
    std::unique_ptr<UIIMAGE> m_score_image;
    std::unique_ptr<UIText> m_level_text;
    std::unique_ptr<UIIMAGE> m_game_over_image;
    std::unique_ptr<UIText> m_success_text;
    std::unique_ptr<UIIMAGE> m_back_image;

    std::unique_ptr<CardManager> m_card_manager;

    int m_current_wave;
    int m_total_waves;
    int m_zombies_in_current_wave;
    int m_zombies_spawned_in_wave;
    WAVE_STATUS m_wave_status;
    float m_wave_start_time;
    float m_wave_interval;
    bool m_final_wave;

    std::unique_ptr<Util::BGM> m_background_music;
    std::unique_ptr<Util::BGM> m_wave_warning_music;
    std::unique_ptr<Util::BGM> m_lose_music;

    bool m_is_background_music_playing;
    bool m_is_wave_warning_playing;
    float m_wave_warning_start_time;

    std::vector<std::unique_ptr<LawnMower>> m_lawn_mowers;
};

#endif
