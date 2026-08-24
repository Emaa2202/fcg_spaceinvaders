/*------------------------------------------------------------
--------------------------------------------------------------
Questo file contiene:
    Tutte le funzioni update con relative ausiliarie
--------------------------------------------------------------
------------------------------------------------------------*/
#include <SFML/Audio.hpp>
#include <algorithm> //per clamp che mi semplifica il movimento 
#include <ctime> //per rand
#include <cstdlib>

#include "state.hpp"


/*-----------------------------------------------------
----Ausiliarie per eliminare esplosioni e proiettili---
-----------------------------------------------------*/
void expireExplosions(State& gs) {
    for(int i = gs.explosions.size()-1; i >= 0; i--) {
        if(gs.explosions[i].clock.getElapsedTime().asSeconds() >= gs.explosions[i].duration) {
            gs.explosions.erase(gs.explosions.begin() + i);
        }
    }
}

void eraseEnemies(State& gs) {
    for(int i = gs.enemies.size()-1; i >= 0; i--) {
        if(!gs.enemies[i].isAlive) {
            gs.enemies.erase(gs.enemies.begin() + i);   
        }
    }
}
    
void erasePlayerBullets(State& gs) {    
    for(int i = gs.playerBullets.size() -1; i >= 0; i--) {
        if(gs.playerBullets[i].sprite.getPosition().y < 0.0) gs.playerBullets.erase(gs.playerBullets.begin() + i);
    }
}


void eraseEnemyBullets(State& gs) {
    for(int i = gs.enemyBullets.size()-1; i >= 0; i--) { //messo indici al contrario perchè nell altro modo gli elem scalano di una pos
        if(gs.enemyBullets[i].pos.y < 0.0) gs.enemyBullets.erase(gs.enemyBullets.begin() + i);
    }
}


/*-----------------------------
-------------Update------------
------------------------------*/
void movePlayer(State&gs) {
    int speed = 10; //controllando a ogni frame (non piu handle) va diminuita la velocita 

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) { //isKeyPressed invece di keyPressed per controllo tempo reale, permette di muoversi e sparare insieme
	    gs.player.sprite.move(sf::Vector2f(-speed, 0));
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
		gs.player.sprite.move(sf::Vector2f(speed, 0));
	}

    sf::Vector2f pos = gs.player.sprite.getPosition();
    float half_width = (gs.assets.player_texture.getSize().x * gs.player.sprite.getScale().x) / 2.0;
    float min_x = half_width; 
    float max_x = static_cast<float>(gs.window.getSize().x) - half_width;

    pos.x = std::clamp(pos.x, min_x, max_x); //costringe posx ad essere compresa tra min e max
    gs.player.sprite.setPosition(pos);
}


void shootPlayerBullets(State& gs) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        if(gs.player.canShoot()) {
            gs.playerBullets.push_back(playerBullet(gs.assets.playerBullet_texture, gs.player.sprite.getPosition()));
            gs.playerBullets_sound.play();
            gs.player.cooldown.restart();
        }
    }   
    //scorrimento proiettili player
    for(auto& bullet : gs.playerBullets) {
        bullet.sprite.move(sf::Vector2f(0, -bullet.speed));        
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
                    dropShieldCharger(gs, enemy);
                    enemy.isAlive = false;
                    gs.enemiesQuantity--; //decrementa contatore nemici
                    gs.player.score += enemy.points;

                    Explosion exp(0.5, gs.assets.explosion_texture, enemy.sprite.getPosition());
                    gs.explosions.push_back(exp);

                    playerBullet.sprite.setPosition(sf::Vector2f(0, -500));
                    break;
                }
            }
            
        }
    
    }
    
    eraseEnemies(gs);
    erasePlayerBullets(gs);
    expireExplosions(gs);
}


