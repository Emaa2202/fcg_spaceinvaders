#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> //per clamp che mi semplifica il movimento 
#include <vector>
#include <string>
//per rand
#include <ctime> 
#include <cstdlib>

#include "player.hpp" //player e sfondo
#include "enemy1.hpp"
#include "enemy2.hpp"
#include "enemy3.hpp"
#include "enemyBullet.hpp"
#include "explosion.hpp"
#include "font.hpp"


struct Start {
    sf::Font font;
    sf::Text title;
    sf::Text caption;
    sf::Clock effect_clock; //clock per effetto 

    Start() :
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

    void updateCaption() {
        if(static_cast<int>(effect_clock.getElapsedTime().asSeconds()) % 2 == 0) {
            caption.setFillColor(sf::Color::Transparent);
        }
        else {
            caption.setFillColor(sf::Color::White);
        }   
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(title);
        window.draw(caption);
    }
    
};


struct End {
    sf::Font font;
    sf::Text title;
    sf::Text caption;
    sf::Text finalScore;
    sf::Clock effect_clock; //clock per effetto 

    End() :
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

    void updateCaption() {
        if(static_cast<int>(effect_clock.getElapsedTime().asSeconds()) % 2 == 0) {
            caption.setFillColor(sf::Color::Transparent);
        }
        else {
            caption.setFillColor(sf::Color::White);
        }   
    }

    void update(int playerScore) {
        finalScore.setString("Punteggio: " + std::to_string(playerScore));
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(title);
        window.draw(caption);
        window.draw(finalScore);
    }
    
};


struct Ui {
    sf::Font font;
    sf::Text livesText;
    sf::Text scoreText;

    Ui() :
        livesText(font),
        scoreText(font) 
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
    }

    void update(int playerLifes, int playerScore) {
        livesText.setString("Vite: " + std::to_string(playerLifes));
        scoreText.setString("Punteggio: " + std::to_string(playerScore));
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(livesText);
        window.draw(scoreText);
    }
};


struct Explosion {
    sf::Sprite sprite;
    sf::Clock clock; //per far durare l'espolosione

    Explosion(const sf::Texture& texture) :
		sprite(texture)
	{
		float centro_x = static_cast<float>(texture.getSize().x) / 2.0;
        float centro_y = static_cast<float>(texture.getSize().y) / 2.0;
        sprite.setOrigin(sf::Vector2f(centro_x, centro_y));
	}
};


struct playerBullet {
	sf::Vector2f pos;
	float speed;
	sf::Sprite sprite;

	playerBullet(const sf::Texture& texture, sf::Vector2f pos_iniziale) :
		pos(pos_iniziale),
		speed(45.0),
		sprite(texture)
	{
		float centro_x = static_cast<float>(texture.getSize().x) / 2.0;
        float centro_y = static_cast<float>(texture.getSize().y) / 2.0;
        sprite.setOrigin(sf::Vector2f(centro_x, centro_y));
		
		sprite.setPosition(pos);
	}
};


struct enemyBullet {
	sf::Vector2f pos;
	float speed;
	sf::Sprite sprite;

	enemyBullet(const sf::Texture& texture, sf::Vector2f pos_iniziale) :
		pos(pos_iniziale),
		speed(20.0),
		sprite(texture)
	{
		float centro_x = static_cast<float>(texture.getSize().x) / 2.0;
        float centro_y = static_cast<float>(texture.getSize().y) / 2.0;
        sprite.setOrigin(sf::Vector2f(centro_x, centro_y));
		
		sprite.setPosition(pos);
	}
};

enum enemyType {
    Type1,
    Type2,
    Type3
};

struct Enemy {
    enemyType type;
    sf::Sprite sprite;
    bool isAlive;
    sf::Clock enemyBullet_clock; //spostato qui per farli sparare anche assieme 
    int col; //colonna per capire se puo sparare
    int points;

    //animazione
    float frameWidth;
    float frameHeight;
    int currentFrame = 0;
    sf::Clock cornometro_animaz;
    float sec_per_frame = 0.8;

    Enemy(const sf::Texture& texture, enemyType init_type) :
        sprite(texture),
        type(init_type),
        isAlive(true)
    {
        frameWidth = texture.getSize().x / 2.0; //2 frame, dim divise per 2
        frameHeight = texture.getSize().y;

        sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight})); //sprite predefinito, y sempre 0 perchè uso hpp 

        float centro_x = static_cast<float>(frameWidth) / 2; //centro calcolato su singolo frame
        float centro_y = static_cast<float>(frameHeight) / 2;
        sprite.setOrigin(sf::Vector2f(centro_x, centro_y));
    }

    void animate() {
        if(cornometro_animaz.getElapsedTime().asSeconds() >= sec_per_frame) {
            currentFrame = 1 - currentFrame; //alterna i frame

            //currentFrame = 0 ---> X = 0
            //currentFrame = 1 ---> X = frameWidth
            int rectX = currentFrame * frameWidth;
            sprite.setTextureRect(sf::IntRect({rectX, 0}, {frameWidth, frameHeight})); //cambio sprite

            cornometro_animaz.restart();
        }
    }

    bool isFrontEnemy(const Enemy target, const std::vector<Enemy>& enemies) {
        float targetY = target.sprite.getPosition().y;

        for(const auto& enemy : enemies) {
            if(enemy.isAlive && enemy.col == target.col && enemy.sprite.getPosition().y > targetY) return false;
        }

        return true;
    }

};


