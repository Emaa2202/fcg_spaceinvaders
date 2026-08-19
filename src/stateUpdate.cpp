/*--------------------------------------
----------------------------------------
Questo file contiene:
    Costruttori di Start, End, Ui, State
    Implementazione delle funzioni update
----------------------------------------
--------------------------------------*/
#include <algorithm> //per clamp che mi semplifica il movimento 
#include <ctime> //per rand
#include <cstdlib>

#include "state.hpp"
#include "graphics/player.hpp" //player + sfondo
#include "graphics/enemy1.hpp"
#include "graphics/enemy2.hpp"
#include "graphics/enemy3.hpp"
#include "graphics/enemyBullet.hpp"
#include "graphics/explosion.hpp"

#include "graphics/font.hpp"

/*-----------------------------
-----------Costruttori---------
------------------------------*/
Start::Start() :
        title(font), 
        caption(font)
    {    
        font.openFromMemory(font_ttf, font_ttf_len);

        //titolo
        title.setString("Space Invaders");
        title.setCharacterSize(256);

        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));

        title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, (sf::VideoMode::getDesktopMode().size.y/ 2.0) * 0.5));

        //mex di premere invio
        caption.setString("Premi invio per giocare");
        caption.setCharacterSize(64);

        sf::FloatRect cbounds = caption.getLocalBounds();
        caption.setOrigin(sf::Vector2f(cbounds.size.x / 2, cbounds.size.y / 2));

        caption.setFillColor(sf::Color::White);
        caption.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, (sf::VideoMode::getDesktopMode().size.y/ 2.0) * 0.8));

}


End::End() :
        title(font), 
        caption(font),
        finalScore(font)
    {    
        font.openFromMemory(font_ttf, font_ttf_len);

        //titolo
        title.setString("Game Over!");
        title.setCharacterSize(256);

        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));

        title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, (sf::VideoMode::getDesktopMode().size.y/ 2.0) * 0.3));

        //mex di premere invio
        caption.setString("Premi invio per giocare ancora");
        caption.setCharacterSize(64);

        sf::FloatRect cbounds = caption.getLocalBounds();
        caption.setOrigin(sf::Vector2f(cbounds.size.x / 2, cbounds.size.y / 2));

        caption.setFillColor(sf::Color::White);
        caption.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, (sf::VideoMode::getDesktopMode().size.y/ 2.0) * 0.7));

        //punti
        finalScore.setString("Punteggio: 0");
        finalScore.setCharacterSize(128);
        finalScore.setFillColor(sf::Color::White);
        
        sf::FloatRect sbounds = finalScore.getLocalBounds();
        finalScore.setOrigin(sf::Vector2f(sbounds.size.x / 2, sbounds.size.y / 2));
        finalScore.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, (sf::VideoMode::getDesktopMode().size.y/ 2.0) * 0.5));
}


Ui::Ui() :
        livesText(font),
        scoreText(font),
        levelText(font) 
    {    
        font.openFromMemory(font_ttf, font_ttf_len);
        
        //vite
        livesText.setString("Vite: 3");
        livesText.setCharacterSize(64);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x * 0.01, sf::VideoMode::getDesktopMode().size.y * 0.89));

        //punti
        scoreText.setString("Punteggio: 0");
        scoreText.setCharacterSize(64);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(livesText.getGlobalBounds().position.x + livesText.getGlobalBounds().size.x * 1.5, sf::VideoMode::getDesktopMode().size.y * 0.89));
    
        //contatore livelli
        levelText.setString("Livello: 1");
        levelText.setCharacterSize(64);
        levelText.setFillColor(sf::Color::White);
        levelText.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x * 0.85, sf::VideoMode::getDesktopMode().size.y * 0.89));
}


State::State() :
        //caricamento texture e collegamento agli sprite
        background(spacebackground_jpg, spacebackground_jpg_len),
        background_sprite(background),

        player_texture(player_png, player_png_len),
        player(player_texture),
        playerBullet_texture(playerBullet_png, playerBullet_png_len),

        enemy1_texture(enemy1_sheet_png, enemy1_sheet_png_len),
        enemy2_texture(enemy2_sheet_png, enemy2_sheet_png_len),
        enemy3_texture(enemy3_sheet_png, enemy3_sheet_png_len),

        enemyBullet_texture(enemyBullet_png, enemyBullet_png_len),
        
        explosion_texture(explosion_png, explosion_png_len),
        explosion_sprite(explosion_texture)
    
    {   
        //creazione finestra
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode({desktop.size.x, desktop.size.y}), "Space Invaders");
        window.setFramerateLimit(60);

        //sfondo di dimensione dello schermo
        double background_scale_x = (static_cast<float>(desktop.size.x) / background.getSize().x); 
        double background_scale_y = (static_cast<float>(desktop.size.y) / background.getSize().y);
        background_sprite.setScale(sf::Vector2f(background_scale_x, background_scale_y));

        initEnemies();        
        
    }