void enablePlayerShield(State& gs) {
    if(gs.player.shields > 0) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift) && gs.shield.cooldown.getElapsedTime().asSeconds() >= 1.0) {
            gs.isShield = true;
            gs.shield.cooldown.restart();
            gs.shield.clock.restart();
        }

        if(gs.isShield) {
            gs.shield.sprite.setPosition(gs.player.sprite.getPosition());
            if(gs.shield.clock.getElapsedTime().asSeconds() >= 0.7) {
                gs.player.shields--;
                gs.isShield = false;
            }
        }
    }
}


void shootPlayerNuke(State& gs) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
        if(gs.player.nukes == 1) {
            gs.existsNuke = true;
            gs.player.nukes--;
            gs.nuke.sprite.setPosition(gs.player.sprite.getPosition());
        }

    }       
    if(gs.existsNuke) {
        gs.nuke.sprite.move(sf::Vector2f(0, -gs.nuke.speed));    
        if(gs.nuke.sprite.getPosition().y <= 0.0) gs.existsNuke = false;
    }  
}


void updateNukeCollision(State& gs) {
    bool hitTarget = false;
    sf::Vector2f hitPos;
    sf::FloatRect nukeBounds = gs.nuke.sprite.getGlobalBounds();
    
    //impatto
    for(auto& enemy : gs.enemies) {
        if(enemy.isAlive) {
            sf::FloatRect enemyBounds = enemy.sprite.getGlobalBounds();

            if(nukeBounds.findIntersection(enemyBounds).has_value()) {
                hitTarget = true;
                hitPos = enemy.sprite.getPosition(); 

                gs.nukeSound.play();
                gs.nuke.sprite.setPosition(sf::Vector2f(0, -500));
                gs.existsNuke = false; 
                break; 
            }
        }
    }

    //area esplosione
    if(hitTarget) {
        float damageWidth = 500.0; //dim totali
        float damageHeight = 260.0;
        sf::Vector2f damagePosition(hitPos.x - (damageWidth / 2.0), hitPos.y - (damageHeight / 2.0)); //coordinate angolo alto sx dell esplosione
        sf::Vector2f damageSize(damageWidth, damageHeight); //raggruppa le dim in un unico oggetto 
        sf::FloatRect damageArea(damagePosition, damageSize); //unisce pos iniziale e dimensioni

        int tempCounter = 0; //per far apparire solo 1 esplosione e non 3x3
        for(auto& enemy : gs.enemies) {
            if(enemy.isAlive) {
                sf::FloatRect enemyBounds = enemy.sprite.getGlobalBounds();
                if(damageArea.findIntersection(enemyBounds).has_value()) {
                    dropShieldCharger(gs, enemy);                    
                    enemy.isAlive = false;
                    gs.enemiesQuantity--; 
                    gs.player.score += enemy.points;

                    tempCounter++;
                    if(tempCounter == 1) { 
                        Explosion exp(3.0, gs.assets.explosion_texture, hitPos);
                        gs.explosions.push_back(exp);
                    }
                }
            }
        }
        tempCounter = 0;
    }
    
    eraseEnemies(gs);
    expireExplosions(gs);
}


void spawnNukeship(State& gs) {
    float distY = sf::VideoMode::getDesktopMode().size.y * 0.04; //per impostare altezza navicella + controlli sotto
    if(!gs.existsNukeShip) {
        float spawnProb = rand() % 10000;
        if(spawnProb <= 1.0) gs.existsNukeShip = true; //1 su 10k frame circa
     
        float dirProb = rand() % 100;
        if(dirProb <= 50) {
            gs.nukeship.rightDirection = true;
            gs.nukeship.sprite.setPosition(sf::Vector2f(0.0, distY));
        }
        else{
            gs.nukeship.rightDirection = false;
            gs.nukeship.sprite.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x, distY));
        }
        gs.nukeship.lifes = 3;
        gs.nukeship.setDirection(gs.nukeship.rightDirection);
    }
    else {    
        gs.nukeship.move(gs.nukeship.rightDirection);

        if(!gs.nukeship.rightDirection && gs.nukeship.sprite.getPosition() == sf::Vector2f(0.0, distY)) { 
            gs.existsNukeShip = false;
        }
        else if(gs.nukeship.rightDirection && gs.nukeship.sprite.getPosition() == sf::Vector2f(sf::VideoMode::getDesktopMode().size.x, distY)) {
            gs.existsNukeShip = false;
        }
    }
}


