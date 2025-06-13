#include "PVZGame.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/BGM.hpp"
#include <cmath>

#include "BucketheadZombie.hpp"
#include "CherryBomb.hpp"
#include "Chomper.hpp"
#include "ConeheadZombie.hpp"
#include "FlagZombie.hpp"
#include "PeaShooter.hpp"
#include "PotatoMine.hpp"
#include "SnowPea.hpp"
#include "SunFlower.hpp"
#include "WallNut.hpp"
#include "Util/BGM.hpp"


PVZGame& PVZGame::GetInstance()
{
    static PVZGame instance;
    return instance;
}

PVZGame::PVZGame()
: m_level(1)
, m_game_status(GAME_STATUS_NONE)
, m_last_gen_sun_time(0)
, m_collect_sun_score(0)
, m_last_zombie_time(0)
, m_selected_plant(PlantType::PEASHOOTER)
, m_current_wave(0)
, m_total_waves(0)
, m_zombies_in_current_wave(0)
, m_zombies_spawned_in_wave(0)
, m_wave_status(WAVE_WAITING)
, m_wave_start_time(0)
, m_wave_interval(10000.0f)
, m_final_wave(false)
, m_is_background_music_playing(false)
, m_background_music_pos(0.0f)
, m_is_wave_warning_playing(false)
, m_wave_warning_start_time(0)
, m_is_plant_music_playing(false)
, m_plant_music_start_time(0)
, monster_id(0)
{
    m_random.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    m_suns.clear();
    m_zombies.clear();
    m_plants.clear();
    m_lawn_mowers.clear();

    m_score_text = std::make_unique<UIText>(RESOURCE_DIR"/fonts/Inter.ttf", std::to_string(m_collect_sun_score), 24, LAWN_START_X, LAWN_START_Y);
    m_score_text->Start();
    m_score_image = std::make_unique<UIIMAGE>(LAWN_START_X + 210, LAWN_START_Y + 30, RESOURCE_DIR"/bar4.png");

    char level_str[128];
    snprintf(level_str, sizeof(level_str), "level:%d", m_level);
    m_level_text = std::make_unique<UIText>(RESOURCE_DIR"/fonts/Inter.ttf", level_str, 24, 500, -350);
    m_level_text->Start();

    m_card_manager = std::make_unique<CardManager>();
    m_game_over_image = std::make_unique<UIIMAGE>(0, 0, RESOURCE_DIR"/ZombiesWon.png");
    m_success_text = std::make_unique<UIText>(RESOURCE_DIR"/fonts/Inter.ttf", "Game Win", 48, 0, 0);
    m_back_image = std::make_unique<UIIMAGE>(600, 320, RESOURCE_DIR"/back.png");

    m_background_music = std::make_unique<Util::BGM>(RESOURCE_DIR"/audio/music/2.75.mp3");
    m_wave_warning_music = std::make_unique<Util::BGM>(RESOURCE_DIR"/audio/music/awooga.mp3");
    m_lose_music = std::make_unique<Util::BGM>(RESOURCE_DIR"/audio/music/losemusic.mp3");
    m_plant_music = std::make_unique<Util::BGM>(RESOURCE_DIR"/audio/music/plant1.mp3");

    m_wave_progress_text = std::make_unique<UIText>(RESOURCE_DIR"/fonts/Inter.ttf", "Wave: 0/0", 24, 500, -300);
    m_wave_progress_text->Start();

    m_zombie_count_text = std::make_unique<UIText>(RESOURCE_DIR"/fonts/Inter.ttf", "Zombies: 0/0", 24, 500, -250);
    m_zombie_count_text->Start();
}

void PVZGame::Start(int level)
{
    LOG_INFO("Starting Plants vs Zombies Level {}!", level);
    PlayBackgroundMusic();

    m_game_status = GAME_STATUS_PLAYING;
    m_level = level;
    m_card_manager->InitCard(level);
    m_card_manager->RefreshCard(m_collect_sun_score);
    m_game_over_image->SetVisible(false);

    m_zombies.clear();
    m_plants.clear();
    m_suns.clear();
    InitWave();
    InitLawnMower();

    char level_str[128];
    snprintf(level_str, sizeof(level_str), "level:%d", m_level);
    m_level_text->SetStrText(level_str);

    m_collect_sun_score = 0;
    m_score_text->SetStrText(std::to_string(m_collect_sun_score));
}

