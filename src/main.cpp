#include <SFML/Graphics.hpp>
#include <algorithm> //per clamp che mi semplifica il movimento 
#include "textures.hpp"


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

    //caricamento texture e collegamento agli sprite prima del corpo del costruttore
    State() :
        background(spacebackground_jpg, spacebackground_jpg_len),
        background_sprite(background),
        player(player_png, player_png_len),
        player_sprite(player)
    {
        //recupero desktop e creazione finestra
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
    }
};

/*-----------------------------------------
-----Funzioni callback gestione eventi-----
-----------------------------------------*/
void handle(const sf::Event::Closed &, State &gs) {
    gs.window.close();
}


void handle(const sf::Event::KeyPressed &keyPressed, State &gs) {
	int speed = 30;
	float half_width = (gs.player.getSize().x * gs.player_sprite.getScale().x) / 2.0f; //calcolo larghezza/2 dello sprite per non farlo fuoriuscire

	if(keyPressed.scancode == sf::Keyboard::Scancode::Left) {
	    gs.playerpos.x -= speed;
	}
	else if(keyPressed.scancode == sf::Keyboard::Scancode::Right) {
		gs.playerpos.x += speed;
	}

	float min_x = half_width; //mezzo sprite (sx)
    float max_x = static_cast<float>(gs.window.getSize().x) - half_width; //x schermo - mezzo sprite (dx)

    //blocca playerpos.x tra min_x e max_x
    gs.playerpos.x = std::clamp(gs.playerpos.x, min_x, max_x);

	gs.player_sprite.setPosition(gs.playerpos);
}

		
template <typename T>
void handle(const T &, State &gs) { //eventi non gestiti esplicitamente
    
}

/*--------------
------Grafica---
--------------*/
void doGraphics(State &gs) {
    //sfondo
    gs.window.clear();
    gs.window.draw(gs.background_sprite);

    //giocatore
    gs.window.draw(gs.player_sprite);

    gs.window.display();
}

/*--------------
---Main loop----
--------------*/
int main() {
    State gs;
    
    while (gs.window.isOpen()) 
    {
        
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        doGraphics(gs);
    }
}