void updateNukeshipCollisions(State& gs) {
    if(gs.existsNukeShip) {
        sf::FloatRect nukeshipBounds = gs.nukeship.sprite.getGlobalBounds();
        for(auto& bullet : gs.playerBullets) {
            sf::FloatRect playerBulletBounds = bullet.sprite.getGlobalBounds();
            
            if(playerBulletBounds.findIntersection(nukeshipBounds).has_value()) {
                gs.nukeship.lifes--;
                
                bullet.sprite.setPosition(sf::Vector2f(0, -500));
                Explosion exp(0.5, gs.assets.explosion_texture, gs.nukeship.sprite.getPosition());
                gs.explosions.push_back(exp);
            }

        }

        sf::FloatRect nukeBounds = gs.nuke.sprite.getGlobalBounds();
        if(gs.existsNuke && nukeBounds.findIntersection(nukeshipBounds).has_value()) {
            gs.nukeship.lifes = 0;

            gs.nuke.sprite.setPosition(sf::Vector2f(0, -500));
            Explosion exp(3.0, gs.assets.explosion_texture, gs.nuke.sprite.getPosition()); 
            gs.explosions.push_back(exp);
        }
        
        if(gs.nukeship.lifes <= 0){
            if(gs.player.nukes < 1) gs.player.nukes++;
            gs.nukeship.lifes = 3;
            gs.existsNukeShip = false;
            gs.nukeSound.play();

            gs.player.score += 50;
        }
    }
}