//posizionamento nemici
void State::initEnemies() {
    float screenWidth = static_cast<float>(sf::VideoMode::getDesktopMode().size.x);
    float screenHeight = static_cast<float>(sf::VideoMode::getDesktopMode().size.y);
    
    float distX = 250; //distanze tra nemici
    float distY = 130;
    
    float gridWidth = (columns - 1) * distX; //dimensioni griglia
    float gridHeight = (rows - 1) * distY;
    
    float startX = (screenWidth - gridWidth) / 2; //posizionamento effettivo griglia
    float startY = screenHeight * 0.07;
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            float posX = startX + (j * distX);
            float posY = startY + (i * distY);
        
            if(i == 4 ||i == 5){
                enemies.push_back(Enemy(enemy1_texture, Type1, j, sf::Vector2f(posX, posY)));
            } 
            else if(i == 2 || i == 3){
                enemies.push_back(Enemy(enemy2_texture, Type2, j, sf::Vector2f(posX, posY)));
            }
            else {
                enemies.push_back(Enemy(enemy3_texture, Type3, j, sf::Vector2f(posX, posY)));
            }
        }
    }
}


/*-----------------------------
-------------Update------------
------------------------------*/
void updatePlayer(State&gs) {
    int speed = 10; //controllando a ogni frame (non piu handle) va diminuita la velocita 

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) { //isKeyPressed invece di keyPressed per controllo tempo reale, permette di muoversi e sparare insieme
	    gs.player.sprite.move(sf::Vector2f(-speed, 0));
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		gs.player.sprite.move(sf::Vector2f(speed, 0));
	}

    sf::Vector2f pos = gs.player.sprite.getPosition();
    float half_width = (gs.player_texture.getSize().x * gs.player.sprite.getScale().x) / 2.0;
    float min_x = half_width; 
    float max_x = static_cast<float>(gs.window.getSize().x) - half_width;

    pos.x = std::clamp(pos.x, min_x, max_x); //costringe posx ad essere compresa tra min e max
    gs.player.sprite.setPosition(pos);
}

void updateplayerBullets(State& gs) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        if(gs.player.canShoot()) {
            gs.playerBullets.push_back(playerBullet(gs.playerBullet_texture, gs.player.sprite.getPosition()));
            gs.player.cooldown.restart();
        }
    }   
    //scorrimento proiettili player
    for(auto& bullet : gs.playerBullets) {
        bullet.sprite.move(sf::Vector2f(0, -bullet.speed));        
    } 
}

//spostamento nemici
void updateEnemies(State& gs) {
    if(gs.move_clock.getElapsedTime().asSeconds() >= 1.0) {
        if(!gs.enemies.empty()) {
            float dist = 60;
            bool edge = false;

            float minX = gs.enemies[0].sprite.getPosition().x; //trova estremi
            float maxX = minX;
            for(auto& enemy : gs.enemies) {
                float x = enemy.sprite.getPosition().x;
                if(x > maxX) maxX = x;
                if(x < minX) minX = x;
            }
            
            float windowWidth = static_cast<float>(gs.window.getSize().x);
            
            if((maxX + dist >= windowWidth - 170.0 && gs.right_dir) || (minX - dist <= 170.0 && !gs.right_dir)) {
                edge = true;
            }

            if(edge) {
                gs.right_dir = !gs.right_dir;
                for(auto& enemy : gs.enemies) {
                    enemy.sprite.move(sf::Vector2f(0.0, 50.0)); //nemici scendono
                    enemy.animate(); //sprite animaz
                }
            }
            else {
                for(auto& enemy : gs.enemies) {
                    if(gs.right_dir) enemy.sprite.move(sf::Vector2f(dist, 0.0));
                    else enemy.sprite.move(sf::Vector2f(-dist, 0.0));
                    enemy.animate();  
                }
            }
             gs.move_clock.restart();
        }
    }     
}