void PVZGame::Update()
{
    if (m_game_status == GAME_STATUS_OVER)
    {
        m_game_over_image->Update();
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB))
        {
            m_game_status = GAME_STATUS_NONE;
            PasueBackgroundMusic();
            PasueLoseMusic();
        }

        return;
    }

    if (m_game_status == GAME_STATUS_WIN)
    {
        if (m_level == 10) {
            m_success_text->SetStrText("thank you for playing");
            m_success_text->Update();
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB))
            {
                m_game_status = GAME_STATUS_NONE;
                PasueBackgroundMusic();
            }
        }
        else {
            m_success_text->SetStrText("Game Win");
            m_success_text->Update();
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB))
            {
                Start(m_level + 1);
            }
        }

        return;
    }

    UpdateBGM();

    RefreshSun();
    RecycleSun();
    DrawSun();

    RecyclePlant();

    UpdateWave();
    RefreshZombie();
    RecycleZombie();

    HandleInput();
    CheckCollisions();

    UpdateProgress();

    m_score_text->Update();
    m_score_image->Update();
    m_level_text->Update();
    m_back_image->Update();
    m_wave_progress_text->Update();
    m_zombie_count_text->Update();

    m_card_manager->Update();

    CheckGameOver();
    UpdateLawnMower();

}

bool PVZGame::CheckBulletCollisions(float x, float y, bool is_slow_down, float damage)
{
    bool bulletHit = false;
    for (auto& zombie : m_zombies)
    {
        if (!zombie->IsCanAttack()) {
            continue;
        }

        float dx = x - zombie->GetTransform().translation.x;
        float dy = y - zombie->GetTransform().translation.y;
        float distance = sqrt(dx*dx + dy*dy);

        if (distance < 40.0f)
        { // Collision detected
            if (is_slow_down) {
                zombie->SlowDown();
            }

            zombie->TakeDamage(damage);
            bulletHit = true;
            break;
        }
    }

    return bulletHit;
}


bool PVZGame::ShouldShoot(int grid_x) {
    bool shoot = false;
    for (auto& zombie : m_zombies)
    {
        if (zombie->IsDestroyed() || !zombie->IsCanAttack()) {
            continue;
        }

        if (grid_x == zombie->GetGridy()) {
            shoot = true;
            break;
        }
    }

    return shoot;
}


void PVZGame::GenSun(float x, float y)
{
    if (x == 99999) {
        std::uniform_real_distribution<float> xDist(LAWN_START_X, LAWN_END_X);
        x = xDist(m_random);
    }

    if (y == 99999) {
        std::uniform_real_distribution<float> yDist(LAWN_END_Y, LAWN_START_Y);
        y = yDist(m_random);
    }

    m_suns.push_back(std::make_unique<Sun>(x, y, m_level == 1));
    LOG_INFO("Sun spawned at ({}, {})", x, y);
}


bool PVZGame::CheckCherryBombExplod(float x, float y, float radius) {
    bool explod = false;
    for (auto& zombie : m_zombies)
    {
        if (zombie->IsDestroyed()) {
            continue;
        }

        float dx = x - zombie->GetTransform().translation.x;
        float dy = y - zombie->GetTransform().translation.y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance <= radius) {
            explod = true;
            LOG_INFO("CherryBomb damaged zombie at x {} y {} distance: {} with radius: {}", x, y, distance, radius);
            break;
        }
    }

    return explod;
}


void PVZGame::CherryBombExplod(float x, float y, float radius, float damage) {
    for (auto& zombie : m_zombies)
    {
        if (zombie->IsDestroyed()) {
            continue;
        }

        float dx = x - zombie->GetTransform().translation.x;
        float dy = y - zombie->GetTransform().translation.y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance <= radius) {
            zombie->TakeDamage(damage, true);
            LOG_INFO("CherryBomb damaged zombie at x {} y {} distance: {} with radius: {} damage: {}", x, y, distance, radius, damage);
        }
    }
}


bool PVZGame::CheckPotatoMineExplod(float x, float y, float radius) {
    bool explod = false;
    for (auto& zombie : m_zombies)
    {
        if (zombie->IsDestroyed()) {
            continue;
        }

        float dx = x - zombie->GetTransform().translation.x;
        float dy = y - zombie->GetTransform().translation.y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance <= radius) {
            explod = true;
            LOG_INFO("PotatoMine damaged zombie at x {} y {} distance: {} with radius: {}", x, y, distance, radius);
            break;
        }
    }

    return explod;
}