//spostamento nemici
void moveEnemies(State& gs) {
    float secondsToElapse = std::clamp(gs.enemiesQuantity/60.0, 0.09, 0.8); //con clamp definisco lim min e max di tempo da contare, divido per 60 come il num iniziale di nemici
    if(gs.enemiesQuantity == 1)secondsToElapse = 0.04;

    if(gs.move_clock.getElapsedTime().asSeconds() >= secondsToElapse) {
        if(!gs.enemies.empty()) {
            float dist = std::clamp(3000.0 / gs.enemiesQuantity, 70.0, 80.0); //con clamp definisco lim min e max di tempo da contare
            bool edge = false;

            float minX = gs.enemies[0].sprite.getPosition().x; //trova estremi
            float maxX = minX;
            for(auto& enemy : gs.enemies) {
                float x = enemy.sprite.getPosition().x;
                if(x > maxX) maxX = x;
                if(x < minX) minX = x;
            }
            
            float windowWidth = static_cast<float>(gs.window.getSize().x);
            
            if((maxX + dist >= windowWidth - 170.0 && gs.right_dir) || (minX - dist <= 170.0 && !gs.right_dir)) edge = true;
            
            if(edge) {
                gs.right_dir = !gs.right_dir;
                for(auto& enemy : gs.enemies) {
                    enemy.sprite.move(sf::Vector2f(0.0,40.0)); //nemici scendono
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
void shootEnemyBullets(State& gs) { 
    for(auto& enemy : gs.enemies) {
        int shoot = rand() % 100;
        if(enemy.isFrontEnemy(enemy, gs.enemies)) {
            switch(enemy.type) {
                case Type1:
                    if(shoot < 1 && enemy.enemyBullet_clock.getElapsedTime().asSeconds() >= 3.0) { 
                        gs.enemyBullets.push_back(enemyBullet(gs.assets.enemyBullet_texture, enemy.sprite.getPosition()));
                        enemy.enemyBullet_clock.restart();
                    }
                break;

                case Type2:
                    if(shoot < 1 && enemy.enemyBullet_clock.getElapsedTime().asSeconds() >= 1.5) { 
                        gs.enemyBullets.push_back(enemyBullet(gs.assets.enemyBullet_texture, enemy.sprite.getPosition()));
                        enemy.enemyBullet_clock.restart();
                    }
                break;

                case Type3:
                    if(shoot < 1 && enemy.enemyBullet_clock.getElapsedTime().asSeconds() >= 0.25) { 
                        gs.enemyBullets.push_back(enemyBullet(gs.assets.enemyBullet_texture, enemy.sprite.getPosition()));
                        enemy.enemyBullet_clock.restart();
                    }
                break;
            }
        }
    }

    //scorrimento proiettili nemici
    for(auto& enemyBullet : gs.enemyBullets) {       
        enemyBullet.sprite.move(sf::Vector2f(0, enemyBullet.speed)); 
    }
}


//collisioni proiettili dei nemici
void updateEnemyBulletsCollisions(State& gs) {
    for(auto& enemyBullet : gs.enemyBullets) {
        sf::FloatRect enBulletsBounds = enemyBullet.sprite.getGlobalBounds();
        sf::FloatRect playerBounds = gs.player.sprite.getGlobalBounds();
        sf::FloatRect shieldBounds = gs.shield.sprite.getGlobalBounds();

        if(enBulletsBounds.findIntersection(playerBounds).has_value()) {
            if(gs.isShield) {
                gs.shield_sound.play();
            }
            else {
                gs.player.lifes--; 
                gs.playerExplosion_sound.play();
            }
            
            Explosion exp(0.5, gs.assets.explosion_texture, gs.player.sprite.getPosition());
            gs.explosions.push_back(exp);
            enemyBullet.pos.y = -500;
        }
    }

    eraseEnemyBullets(gs);
    expireExplosions(gs);
}


//per gestire gli scudi bonus (ausiliaria di updatePlayerBulletsCollisions)
void dropShieldCharger(State& gs, Enemy enemy) {
    float prob = rand() % 100;
    if(prob <= 3.0 && !gs.shieldChargerReleased) {
        gs.shieldChargerReleased = true;
        gs.shieldCharger.sprite.setPosition(enemy.sprite.getPosition());
    }
}


void pickShieldCharger(State& gs) {
    if(gs.shieldChargerReleased) {
        gs.shieldCharger.animate();
        gs.shieldCharger.sprite.move(sf::Vector2f(0, gs.shieldCharger.speed));

        sf::FloatRect shieldChargerBounds = gs.shieldCharger.sprite.getGlobalBounds();
        sf::FloatRect playerBounds = gs.player.sprite.getGlobalBounds();
        if(shieldChargerBounds.findIntersection(playerBounds).has_value()) {
            gs.player.shields++;
            
            gs.shieldChargerSound.play();
            gs.shieldCharger.sprite.setPosition(sf::Vector2f(0, -500));
            gs.shieldChargerReleased = false;
        }

        else if(gs.shieldCharger.sprite.getPosition().y >= sf::VideoMode::getDesktopMode().size.y) {
            gs.shieldChargerReleased = false;
        }
    }
}


void updateGameOver(State& gs) {
    bool lost = false;
    
    //se i nemici si avvicinano troppo
    if(!gs.enemies.empty()) {
        for(auto& enemy : gs.enemies) {
            float maxY = enemy.sprite.getPosition().y;
            if(maxY > sf::VideoMode::getDesktopMode().size.y * 0.7) lost = true;
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


void updateIngamePlayer(State& gs) {
    movePlayer(gs); //movimento
    shootPlayerBullets(gs); //spara
    enablePlayerShield(gs); //scudi
    shootPlayerNuke(gs); //colpo bonus
    updateNukeCollision(gs); //collisioni proiettile bonus
    pickShieldCharger(gs); //raccoglie scudi bonus
    updateEnemyBulletsCollisions(gs); //colpito da nemici
}

void updateIngameEnemies(State& gs) {
    moveEnemies(gs); //movimento
    shootEnemyBullets(gs); //sparano
    updatePlayerBulletsCollisions(gs); //colpiti da player
}

void updateIngameNukeship(State& gs) {
    spawnNukeship(gs); //movimento 
    updateNukeshipCollisions(gs); //colpita da player
}