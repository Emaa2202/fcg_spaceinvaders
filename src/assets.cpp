/*------------------------------------------------------------
--------------------------------------------------------------
Questo file contiene:
    Inizializzazioni di sprite e suoni 
--------------------------------------------------------------
------------------------------------------------------------*/
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "assets.hpp"

#include "graphics/player.hpp" //player + sfondo
#include "graphics/enemy1.hpp"
#include "graphics/enemy2.hpp"
#include "graphics/enemy3.hpp"
#include "graphics/enemyBullet.hpp"
#include "graphics/explosion.hpp"
#include "graphics/shield.hpp"
#include "graphics/shieldCharger.hpp"
#include "graphics/nuke.hpp"
#include "graphics/nukeship.hpp"

#include "sounds/playerBulletSound.hpp"
#include "sounds/playerExplosion.hpp"
#include "sounds/shield.hpp"
#include "sounds/shieldCharger.hpp"
#include "sounds/nuke.hpp"


Assets::Assets() :
        background(spacebackground_jpg, spacebackground_jpg_len),
        
        player_texture(player_png, player_png_len),
        playerBullet_texture(playerBullet_png, playerBullet_png_len),
        playerBullets_buffer(playerBullet_mp3, playerBullet_mp3_len),

        shield_texture(shield_png, shield_png_len),
        shieldSound_buffer(shield_mp3, shield_mp3_len),
        
        nuke_texture(nuke_png, nuke_png_len),
        nukeSound_buffer(nuke_mp3, nuke_mp3_len),

        enemy1_texture(enemy1_sheet_png, enemy1_sheet_png_len),
        enemy2_texture(enemy2_sheet_png, enemy2_sheet_png_len),
        enemy3_texture(enemy3_sheet_png, enemy3_sheet_png_len),
        enemyBullet_texture(enemyBullet_png, enemyBullet_png_len),

        shieldCharger_texture(shieldCharger_png, shieldCharger_png_len),
        shieldChargerSound_buffer(shieldCharger_mp3, shieldCharger_mp3_len),
        
        nukeShip_texture(nukeShip_png, nukeShip_png_len),
        
        explosion_texture(explosion_png, explosion_png_len),
        playerExplosion_buffer(playerExplosion_mp3, playerExplosion_mp3_len)    
    {
}
