#include <SFML/Graphics.hpp>
#include <iostream>

#include "state.hpp"


/*-----------------------------------------
-----Funzioni callback gestione eventi-----
-----------------------------------------*/
void handle(const sf::Event::Closed &, State &gs) {
    gs.window.close();
}

template <typename T>
void handle(const T &, State &gs) { //eventi non gestiti esplicitamente
    
}


//movimento mouse pausa
void handle(const sf::Event::MouseMoved &event, State &gs) {
    sf::Vector2f mousePos = gs.window.mapPixelToCoords(sf::Vector2i(event.position.x, event.position.y));
    gs.pause.mouse(mousePos);
}

//click mouse pausa
void handle(const sf::Event::MouseButtonPressed &event, State &gs) {
    sf::Vector2f mousePos = gs.window.mapPixelToCoords(sf::Vector2i(event.position.x, event.position.y));
    if(gs.isPaused && event.button == sf::Mouse::Button::Left){
        if(gs.pause.captions[gs.pause.selectedCaptionIndex].getGlobalBounds().contains(mousePos)) {
            if(gs.pause.selectedCaptionIndex == 0) { 
                gs.isPaused = false;
                gs.soundtrack.play();
            } 
            else if (gs.pause.selectedCaptionIndex == 1) gs.window.close();
        }
    }
}


//resetta tutto e riporta gameOver a false, facendo ripartire l update
void handle(const sf::Event::KeyPressed &event, State &gs) {
    if(gs.startScreen && event.code == sf::Keyboard::Key::Enter) { //inizia la partita
        gs.startScreen = false;
        gs.player.score = 0;
    }

    if(!gs.startScreen && !gs.gameOver) {
        if(event.code == sf::Keyboard::Key::Escape) {
            gs.isPaused = true;
            gs.soundtrack.pause();
        }
    }

    if(gs.isPaused) { //navigazione pausa con tastiera
        if(event.code == sf::Keyboard::Key::Up) {
            gs.pause.up();
        }
        if(event.code == sf::Keyboard::Key::Down) gs.pause.down();
        if(event.code == sf::Keyboard::Key::Enter) {
            if(gs.pause.selectedCaptionIndex == 0) { //riprendi
                gs.isPaused = false;
                gs.soundtrack.play();
            }
            else gs.window.close();
        }
    }

    if(gs.gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter)) { //riavvia
        gs.player.resetAll();

        gs.gameOver = false;
        gs.gameoverTransition = false;

        gs.playMusic("soundtrack.mp3"); //riproduce audio con var soundtrack

        gs.isShield = false;
        gs.shieldChargerReleased = false;
        gs.existsNuke = false;
        gs.existsNukeShip = false;

        gs.playerBullets.clear();
        gs.enemyBullets.clear();
        gs.enemies.clear();
        gs.explosions.clear();
        
        gs.initEnemies();
    }
}


/*-----------------------------
-------------Update------------
------------------------------*/
void update(State& gs) {
    if(gs.startScreen) {
        gs.start.updateCaption();
        return;
    }
    else if(gs.gameOver) {
        gs.end.update(gs.player.score);
        gs.end.updateCaption();
        return;
    }

    if(gs.isPaused) {
        return;
    }

    if(gs.gameoverTransition) {
        if(gs.gameoverTransition_clock.getElapsedTime().asSeconds() >= 0.5) {
            gs.gameoverTransition = false; //tolgo la transizione, metto true a gameOver e appare la schermata
            gs.gameOver = true; 
            
            gs.soundtrack.stop();

            gs.explosions.clear();
            gs.enemyBullets.clear();
            gs.playerBullets.clear();
        }
        return; 
    }

    if(gs.nextLevelTransition) {
        if(gs.nextLevelTransition_clock.getElapsedTime().asSeconds() >= 0.5) {
            gs.nextLevelTransition = false;

            //pulisco tutto, aggiungo 1 vita e incremento contatore livelli
            gs.player.level++;
            gs.player.lifes++;
            gs.player.shields++;
            
            gs.isShield = false;
            gs.shieldChargerReleased = false;
            gs.existsNuke = false;
            gs.existsNukeShip = false;

            gs.player.resetPosition();
            gs.initEnemies();
 
            gs.playerBullets.clear();
            gs.enemyBullets.clear();
            gs.explosions.clear();
            
            //reimposta la dir nemici a destra
            gs.right_dir = true;

            gs.nextLevelTransition_clock.restart();
        }
        return;
    }

    updatePlayer(gs);
    updateplayerBullets(gs);
    updateShield(gs);
    updateShieldCharger(gs);
    updatePlayerNuke(gs);
    updateNukeCollision(gs);
    updateNukeship(gs);
    updateNukeshipCollisions(gs);
    updateEnemies(gs);
    updateEnemyBullets(gs);
    updatePlayerBulletsCollisions(gs);
    updateEnemyBulletsCollisions(gs);
    if(gs.player.lifes < 0) gs.ui.update(0, gs.player.score, gs.player.level, gs.player.shields); //per nascondere il -1 vite al gameOver
    else gs.ui.update(gs.player.lifes, gs.player.score, gs.player.level, gs.player.shields);
    updateLevel(gs);
    updateGameOver(gs);
}


/*--------------
------Grafica---
--------------*/
void doGraphics(State &gs) {
    //sfondo
    gs.window.clear();
    gs.window.draw(gs.background_sprite);
    
    if(gs.startScreen) {
        gs.start.draw(gs.window);
    }
    else if(gs.gameOver) {
        gs.end.draw(gs.window);
    }
    else {
        //proiettili nemici
	    for (const auto& enemyBullet : gs.enemyBullets) {
            gs.window.draw(enemyBullet.sprite);
        }

        if(gs.shieldChargerReleased) gs.window.draw(gs.shieldCharger.sprite);

        //nemici
	    for (const auto& enemy : gs.enemies) {
            gs.window.draw(enemy.sprite);
        }

	    //proiettili giocatore
	    for (const auto& playerBullet : gs.playerBullets) {
            gs.window.draw(playerBullet.sprite);
        }

        //giocatore
        gs.window.draw(gs.player.sprite);
        
        //scudo
        if(gs.isShield) gs.window.draw(gs.shield.sprite);

        //nuke
        if(gs.existsNuke) gs.window.draw(gs.nuke.sprite);
        if(gs.existsNukeShip) gs.window.draw(gs.nukeship.sprite);

        //esplosioni
        for (const auto& exp : gs.explosions) {
            gs.window.draw(exp.sprite);
        }

        if(gs.isPaused) {
            gs.pause.draw(gs.window);
        }

        gs.ui.draw(gs.window);
    }
    gs.window.display();
}


/*--------------
---Main loop----
--------------*/
int main(int argc, char* argv[]) {
    std::string audioDir = "./media/audio"; //se non scrivo da terminale, questo è default
    if (argc >= 2) audioDir = argv[1];
    
    State gs;
    srand(time(NULL));
    gs.audioDir = audioDir; //passo a state la cartella per cercare audio e usare la funzione playMusic

    gs.playMusic("soundtrack.mp3"); //riproduce audio con var soundtrack
    while (gs.window.isOpen()) {
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        update(gs);
        doGraphics(gs);
    }
}