void PVZGame::PotatoMineExplod(float x, float y, float radius, float damage) {
    for (auto& zombie : m_zombies)
    {
        if (!zombie->IsCanAttack()) {
            continue;
        }

        float dx = x - zombie->GetTransform().translation.x;
        float dy = y - zombie->GetTransform().translation.y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance <= radius) {
            zombie->TakeDamage(damage, true);
            LOG_INFO("PotatoMine damaged zombie at x {} y {} distance: {} with radius: {} damage: {}", x, y, distance, radius, damage);
            break;
        }
    }
}


bool PVZGame::CheckChomperDigest(float x, float y)
{
    for (auto& zombie : m_zombies)
    {
        if (zombie->IsDestroyed() || zombie->IsDie()) {
            continue;
        }

        float dx = x - zombie->GetTransform().translation.x;
        float dy = y - zombie->GetTransform().translation.y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance <= 58.0f) {
            zombie->TakeDamage(zombie->GetHealth(), true);
            LOG_INFO("Chomper damaged zombie at x {} y {} distance: {} with radius: {} damage: {}", x, y, distance);
            return true;
        }
    }

    return false;
}


void PVZGame::CheckLawnerTrigger(int grid_y, float x, float radius) {
    //LOG_INFO("Lawn mower destroyed zombie grid_y {}, x {}", grid_y, x);

    for (auto& zombie : m_zombies)
    {
        if (zombie->IsDestroyed() || zombie->IsDie()) {
            continue;
        }

        //LOG_INFO("Lawn mower destroyed zombie at ({}, {}), grid_y {}, x {}",zombie->GetTransform().translation.x, zombie->GetTransform().translation.y, grid_y, x);
        if (zombie->GetGridy() == grid_y && fabs(zombie->GetTransform().translation.x - x) < radius) {
            zombie->TakeDamage(zombie->GetHealth(), true);
            //LOG_INFO("Lawn mower destroyed zombie at ({}, {}), grid_y {}, x {}",zombie->GetTransform().translation.x, zombie->GetTransform().translation.y, grid_y, x);
        }
    }
}



void PVZGame::RefreshSun()
{
    float currentTime = Util::Time::GetElapsedTimeMs();
    if (currentTime - m_last_gen_sun_time > SUN_SPAWN_INTERVAL_AUTO)
    {
        GenSun();
        m_last_gen_sun_time = currentTime;

        LOG_TRACE("PVZGame::RefreshSun");
    }
}


