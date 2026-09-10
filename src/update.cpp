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


/*-----------------------------
-------------Update------------
------------------------------*/
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


//nemici colpiti da giocatore
void updatePlayerBulletsCollisions(State& gs) { 
    for(auto& playerBullet : gs.playerBullets) {
        sf::FloatRect playerBulletBounds = playerBullet.sprite.getGlobalBounds();
        
        for(auto& enemy : gs.enemies) {
            if(enemy.isAlive) {
                sf::FloatRect enemyBounds = enemy.sprite.getGlobalBounds();
                
                if(playerBulletBounds.findIntersection(enemyBounds).has_value()) {
                    gs.floatingTexts.push_back(FloatingText(gs.start.font, "+" + std::to_string(enemy.points), enemy.sprite.getPosition()));
                    
                    gs.shieldCharger.drop(enemy);
                    enemy.isAlive = false;
                    gs.player.score += enemy.points;

                    Explosion exp(0.5, gs.assets.explosion_texture, enemy.sprite.getPosition());
                    gs.explosions.push_back(exp);

                    playerBullet.sprite.setPosition(sf::Vector2f(0, -500));
                    break;
                }
            }
            
        }
    
    }
    
    gs.eraseEnemies();
    gs.erasePlayerBullets();
    gs.expireExplosions();
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
                    gs.floatingTexts.push_back(FloatingText(gs.start.font, "+" + std::to_string(enemy.points), enemy.sprite.getPosition()));
                    gs.shieldCharger.drop(enemy);                    
                    enemy.isAlive = false;
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
    
    gs.eraseEnemies();
    gs.expireExplosions();
}


void updateBonusShipCollisions(State& gs) { //nave colpita
    if(gs.bonusship.exists) {
        sf::FloatRect bonusShipBounds = gs.bonusship.sprite.getGlobalBounds();
        for(auto& bullet : gs.playerBullets) {
            sf::FloatRect playerBulletBounds = bullet.sprite.getGlobalBounds();
            
            if(playerBulletBounds.findIntersection(bonusShipBounds).has_value()) {
                gs.bonusship.lifes--;
                
                Explosion exp(0.5, gs.assets.explosion_texture, gs.bonusship.sprite.getPosition());
                if(gs.bonusship.lifes == 0) exp.sprite.setScale(sf::Vector2f(3.0, 3.0));
                bullet.sprite.setPosition(sf::Vector2f(0, -500));
                gs.explosions.push_back(exp);
            }

        }

        sf::FloatRect nukeBounds = gs.nuke.sprite.getGlobalBounds();
        if(gs.existsNuke && nukeBounds.findIntersection(bonusShipBounds).has_value()) {
            gs.bonusship.lifes = 0;

            Explosion exp(3.0, gs.assets.explosion_texture, gs.nuke.sprite.getPosition()); 
            gs.nuke.sprite.setPosition(sf::Vector2f(0, -500));
            gs.explosions.push_back(exp);
        }
        
        if(gs.bonusship.lifes <= 0){
            if(gs.player.nukes < 1) gs.player.nukes++;
            gs.bonusship.lifes = 3;
            gs.bonusship.exists = false;
            gs.nukeSound.play();
            
            gs.floatingTexts.push_back(FloatingText(gs.start.font, "NUKE +1 \n+" + std::to_string(50), gs.bonusship.sprite.getPosition() + sf::Vector2f(0.0, +80.0)));
            gs.player.score += 50;
        }
    }
}


