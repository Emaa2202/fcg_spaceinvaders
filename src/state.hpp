/*--------------------------------------
----------------------------------------
Questo file contiene:
    State
    Firme delle funzioni update
----------------------------------------
--------------------------------------*/
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <string>

#include "interfaces.hpp"
#include "entities.hpp"
#include "assets.hpp"

struct State {
    Assets assets;

    std::string audioDir;
    sf::Music soundtrack;
    
    sf::RenderWindow window;
    sf::Sprite background_sprite;
    
    Player player;
        std::vector<playerBullet> playerBullets;
        sf::Sound playerBullets_sound; 

    Shield shield;
        bool isShield = false; //controlla se bisogna disegnare scudo
        sf::Sound shield_sound;
    
    Nuke nuke;
        bool existsNuke = false;
        sf::Sound nukeSound;
    
    std::vector<Enemy> enemies;
        int rows = 6;
        int columns = 12;
        int enemiesQuantity; //per regolare vel nemici 
        sf::Clock move_clock; //per spostamento nemici
        bool right_dir = true; //direzione nemici, prima era sotto ma mi serve persistente
        std::vector<enemyBullet> enemyBullets;

    ShieldCharger shieldCharger;
        bool shieldChargerReleased = false;
        sf::Sound shieldChargerSound;

    Nukeship nukeship;
        bool existsNukeShip = false;

    std::vector<Explosion> explosions;
        sf::Sound playerExplosion_sound;

    Ui ui;

    Start start;
        bool startScreen = true;

    End end;
        bool gameOver = false; //per far apparirre la schermata gameover
        sf::Clock gameoverTransition_clock; //per non far apparire la schermata gameover instantaneamente
        bool gameoverTransition = false;
    
    Pause pause;
        bool isPaused = false;

    sf::Clock nextLevelTransition_clock; 
    bool nextLevelTransition = false;
    
    State();
    void initEnemies();
    void playMusic(const std::string& trackName);
};

void movePlayer(State& gs);
void shootPlayerBullets(State& gs);
void enablePlayerShield(State& gs);
void shootPlayerNuke(State& gs);
void updateNukeCollision(State& gs);
void pickShieldCharger(State& gs);
void updateEnemyBulletsCollisions(State& gs);

void moveEnemies(State& gs);
void shootEnemyBullets(State& gs);
void updatePlayerBulletsCollisions(State& gs);
void dropShieldCharger(State& gs, Enemy enemy); //inserita nelle collisioni

void spawnNukeship(State& gs);
void updateNukeshipCollisions(State& gs);

void updateGameOver(State& gs);
void updateLevel(State& gs);

void updateIngamePlayer(State& gs);
void updateIngameEnemies(State& gs);
void updateIngameNukeship(State& gs);