//proiettili nemici
void updateEnemyBullets(State& gs) { 
    for(auto& enemy : gs.enemies) {
        int shoot = rand() % 100;
        if(enemy.isFrontEnemy(enemy, gs.enemies)) {
            switch(enemy.type) {
                case Type1:
                    if(shoot < 1 && enemy.enemyBullet_clock.getElapsedTime().asSeconds() >= 3.0){ 
                        gs.enemyBullets.push_back(enemyBullet(gs.enemyBullet_texture, enemy.sprite.getPosition()));
                        enemy.enemyBullet_clock.restart();
                    }
                break;

                case Type2:
                    if(shoot < 1 && enemy.enemyBullet_clock.getElapsedTime().asSeconds() >= 1.5){ 
                        gs.enemyBullets.push_back(enemyBullet(gs.enemyBullet_texture, enemy.sprite.getPosition()));
                        enemy.enemyBullet_clock.restart();
                    }
                break;

                case Type3:
                    if(shoot < 1 && enemy.enemyBullet_clock.getElapsedTime().asSeconds() >= 0.25){ 
                        gs.enemyBullets.push_back(enemyBullet(gs.enemyBullet_texture, enemy.sprite.getPosition()));
                        enemy.enemyBullet_clock.restart();
                    }
                break;
            }
        }
    }

    //scorrimento proiettili nemici
    for(auto& enemyBullet : gs.enemyBullets) {
        enemyBullet.pos.y += enemyBullet.speed;        
        enemyBullet.sprite.setPosition(enemyBullet.pos); 
    }
}


//collisioni proiettile giocatore
void updatePlayerBulletsCollisions(State& gs) { 
    for(auto& playerBullet : gs.playerBullets) {
        sf::FloatRect playerBulletBounds = playerBullet.sprite.getGlobalBounds();
    
        for(auto& enemy : gs.enemies) {
            if(enemy.isAlive) {
                sf::FloatRect enemyBounds = enemy.sprite.getGlobalBounds();
            
                if(playerBulletBounds.findIntersection(enemyBounds).has_value()) {
                    enemy.isAlive = false;
                    gs.player.score += enemy.points;

                    Explosion exp(gs.explosion_texture);
                    exp.sprite.setPosition(enemy.sprite.getPosition());
                    exp.sprite.setScale(sf::Vector2f(0.5, 0.5));
                    gs.explosions.push_back(exp);

                    playerBullet.sprite.setPosition(sf::Vector2f(0, -500));
                    break;
                }
            }
            
        }
    
    }
    
    for(int i = gs.enemies.size()-1; i >= 0; i--) {
        if(!gs.enemies[i].isAlive) gs.enemies.erase(gs.enemies.begin() + i);
    }

    for(int i = gs.playerBullets.size() -1; i >= 0; i--) {
        if(gs.playerBullets[i].sprite.getPosition().y < 0.0) gs.playerBullets.erase(gs.playerBullets.begin() + i);
    }

    for(int i = gs.explosions.size()-1; i >= 0; i--) {
        if(gs.explosions[i].clock.getElapsedTime().asSeconds() >= 0.2) {
            gs.explosions.erase(gs.explosions.begin() + i);
        }
    }
}


//collisioni proiettili dei nemici
void updateEnemyBulletsCollisions(State& gs) {
    for(auto& enemyBullet : gs.enemyBullets) {
        sf::FloatRect enBulletsBounds = enemyBullet.sprite.getGlobalBounds();
        sf::FloatRect playerBounds = gs.player.sprite.getGlobalBounds();
        
        if(enBulletsBounds.findIntersection(playerBounds).has_value()) {
            gs.player.lifes--;
            
            Explosion exp(gs.explosion_texture);
            exp.sprite.setPosition(gs.player.sprite.getPosition());
            exp.sprite.setScale(sf::Vector2f(0.5, 0.5));
            gs.explosions.push_back(exp);
            
            enemyBullet.pos.y = -500;
        }
    }

    for(int i = gs.enemyBullets.size()-1; i >= 0; i--) { //messo indici al contrario perchè nell altro modo gli elem scalano di una pos
        if(gs.enemyBullets[i].pos.y < 0.0) gs.enemyBullets.erase(gs.enemyBullets.begin() + i);
    }

    for(int i = gs.explosions.size()-1; i >= 0; i--) {
        if(gs.explosions[i].clock.getElapsedTime().asSeconds() >= 0.2) { //dopo un po viene tolta
            gs.explosions.erase(gs.explosions.begin() + i);
        }
    }
}


void updateGameOver(State& gs) {
    bool lost = false;
    
    //se i nemici si avvicinano troppo
    if(!gs.enemies.empty()) {
        for(auto& enemy : gs.enemies) {
            float maxY = enemy.sprite.getPosition().y;
            if(maxY > sf::VideoMode::getDesktopMode().size.y * 0.6) lost = true;
        }
    }

    if(gs.player.lifes < 0){
        lost = true;
    }

    if(lost) {
        gs.gameoverTransition = true;
        gs.gameoverTransition_clock.restart(); 
    }

}

void updateLevel(State& gs) {
    if(gs.enemies.empty() && !gs.nextLevelTransition) {
        gs.nextLevelTransition = true;
        gs.nextLevelTransition_clock.restart(); 
    }
}