/*----------------------
-------Game State-------
----------------------*/
struct State {
    //risorse generali
    sf::RenderWindow window;

    sf::Texture background;
    sf::Sprite background_sprite;
    
	sf::Texture player;
    sf::Sprite player_sprite;
	sf::Vector2f playerpos;

	sf::Texture playerBullet_texture;
	std::vector<playerBullet> playerBullets;
    sf::Clock playerBullet_clock; //serve per tenere traccia del cooldown

    std::vector<Enemy> enemies;
    int rows = 6;
    int columns = 12;
    sf::Texture enemy1_texture;
    sf::Texture enemy3_texture;
    sf::Texture enemy2_texture;

    sf::Clock move_clock; //per spostamento nemici
    bool right_dir = true; //direzione nemici, prima era sotto ma mi serve persistente

    sf::Texture enemyBullet_texture;
    std::vector<enemyBullet> enemyBullets;
    
    int playerLifes = 3;

    sf::Texture explosion_texture;
    sf::Sprite explosion_sprite;
    std::vector<Explosion> explosions;

    bool gameOver = false;
    bool startScreen = true;

    Ui ui;
    Start start;
    End end;

    int playerScore = 0;

    //posizionamento player
    void initPlayer() {
        //sposta origine di player al centro dello sprite
        float player_centro_x = static_cast<float>(player.getSize().x) / 2.0;
        float player_centro_y = static_cast<float>(player.getSize().y) / 2.0;
        player_sprite.setOrigin(sf::Vector2f(player_centro_x, player_centro_y));

        player_sprite.setScale(sf::Vector2f(0.3, 0.4));
        player_sprite.setPosition(sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().size.x) / 2.0, static_cast<float>(sf::VideoMode::getDesktopMode().size.y) * 0.8)); 

		//posizione player
		playerpos = player_sprite.getPosition();
    }
    
    //posizionamento nemici
    void initEnemies() {
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
                    Enemy en(enemy1_texture, Type1);
                    en.points = 10;
                    en.col = j;
                    en.sprite.setPosition(sf::Vector2f(posX, posY));
                    en.sprite.setScale(sf::Vector2f(0.6, 0.6));
                    enemies.push_back(en);
                } 
                else if(i == 2 || i == 3){
                    Enemy en(enemy2_texture, Type2);
                    en.points = 15;
                    en.col = j;
                    en.sprite.setPosition(sf::Vector2f(posX, posY));
                    en.sprite.setScale(sf::Vector2f(1.0, 1.0));
                    enemies.push_back(en);
                }
                else {
                    Enemy en(enemy3_texture, Type3);
                    en.points = 20;
                    en.col = j;
                    en.sprite.setPosition(sf::Vector2f(posX, posY));
                    en.sprite.setScale(sf::Vector2f(0.8, 0.8));
                    enemies.push_back(en);
                }
            }
        }
    }

    
    State() :
        //caricamento texture e collegamento agli sprite
        background(spacebackground_jpg, spacebackground_jpg_len),
        background_sprite(background),
        
		player(player_png, player_png_len),
        player_sprite(player),
		
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

        initPlayer();
        initEnemies();        
        
    }
};


/*-----------------------------------------
-----Funzioni callback gestione eventi-----
-----------------------------------------*/
void handle(const sf::Event::Closed &, State &gs) {
    gs.window.close();
}

template <typename T>
void handle(const T &, State &gs) { //eventi non gestiti esplicitamente
    
}

//resetta tutto e riporta gameOver a false, facendo ripartire l update
void handle(const sf::Event::KeyPressed &event, State &gs) {
    if(gs.startScreen && event.code == sf::Keyboard::Key::Enter) { //inizia la partita
        gs.startScreen = false;
        gs.playerScore = 0;
    }
    
    if(gs.gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter)) { //riavvia
        gs.playerScore = 0;

        gs.gameOver = false;
        gs.playerLifes = 3;
        
        gs.playerBullets.clear();
        gs.enemyBullets.clear();
        gs.enemies.clear();
        gs.explosions.clear();
        
        gs.initPlayer();
        gs.initEnemies();
    }
}