void PVZGame::RecycleSun()
{
    for (auto it = m_suns.begin(); it != m_suns.end();)
    {
        (*it)->Update();
        if ((*it)->IsDestroyed())
        {
            if ((*it)->IsCollected()) {
                m_collect_sun_score += SUN_VALUE;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                LOG_INFO("Collected sun! Total: {}", m_collect_sun_score);
            }

            it = m_suns.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void PVZGame::DrawSun()
{
    for (auto it = m_suns.begin(); it != m_suns.end(); ++it)
    {
        (*it)->Draw();
    }
}


void PVZGame::GenZombie(ZombieType type, int monster_id)
{
    std::uniform_int_distribution<int> xDist(0, GRID_ROWS - 1);
    float gridY = xDist(m_random);

    float cellHeight = (LAWN_END_Y - LAWN_START_Y) / static_cast<float>(GRID_ROWS);
    float centerY = LAWN_START_Y + gridY * cellHeight + cellHeight / 2.0f;

    std::vector<std::string> paths;
    std::vector<std::string> attack_paths;
    char path[512];
    path[0] = '\0';

    if (type == ZombieType::NORMAL_ZOMBIE)
    {
        for (int i = 1; i <= 22; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/Zombie/zm/%d.png", RESOURCE_DIR, i);
            paths.push_back(path);
        }

        for (int i = 1; i <= 21; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/Zombie/zmattack/%d.png", RESOURCE_DIR, i);
            attack_paths.push_back(path);
        }

        m_zombies.push_back(std::make_unique<Zombie>(ZombieType::NORMAL_ZOMBIE, gridY, centerY, 100, ZOMBIE_SPEED, monster_id, paths, attack_paths));
    }
    else if (type == ZombieType::FLAG_ZOMBIE) {
        for (int i = 1; i <= 11; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/FlagZombie/FlagZombie/FlagZombie-%d.png", RESOURCE_DIR, i);
            paths.push_back(path);
        }

        for (int i = 0; i <= 10; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/FlagZombie/FlagZombieAttack/FlagZombieAttack-%d.png", RESOURCE_DIR, i);
            attack_paths.push_back(path);
        }

        m_zombies.push_back(std::make_unique<FlagZombie>(gridY, centerY, 100, monster_id, paths, attack_paths));
    }
    else if (type == ZombieType::CONEHEAD_ZOMBIE) {
        for (int i = 0; i <= 20; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/ConeheadZombie/ConeheadZombie/ConeheadZombie-%d.png", RESOURCE_DIR, i);
            paths.push_back(path);
        }

        for (int i = 0; i <= 10; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/ConeheadZombie/ConeheadZombieAttack/ConeheadZombieAttack-%d.png", RESOURCE_DIR, i);
            attack_paths.push_back(path);
        }

        m_zombies.push_back(std::make_unique<ConeheadZombie>(gridY, centerY, 100, monster_id, paths, attack_paths));
    }
    else if (type == ZombieType::BUCKETHEAD_ZOMBIE) {
        for (int i = 0; i <= 14; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/BucketheadZombie/BucketheadZombie/BucketheadZombie-%d.png", RESOURCE_DIR, i);
            paths.push_back(path);
        }

        for (int i = 0; i <= 10; i++) {
            snprintf(path, sizeof(path), "%s/Zombie/BucketheadZombie/BucketheadZombieAttack/BucketheadZombieAttack-%d.png", RESOURCE_DIR, i);
            attack_paths.push_back(path);
        }

        m_zombies.push_back(std::make_unique<BucketHeadZombie>(gridY, centerY, 100, monster_id, paths, attack_paths));
    }


    LOG_INFO("Zombie spawned! {} {} {}", gridY, cellHeight, centerY);
}


void PVZGame::RefreshZombie()
{
    if (m_wave_status != WAVE_SPAWNING) {
        return;
    }

    if (m_zombies_spawned_in_wave >= m_zombies_in_current_wave) {
        return;
    }

    float currentTime = Util::Time::GetElapsedTimeMs();
    float spaw_interval = m_final_wave ? 3000.0f : 6000.0f;

    if (currentTime - m_last_zombie_time > spaw_interval)
    {
        ZombieType zombieType = GetZombieTypeForLevel(m_level);
        GenZombie(zombieType, monster_id);
        m_zombies_spawned_in_wave++;
        monster_id++;
        m_last_zombie_time = currentTime;

        LOG_INFO("Spawned zombie {}/{} for wave {}", m_zombies_spawned_in_wave, m_zombies_in_current_wave, m_current_wave);
    }
}


void PVZGame::RecycleZombie()
{
    for (auto it = m_zombies.begin(); it != m_zombies.end();)
    {
        (*it)->Update();
        if ((*it)->IsDestroyed())
        {
            it = m_zombies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


bool PVZGame::CanPlacePlant(int grid_x, int grid_y)
{
    // Check if there's already a plant at this position
    for (auto& plant : m_plants)
    {
        if (plant->GetGridX() == grid_x && plant->GetGridY() == grid_y)
        {
            return false;
        }
    }

    return true;
}



void PVZGame::PlacePlant(int grid_x, int grid_y, float mouse_x, float mouse_y,  PlantType type)
{
    if (type == PlantType::SHOVEL) {
        for (auto& plant : m_plants)
        {
            if (plant->GetGridX() == grid_x && plant->GetGridY() == grid_y)
            {
                plant->TakeDamage(plant->GetHealth());
                LOG_INFO("Placed PlacePlant  Shovel at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
                break;
            }
        }

        return;
    }

    if (!CanPlacePlant(grid_x, grid_y))
    {
        LOG_INFO("Placed PlacePlant  error at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
        return;
    }

    bool is_suc = false;
    switch (type) {
        case PlantType::PEASHOOTER:
        {
            if (m_collect_sun_score >= PEASHOOTER_COST)
            {
                m_plants.push_back(std::make_unique<PeaShooter>(grid_x, grid_y, mouse_x, mouse_y, 100.f));
                m_collect_sun_score -= PEASHOOTER_COST;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                is_suc = true;
                LOG_INFO("Placed Peashooter at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            }
            else
            {
                LOG_INFO("Not enough sun for Peashooter! Need {}, have {}", PEASHOOTER_COST, m_collect_sun_score);
            }
            break;
        }
        case PlantType::SUN_FLOWER:
        {
            if (m_collect_sun_score >= SUNFLOWER_COST)
            {
                std::uniform_int_distribution<int> xDist(10000, SUN_SPAWN_INTERVAL);
                int interval = xDist(m_random);
                m_plants.push_back(std::make_unique<SunFlower>(grid_x, grid_y, mouse_x, mouse_y, 100.f, interval));
                m_collect_sun_score -= SUNFLOWER_COST;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                is_suc = true;
                LOG_INFO("Placed SunFlower at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            }
            else
            {
                LOG_INFO("Not enough sun for SunFlower! Need {}, have {}", SUNFLOWER_COST, m_collect_sun_score);
            }
            break;
        }
        case PlantType::CHERRY_BOMB:
        {
            if (m_collect_sun_score >= CHERRYBOMB_COST)
            {
                m_plants.push_back(std::make_unique<CherryBomb>(grid_x, grid_y, mouse_x, mouse_y, 300.f));
                m_collect_sun_score -= CHERRYBOMB_COST;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                is_suc = true;
                LOG_INFO("Placed CherryBomb at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            }
            else
            {
                LOG_INFO("Not enough sun for CherryBomb! Need {}, have {}", CHERRYBOMB_COST, m_collect_sun_score);
            }
            break;
        }
        case PlantType::WALL_NUT:
        {
            if (m_collect_sun_score >= WALLNUT_COST)
            {
                m_plants.push_back(std::make_unique<WallNut>(grid_x, grid_y, mouse_x, mouse_y, 400.f));
                m_collect_sun_score -= WALLNUT_COST;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                is_suc = true;
                LOG_INFO("Placed WallNut at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            }
            else
            {
                LOG_INFO("Not enough sun for WallNut! Need {}, have {}", WALLNUT_COST, m_collect_sun_score);
            }
            break;
        }
        case PlantType::POTATO_MINE:
        {
            if (m_collect_sun_score >= POTATO_MINE_COST)
            {
                m_plants.push_back(std::make_unique<PotatoMine>(grid_x, grid_y, mouse_x, mouse_y, 200.f));
                m_collect_sun_score -= POTATO_MINE_COST;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                is_suc = true;
                LOG_INFO("Placed PotatoMine at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            }
            else
            {
                LOG_INFO("Not enough sun for PotatoMine! Need {}, have {}", POTATO_MINE_COST, m_collect_sun_score);
            }
            break;
        }
        case PlantType::SNOWPEA:
        {
            if (m_collect_sun_score >= SNOWPEA_COST)
            {
                m_plants.push_back(std::make_unique<SnowPea>(grid_x, grid_y, mouse_x, mouse_y, 300.f));
                m_collect_sun_score -= SNOWPEA_COST;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                is_suc = true;
                LOG_INFO("Placed SnowPea at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            }
            else
            {
                LOG_INFO("Not enough sun for SnowPea! Need {}, have {}", SNOWPEA_COST, m_collect_sun_score);
            }
            break;
        }
        case PlantType::CHOMPER:
        {
            if (m_collect_sun_score >= CHOMPER_COST)
            {
                m_plants.push_back(std::make_unique<Chomper>(grid_x, grid_y, mouse_x, mouse_y, 300.f));
                m_collect_sun_score -= CHOMPER_COST;
                m_score_text->SetStrText(std::to_string(m_collect_sun_score));
                m_card_manager->RefreshCard(m_collect_sun_score);
                is_suc = true;
                LOG_INFO("Placed Chomper at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            }
            else
            {
                LOG_INFO("Not enough sun for Chomper! Need {}, have {}", CHOMPER_COST, m_collect_sun_score);
            }
            break;
        }
        default:
            LOG_INFO("Placed PlacePlant  type error at grid {}, {}, {}, {}", grid_x, grid_y, mouse_x, mouse_y);
            break;
    }

    if (is_suc) {
        PlayPlantMusic();
    }
}



void PVZGame::RecyclePlant()
{
    for (auto it = m_plants.begin(); it != m_plants.end();)
    {
        (*it)->Update();
        if ((*it)->IsDestroyed())
        {
            it = m_plants.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void PVZGame::CheckGameOver()
{
    for (int i = 0; i < GRID_ROWS; ++i) {
        bool hasLawnMowerInRow = false;
        bool hasZombieReachHouse = false;
        for (auto& zombie : m_zombies)
        {
            if (zombie->IsDestroyed() || zombie->IsDie() || zombie->GetGridy() != i)
            {
                continue;
            }

            if (zombie->HasReachedHouse())
            {
                LOG_INFO("PVZGame::CheckGameOver, monster_id:{} grid_y:{}", zombie->GetMonsterid(), zombie->GetGridy());
                hasZombieReachHouse = true;
                for (auto& lawnmower : m_lawn_mowers)
                {
                    if (!lawnmower->IsDestroyed() && lawnmower->GetGridY() == zombie->GetGridy())
                    {
                        lawnmower->Active();
                        hasLawnMowerInRow = true;
                        LOG_INFO("PVZGame::CheckGameOver, {}", zombie->GetGridy());
                        break;
                    }
                }
            }

            if (hasLawnMowerInRow)
                break;
        }

        if (hasZombieReachHouse && !hasLawnMowerInRow)
        {
            m_game_status = GAME_STATUS_OVER;
            m_game_over_image->SetVisible(true);
            PlayLoseMusic();
            LOG_INFO("Game Over! Zombies reached your house and no lawn mower available! {}", i);
            break;
        }
    }
}


void PVZGame::HandleInput()
{
    auto mousePos = Util::Input::GetCursorPosition();
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        glm::vec2 mouseVec2 = static_cast<glm::vec2>(mousePos);
        m_card_manager->OnMouseLBPressed(mouseVec2);
    }

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
    {
        if (mousePos.x >= 574 && mousePos.x <= 626 && mousePos.y >= 297 && mousePos.y <= 345) {
            m_game_status = GAME_STATUS_NONE;
            PasueBackgroundMusic();
            LOG_INFO("Clicked on grid ({}, {})", mousePos.x, mousePos.y);
        }
        else {
            if (mousePos.x >= LAWN_START_X && mousePos.x <= LAWN_END_X && mousePos.y >= LAWN_END_Y && mousePos.y <= LAWN_START_Y)
            {
                // 计算网格位置
                int gridX = static_cast<int>((mousePos.x - LAWN_START_X) / ((LAWN_END_X - LAWN_START_X) / GRID_COLS));
                int gridY = static_cast<int>((mousePos.y - LAWN_START_Y) / ((LAWN_END_Y - LAWN_START_Y) / GRID_ROWS));

                float cellWidth = (LAWN_END_X - LAWN_START_X) / static_cast<float>(GRID_COLS);
                float cellHeight = (LAWN_END_Y - LAWN_START_Y) / static_cast<float>(GRID_ROWS);
                float centerX = LAWN_START_X + gridX * cellWidth + cellWidth / 2.0f;
                float centerY = LAWN_START_Y + gridY * cellHeight + cellHeight / 2.0f;
                if (centerX > LAWN_END_X) {
                    centerX = LAWN_END_X - cellWidth / 2.0f;
                }

                PlantType draggedPlantType = m_card_manager->GetDraggedPlantType();
                PlacePlant(gridX, gridY, centerX, centerY, draggedPlantType);
                m_card_manager->OnPlantPlaced();
                LOG_INFO("Clicked on grid ({}, {}, {}, {}, {}, {})", gridX, gridY, mousePos.x, mousePos.y, centerX, centerY);
            }
            m_card_manager->OnMouseLBUp();
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::F1))
    {
        auto mousePos = Util::Input::GetCursorPosition();
        LOG_INFO("F1 ({}, {})", mousePos.x, mousePos.y);
    }
}


void PVZGame::CheckCollisions()
{
    for (auto& zombie : m_zombies) {
        if (zombie->IsDestroyed() || zombie->IsDie()) {
            continue;
        }

        bool eating = false;
        for (auto& plant : m_plants)
        {
            if (!plant->IsCanAttack()) {
                continue;
            }

            float dx = zombie->GetTransform().translation.x - plant->GetTransform().translation.x;
            float dy = zombie->GetTransform().translation.y - plant->GetTransform().translation.y;
            float distance = sqrt(dx*dx + dy*dy);

            if (distance < 50.0f)
            { // Zombie is eating plant
                if (!zombie->IsEating())
                {
                    zombie->StartEating();
                }
                plant->TakeDamage(20.0f * (Util::Time::GetDeltaTimeMs() / 1000.0f)); // Damage per second
                eating = true;
                break;
            }
        }

        if (!eating && zombie->IsEating())
        {
            zombie->StopEating();
        }
    }
}

void PVZGame::InitWave() {
    if (m_level <= 3) {
        m_total_waves = 2;
    }
    else if (m_level <= 6) {
        m_total_waves = 3;
    }
    else if (m_level <= 9) {
        m_total_waves = 4;
    }
    else {
        m_total_waves = 5;
    }

    m_current_wave = 0;
    m_zombies_in_current_wave = 0;
    m_zombies_spawned_in_wave = 0;
    m_wave_status = WAVE_WAITING;
    m_wave_start_time = Util::Time::GetElapsedTimeMs();
    m_final_wave = false;

    LOG_INFO("Level {} initialized with {} waves", m_level, m_total_waves);
}

void PVZGame::UpdateWave() 
{
    float currentTime = Util::Time::GetElapsedTimeMs();
    switch (m_wave_status)
    {
        case WAVE_WAITING:
            if (currentTime - m_wave_start_time >= m_wave_interval)
            {
                m_current_wave++;
                m_zombies_spawned_in_wave = 0;
                m_zombies_in_current_wave = GetZombiesPerWave(m_level, m_current_wave);
                m_wave_status = WAVE_SPAWNING;
                m_wave_start_time = Util::Time::GetElapsedTimeMs();
                m_final_wave = (m_current_wave == m_total_waves);
                LOG_INFO("Starting Wave {}/{} with {} zombies", m_current_wave, m_total_waves, m_zombies_in_current_wave);
            }
            break;
        case WAVE_SPAWNING:
            // 检查是否已生成完当前波的所有僵尸，且场上没有僵尸了
            if (m_zombies_spawned_in_wave >= m_zombies_in_current_wave && m_zombies.empty())
            {
                m_wave_status = WAVE_COMPLETED;
                LOG_INFO("Wave {} completed!", m_current_wave);
            }
            break;
        case WAVE_COMPLETED:
            if (m_current_wave >= m_total_waves)
            {
                m_wave_status = LEVEL_COMPLETED;
            }
            else
            {
                m_wave_status = WAVE_WAITING;
                m_wave_start_time = currentTime;
            }
            break;
        case LEVEL_COMPLETED:
            {
                m_game_status = GAME_STATUS_WIN;
                m_success_text->Start();
                LOG_INFO("Level {} completed! All waves defeated!", m_level);
            }
            break;
    }
}


int PVZGame::GetZombiesPerWave(int level, int wave)
{
    // 基础僵尸数量 + 关卡奖励 + 波数奖励
    int base_zombies = 1;
    int level_bonus = (level - 1) * 2;
    int wave_bonus = (wave - 1) * 1;
    int total_zombies = base_zombies + level_bonus + wave_bonus;

    //LOG_INFO("PVZGame::GetZombiesPerWave level:{} wave:{} total_zombies:{}", level, wave, total_zombies);
    return total_zombies;
}


ZombieType PVZGame::GetZombieTypeForLevel(int level)
{
    std::uniform_int_distribution<int> typeDist(1, 100);
    int roll = typeDist(m_random);

    if (level >= 10)
    {
        if (roll <= 40) return ZombieType::BUCKETHEAD_ZOMBIE;
        if (roll <= 70) return ZombieType::CONEHEAD_ZOMBIE;
        if (roll <= 85) return ZombieType::FLAG_ZOMBIE;
        return ZombieType::NORMAL_ZOMBIE;
    }
    else if (level >= 8)
    {
        if (roll <= 50) return ZombieType::CONEHEAD_ZOMBIE;
        if (roll <= 75) return ZombieType::FLAG_ZOMBIE;
        return ZombieType::NORMAL_ZOMBIE;
    }
    else if (level >= 5)
    {
        if (roll <= 60) return ZombieType::FLAG_ZOMBIE;
        return ZombieType::NORMAL_ZOMBIE;
    }

    return ZombieType::NORMAL_ZOMBIE;
}


void PVZGame::PlayBackgroundMusic() 
{
    if (m_background_music && !m_is_background_music_playing) 
    {
        m_background_music->SetVolume(50);
        m_background_music->Play(-1);
        m_is_background_music_playing = true;
        
        LOG_INFO("Background music started with loop");
    }
}


void PVZGame::PasueBackgroundMusic()
{
    if (m_is_background_music_playing) 
    {
        m_background_music->GetMusicPosition(m_background_music_pos);
        LOG_INFO("PVZGame::PasueBackgroundMusic {}", m_background_music_pos);

        m_background_music->Pause();
        m_is_background_music_playing = false;

        LOG_INFO("Background music stopped");
    }
}


void PVZGame::ResumeBackgroundMusic() {
    if (!m_is_background_music_playing)
    {
        m_background_music->PlayMusicInPosition(m_background_music_pos);
        m_is_background_music_playing = true;

        LOG_INFO("Background music resume");
    }
}


void PVZGame::PlayWaveWarning() 
{
    if (m_wave_warning_music) 
    {
        PasueBackgroundMusic();

        m_wave_warning_music->SetVolume(70);
        m_wave_warning_music->Play(0);  
        m_is_wave_warning_playing = true;
        m_wave_warning_start_time = Util::Time::GetElapsedTimeMs();
        LOG_INFO("Wave warning sound played");
    }
}

void PVZGame::PlayLoseMusic() 
{
    if (m_lose_music) 
    {
        PasueBackgroundMusic();

        m_lose_music->SetVolume(70);
        m_lose_music->Play(-1);

        LOG_INFO("Lose music played");
    }
}


void PVZGame::PasueLoseMusic() {
    if (m_lose_music)
    {
        m_lose_music->Pause();
    }
}


void PVZGame::PlayPlantMusic() {
    if (m_plant_music) {
        PasueBackgroundMusic();

        m_plant_music->SetVolume(70);
        m_plant_music->Play(0);
        m_is_plant_music_playing = true;
        m_plant_music_start_time = Util::Time::GetElapsedTimeMs();

        LOG_INFO("Plant music played");
    }

}


void PVZGame::UpdateBGM() 
{
    if (m_is_wave_warning_playing)
    {
        float currentTime = Util::Time::GetElapsedTimeMs();
        if (currentTime - m_wave_warning_start_time > 4000.0f)
        {
            m_is_wave_warning_playing = false;
            ResumeBackgroundMusic();
        }
    }

    if (m_is_plant_music_playing) {
        float currentTime = Util::Time::GetElapsedTimeMs();
        if (currentTime - m_plant_music_start_time > 500.0f) {
            m_is_plant_music_playing = false;
            ResumeBackgroundMusic();
        }
    }
}


void PVZGame::InitLawnMower() 
{
    m_lawn_mowers.clear();
    for (int row = 0; row < GRID_ROWS; row++) 
    {
        float cellHeight = (LAWN_END_Y - LAWN_START_Y) / static_cast<float>(GRID_ROWS);
        float centerY = LAWN_START_Y + row * cellHeight + cellHeight / 2.0f;
        float x = LAWN_START_X - 40;

        m_lawn_mowers.push_back(std::make_unique<LawnMower>(row, x, centerY));

        LOG_INFO("Initialized {} {} {} lawn mowers", row, x, centerY);
    }
}


void PVZGame::UpdateLawnMower()
{
    for (auto it = m_lawn_mowers.begin(); it != m_lawn_mowers.end();) 
    {
        (*it)->Update();
        if ((*it)->IsDestroyed()) 
        {
            it = m_lawn_mowers.erase(it);
        } 
        else 
        {
            ++it;
        }
    }
}

void PVZGame::UpdateProgress()
{
    char wave_text[128];
    snprintf(wave_text, sizeof(wave_text), "Wave: %d/%d", m_current_wave, m_total_waves);
    m_wave_progress_text->SetStrText(wave_text);

    int total_remaining = GetTotalRemainingZombies();
    int current_zombies = static_cast<int>(m_zombies.size());

    char zombie_text[128];
    snprintf(zombie_text, sizeof(zombie_text),"Zombies: %d | Left: %d",current_zombies,total_remaining);
    m_zombie_count_text->SetStrText(zombie_text);
}


int PVZGame::GetRemainingZombiesInWave()
{
    if (m_wave_status != WAVE_SPAWNING)
    {
        return 0;
    }

    return m_zombies_in_current_wave - m_zombies_spawned_in_wave;
}

int PVZGame::GetTotalRemainingZombies()
{
    int total = static_cast<int>(m_zombies.size());
    if (m_wave_status == WAVE_SPAWNING)
    {
        total += GetRemainingZombiesInWave();
    }

    for (int wave = m_current_wave + 1; wave <= m_total_waves; wave++)
    {
        total += GetZombiesPerWave(m_level, wave);
    }

    return total;
}

