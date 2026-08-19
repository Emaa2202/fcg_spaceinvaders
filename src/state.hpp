/*--------------------------------------
----------------------------------------
Questo file contiene:
    State
    Firme delle funzioni update
----------------------------------------
--------------------------------------*/
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

#include "interfaces.hpp"
#include "entities.hpp"


struct State {
    std::string audioDir;
    sf::Music soundtrack;

    sf::RenderWindow window;

    sf::Texture background;
    sf::Sprite background_sprite;

    sf::Texture player_texture;
    Player player;

	std::vector<playerBullet> playerBullets;
    sf::Texture playerBullet_texture;
    sf::SoundBuffer playerBullets_buffer;
    sf::Sound playerBullets_sound; 

    std::vector<Enemy> enemies;
    int rows = 6;
    int columns = 12;
    sf::Texture enemy1_texture;
    sf::Texture enemy3_texture;
    sf::Texture enemy2_texture;
    int enemiesQuantity; //per regolare vel nemici

    sf::Clock move_clock; //per spostamento nemici
    bool right_dir = true; //direzione nemici, prima era sotto ma mi serve persistente

    sf::Texture enemyBullet_texture;
    std::vector<enemyBullet> enemyBullets;

    sf::Texture explosion_texture;
    sf::Sprite explosion_sprite;
    std::vector<Explosion> explosions;
    sf::SoundBuffer playerExplosion_buffer;
    sf::Sound playerExplosion_sound;

    bool gameOver = false; //per far apparirre la schermata gameover
    bool startScreen = true;

    sf::Clock gameoverTransition_clock; //per non far apparire la schermata gameover instantaneamente
    bool gameoverTransition = false;

    sf::Clock nextLevelTransition_clock; 
    bool nextLevelTransition = false;

    Ui ui;
    Start start;
    End end;

    State();
    void initEnemies();
    void playMusic(const std::string& trackName);
};

void updatePlayer(State& gs);
void updateplayerBullets(State& gs);
void updateEnemies(State& gs);
void updateEnemyBullets(State& gs);
void updatePlayerBulletsCollisions(State& gs);
void updateEnemyBulletsCollisions(State& gs);
void updateGameOver(State& gs);
void updateLevel(State& gs);