/*-----------------------------
-------------Update------------
------------------------------*/
void updatePlayer(State&gs) {
    int speed = 10; //controllando a ogni frame (non piu handle) va diminuita la velocita 

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) { //isKeyPressed invece di keyPressed per controllo tempo reale, permette di muoversi e sparare insieme
	    gs.playerpos.x -= speed;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		gs.playerpos.x += speed;
	}

    float half_width = (gs.player.getSize().x * gs.player_sprite.getScale().x) / 2.0; //calcolo larghezza/2 dello sprite per non farlo fuoriuscire di bordi
	float min_x = half_width; //mezzo sprite (sx)
    float max_x = static_cast<float>(gs.window.getSize().x) - half_width; //x schermo - mezzo sprite (dx)

    //blocca playerpos.x tra min_x e max_x
    gs.playerpos.x = std::clamp(gs.playerpos.x, min_x, max_x);

	gs.player_sprite.setPosition(gs.playerpos);
}

void updateplayerBullets(State& gs) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        if(gs.playerBullet_clock.getElapsedTime().asSeconds() >= 0.45) {
            gs.playerBullets.push_back(playerBullet(gs.playerBullet_texture, gs.playerpos));
            gs.playerBullet_clock.restart();
        }
    }   
    //scorrimento proiettili player
    for(auto& playerBullet : gs.playerBullets) {
        playerBullet.pos.y -= playerBullet.speed;        
        playerBullet.sprite.setPosition(playerBullet.pos); 
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
                    gs.playerScore += enemy.points;

                    Explosion exp(gs.explosion_texture);
                    exp.sprite.setPosition(enemy.sprite.getPosition());
                    exp.sprite.setScale(sf::Vector2f(0.5, 0.5));
                    gs.explosions.push_back(exp);

                    playerBullet.pos.y = -500;
                    break;
                }
            }
            
        }
    
    }
    
    for(int i = gs.enemies.size()-1; i >= 0; i--) {
        if(!gs.enemies[i].isAlive) gs.enemies.erase(gs.enemies.begin() + i);
    }

    for(int i = gs.playerBullets.size() -1; i >= 0; i--) {
        if(gs.playerBullets[i].pos.y < 0.0) gs.playerBullets.erase(gs.playerBullets.begin() + i);
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
        sf::FloatRect playerBounds = gs.player_sprite.getGlobalBounds();
        
        if(enBulletsBounds.findIntersection(playerBounds).has_value()) {
            gs.playerLifes--;
            
            Explosion exp(gs.explosion_texture);
            exp.sprite.setPosition(gs.player_sprite.getPosition());
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
    //se i nemici si avvicinano troppo
    if(!gs.enemies.empty()) {
        for(auto& enemy : gs.enemies) {
            float maxY = enemy.sprite.getPosition().y;
            if(maxY > sf::VideoMode::getDesktopMode().size.y * 0.6) gs.gameOver = true;
        }
    }
    
    if(gs.playerLifes < 0){
        //via effetto esplosione e proiettili
        gs.explosions.clear();
        gs.enemyBullets.clear();
        gs.playerBullets.clear();
        gs.gameOver = true;
    }
}

void updateLevel(State& gs) {
    if(gs.enemies.empty()) {
        gs.playerLifes++;
        gs.initPlayer();
        gs.initEnemies();

        //pulisco tutto 
        gs.playerBullets.clear();
        gs.enemyBullets.clear();
        gs.explosions.clear();
        
        //reimposta la dir nemici a destra
        gs.right_dir = true;
    }
}

void update(State& gs) {
    if(gs.startScreen) {
        gs.start.updateCaption();
        return;
    }
    else if(gs.gameOver) {
        gs.end.update(gs.playerScore);
        gs.end.updateCaption();
        return;
    }

    updatePlayer(gs);
    updateplayerBullets(gs);
    updateEnemies(gs);
    updateEnemyBullets(gs);
    updatePlayerBulletsCollisions(gs);
    updateEnemyBulletsCollisions(gs);
    gs.ui.update(gs.playerLifes, gs.playerScore);
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

        //nemici
	    for (const auto& enemy : gs.enemies) {
            gs.window.draw(enemy.sprite);
        }

	    //proiettili giocatore
	    for (const auto& playerBullet : gs.playerBullets) {
            gs.window.draw(playerBullet.sprite);
        }

        //giocatore
        gs.window.draw(gs.player_sprite);

        //esplosioni
        for (const auto& exp : gs.explosions) {
            gs.window.draw(exp.sprite);
        }

        gs.ui.draw(gs.window);
    }
    gs.window.display();
}


/*--------------
---Main loop----
--------------*/
int main() {
    State gs;
    srand(time(NULL));

    while (gs.window.isOpen()) {
        
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        update(gs);

        doGraphics(gs);
    }
}