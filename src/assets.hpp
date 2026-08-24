/*------------------------------------------------------------
--------------------------------------------------------------
Questo file contiene:
    Dichiarazioni di texture e soundbuffer
--------------------------------------------------------------
------------------------------------------------------------*/
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>


struct Assets {
    sf::Texture background;
    sf::Texture player_texture;
    sf::Texture shield_texture;
    sf::SoundBuffer shieldSound_buffer;
    sf::Texture shieldCharger_texture;
    sf::SoundBuffer shieldChargerSound_buffer;
    sf::Texture nuke_texture;
    sf::Texture nukeShip_texture;
    sf::Texture playerBullet_texture;
    sf::SoundBuffer playerBullets_buffer;
    sf::Texture enemy1_texture;
    sf::Texture enemy3_texture;
    sf::Texture enemy2_texture;
    sf::Texture enemyBullet_texture;
    sf::SoundBuffer playerExplosion_buffer;
    sf::Texture explosion_texture;
    sf::SoundBuffer nukeSound_buffer;

    Assets();
};