//spostamento nemici
//fix: il mio schermo è 3840x2160 cambio tutto in dim relative
void moveEnemies(State& gs) {           
    for(auto& enemy : gs.enemies) {
        if(enemy.type == Type1 || enemy.type == Type2) enemy.animate();
    }
    
    float secondsToElapse = std::clamp(gs.enemies.size()/60.0, 0.09, 1.0); //con clamp definisco lim min e max di tempo da contare, divido per 60 come il num iniziale di nemici
    if(gs.enemies.size() == 1)secondsToElapse = 0.04; 

    if(gs.move_clock.getElapsedTime().asSeconds() >= secondsToElapse) {
        if(!gs.enemies.empty()) {
            float dist = std::clamp(gs.windowWidth * 0.78 / gs.enemies.size(), gs.windowWidth * 0.018, gs.windowWidth * 0.021); //con clamp definisco lim min e max di distanza da contare
            bool edge = false;

            float minX = gs.enemies[0].sprite.getPosition().x; //trova estremi
            float maxX = minX;
            for(auto& enemy : gs.enemies) {
                float x = enemy.sprite.getPosition().x;
                if(x > maxX) maxX = x;
                if(x < minX) minX = x;
            }
            
            if((maxX + dist >= gs.windowWidth - (gs.windowWidth * 0.044) && gs.right_dir) || (minX - dist <= (gs.windowWidth * 0.044) && !gs.right_dir)) edge = true;
            
            if(edge) {
                gs.right_dir = !gs.right_dir;
                for(auto& enemy : gs.enemies) {
                    if(enemy.type == Type3) enemy.animate(); //sprite animaz
                    enemy.sprite.move(sf::Vector2f(0.0, gs.windowHeigth * 0.019)); //nemici scendono
                }
            }
            else {
                for(auto& enemy : gs.enemies) {
                    if(gs.right_dir) enemy.sprite.move(sf::Vector2f(dist, 0.0));
                    else enemy.sprite.move(sf::Vector2f(-dist, 0.0));
                    if(enemy.type == Type3) enemy.animate(); //sprite animaz
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

    gs.eraseEnemyBullets();
    gs.expireExplosions();
}


void pickShieldCharger(State& gs) {
    if(gs.shieldCharger.isReleased) {
        gs.shieldCharger.animate();
        gs.shieldCharger.sprite.move(sf::Vector2f(0, gs.shieldCharger.speed));

        sf::FloatRect shieldChargerBounds = gs.shieldCharger.sprite.getGlobalBounds();
        sf::FloatRect playerBounds = gs.player.sprite.getGlobalBounds();
        if(shieldChargerBounds.findIntersection(playerBounds).has_value()) {
            gs.player.shields++;
            
            gs.shieldChargerSound.play();
            gs.shieldCharger.sprite.setPosition(sf::Vector2f(0, -500));
            gs.shieldCharger.isReleased = false;
        }

        else if(gs.shieldCharger.sprite.getPosition().y >= sf::VideoMode::getDesktopMode().size.y) {
            gs.shieldCharger.isReleased = false;
        }
    }
}


void updateIngamePlayer(State& gs) {
    gs.player.move();
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

void updateIngameBonusShip(State& gs) {
    gs.bonusship.spawn(); //movimento 
    updateBonusShipCollisions(gs); //colpita da player
}


bool updateTransitions(State& gs) {
    //gameover
    if(gs.gameoverTransition) {
        if(gs.gameoverTransition_clock.getElapsedTime().asSeconds() >= 0.5) {
            gs.gameoverTransition = false;
            gs.gameOver = true; 
            gs.soundtrack.stop();
            gs.explosions.clear();
            gs.enemyBullets.clear();
            gs.playerBullets.clear();
            
            //aggiorna qui schermata di gameover per evitare compaia durante la transizione
            gs.end.update(gs.player.score); //basta aggiornarlo al gameover e non di continuo
            gs.end.updateCaption();
        }
        return true; //con true blocca gameplay
    }

    //prossimo livello
    if(gs.nextLevelTransition) {
        if(gs.nextLevelTransition_clock.getElapsedTime().asSeconds() >= 0.5) {
            gs.nextLevelTransition = false;

            gs.player.level++;
            gs.player.lifes++;
            gs.player.shields++;
            
            gs.playerBullets.clear();
            gs.enemyBullets.clear();
            gs.explosions.clear();

            gs.isShield = false;
            gs.shieldCharger.isReleased = false;
            gs.existsNuke = false;
            gs.bonusship.exists = false;

            gs.player.resetPosition();
            gs.initEnemies();
            gs.right_dir = true; 
            
            gs.move_clock.restart(); //resetta clock velocita enemies
        }
        return true; 
    }

    //controlla sconfitta
    bool lost = false;
    if(gs.player.lifes < 0) {
        lost = true;
    } 
    else if(!gs.enemies.empty()) {
        for(auto& enemy : gs.enemies) {
            if(enemy.sprite.getPosition().y > sf::VideoMode::getDesktopMode().size.y * 0.7) {
                lost = true;
                break;
            }
        }
    }

    if(lost) {
        gs.gameoverTransition = true;
        gs.gameoverTransition_clock.restart();
        return true; 
    }

    //controlla vittoria
    if(gs.enemies.empty()) {
        gs.nextLevelTransition = true;
        gs.nextLevelTransition_clock.restart();
        return true;
    }

    //nessuna transizione gioco continua normalmente
    return false; 
}


void updateFloatingTexts(State& gs) {
    for(int i = gs.floatingTexts.size() - 1; i >= 0; i--) {
        gs.floatingTexts[i].animate();
        if(gs.floatingTexts[i].expired()) {
            gs.floatingTexts.erase(gs.floatingTexts.begin() + i);
        }
    }
}