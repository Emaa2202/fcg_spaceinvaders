#include <SFML/Graphics.hpp>
#include <algorithm> //per clamp che mi semplifica il movimento 
#include <vector>
#include "textures.hpp"
#include "enemy1.hpp"
#include "enemy2.hpp"
#include "enemy3.hpp"

struct Bullet {
	sf::Vector2f pos;
	float speed;
	sf::Sprite sprite;

	Bullet(const sf::Texture& texture, sf::Vector2f pos_iniziale) :
		pos(pos_iniziale),
		speed(15.0),
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

    //animazione
    int frameWidth;
    int frameHeight;
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

	sf::Texture bullet_texture;
	std::vector<Bullet> bullets;
    sf::Clock bullet_clock; //serve per tenere traccia del cooldown

    std::vector<Enemy> enemies;
    int rows = 3;
    int columns = 10;
    float distX = 300.0; //distanze tra un nemico e l altro
    float distY = 200.0;
    sf::Texture enemy1_texture;
    sf::Texture enemy3_texture;
    sf::Texture enemy2_texture;

    sf::Clock move_clock; //per spostamento nemici
    bool right_dir = true; //direzione nemici, prima era sotto ma mi serve persistente

    //caricamento texture e collegamento agli sprite prima del corpo del costruttore
    State() :
        background(spacebackground_jpg, spacebackground_jpg_len),
        background_sprite(background),
        
		player(player_png, player_png_len),
        player_sprite(player),
		
		bullet_texture(bullet_png, bullet_png_len),

        enemy1_texture(enemy1_sheet_png, enemy1_sheet_png_len),
        enemy2_texture(enemy2_sheet_png, enemy2_sheet_png_len),
        enemy3_texture(enemy3_sheet_png, enemy3_sheet_png_len)
    {
        //creazione finestra
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode({desktop.size.x, desktop.size.y}), "Space Invaders");
        window.setFramerateLimit(60);
        
        //sfondo di dimensione dello schermo
        double background_scale_x = (static_cast<float>(desktop.size.x) / background.getSize().x); 
        double background_scale_y = (static_cast<float>(desktop.size.y) / background.getSize().y);
        background_sprite.setScale(sf::Vector2f(background_scale_x, background_scale_y));

        //sposta origine di player al centro dello sprite
        float player_centro_x = static_cast<float>(player.getSize().x) / 2.0;
        float player_centro_y = static_cast<float>(player.getSize().y) / 2.0;
        player_sprite.setOrigin(sf::Vector2f(player_centro_x, player_centro_y));

        player_sprite.setScale(sf::Vector2f(0.5, 0.5));
        player_sprite.setPosition(sf::Vector2f(static_cast<float>(desktop.size.x) / 2.0, static_cast<float>(desktop.size.y) * 0.8)); 

		//posizione player
		playerpos = player_sprite.getPosition();

        //posizione nemici
        float screenWidth = static_cast<float>(desktop.size.x);
        float screenHeight = static_cast<float>(desktop.size.y);

        float distX = 300; //distanze tra nemici
        float distY = 200;

        float gridWidth = (columns - 1) * distX; //dimensioni griglia
        float gridHeight = (rows - 1) * distY;

        float startX = (screenWidth - gridWidth) / 2; //posizionamento effettivo griglia
        float startY = screenHeight * 0.15;
        
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                float posX = startX + (j * distX);
                float posY = startY + (i * distY);
            
                if(i == 2){
                    Enemy en(enemy1_texture, Type1);
                    en.sprite.setPosition(sf::Vector2f(posX, posY));
                    en.sprite.setScale(sf::Vector2f(0.8, 0.8));
                    enemies.push_back(en);
                } 
                else if(i == 1){
                    Enemy en(enemy2_texture, Type2);
                    en.sprite.setPosition(sf::Vector2f(posX, posY));
                    en.sprite.setScale(sf::Vector2f(1.2, 1.2));
                    enemies.push_back(en);
                }
                else {
                    Enemy en(enemy3_texture, Type3);
                    en.sprite.setPosition(sf::Vector2f(posX, posY));
                    enemies.push_back(en);
                }
            }
        }
        
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


/*-----------------------------
------Logica base del gioco----
------------------------------*/
void update(State& gs) {
    int speed = 10; //controllando a ogni frame (non piu handle) va diminuita la velocita 

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) { //isKeyPressed invece di keyPressed per controllo tempo reale, permette di muoversi e sparare insieme
	    gs.playerpos.x -= speed;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		gs.playerpos.x += speed;
	}

    float half_width = (gs.player.getSize().x * gs.player_sprite.getScale().x) / 2.0f; //calcolo larghezza/2 dello sprite per non farlo fuoriuscire
	float min_x = half_width; //mezzo sprite (sx)
    float max_x = static_cast<float>(gs.window.getSize().x) - half_width; //x schermo - mezzo sprite (dx)

    //blocca playerpos.x tra min_x e max_x
    gs.playerpos.x = std::clamp(gs.playerpos.x, min_x, max_x);

	gs.player_sprite.setPosition(gs.playerpos);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
	    if(gs.bullet_clock.getElapsedTime().asSeconds() >= 0.45) {
            gs.bullets.push_back(Bullet(gs.bullet_texture, gs.playerpos));
            gs.bullet_clock.restart();
        }
    }

    //scorrimento proiettili player
    for(auto& bullet : gs.bullets) {
        bullet.pos.y -= bullet.speed;        
        bullet.sprite.setPosition(bullet.pos); 
    }

    //spostamento nemici
    if(gs.move_clock.getElapsedTime().asSeconds() >= 1.0) {
        if(!gs.enemies.empty()) {
            float dist = 100;
            bool edge = false;

            float minX = gs.enemies[0].sprite.getPosition().x; //trova estremi
            float maxX = minX;
            for(auto& enemy : gs.enemies) {
                float x = enemy.sprite.getPosition().x;
                if(x > maxX) maxX = x;
                if(x < minX) minX = x;
            }
            
            float windowWidth = static_cast<float>(gs.window.getSize().x);
            
            if((maxX + dist >= windowWidth && gs.right_dir) || (minX - dist <= 0 && !gs.right_dir)) {
                edge = true;
            }

            if(edge) {
                gs.right_dir = !gs.right_dir;
                for (auto& enemy : gs.enemies) {
                    enemy.sprite.move(sf::Vector2f(0.0, 30.0)); //nemici scendono
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


/*--------------
------Grafica---
--------------*/
void doGraphics(State &gs) {
    //sfondo
    gs.window.clear();
    gs.window.draw(gs.background_sprite);

    //nemici
	for (const auto& enemy : gs.enemies) {
        gs.window.draw(enemy.sprite);
    }

	//proiettili giocatore
	for (const auto& bullet : gs.bullets) {
        gs.window.draw(bullet.sprite);
    }

    //giocatore
    gs.window.draw(gs.player_sprite);

    gs.window.display();
}


/*--------------
---Main loop----
--------------*/
int main() {
    State gs;
    
    while (gs.window.isOpen()) {
        
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        update(gs);

        doGraphics(gs);
    }
}