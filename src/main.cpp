#include <SFML/Graphics.hpp>
#include "textures.hpp"

int main()
{
	//recupero desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode({desktop.size.x, desktop.size.y}), "Space Invaders");
	window.setFramerateLimit(60);
	
	//caricamento sfondo da file hpp e dimensione corretta
	sf::Texture background(spacebackground_jpg, spacebackground_jpg_len);
	sf::Sprite background_sprite(background); //serve per renderlo drawable
	
	double background_scale_x = (static_cast<float>(desktop.size.x) / background.getSize().x); //facendo cast di 1 ottengo risultato float e non int
	double background_scale_y = (static_cast<float>(desktop.size.y) / background.getSize().y);
	background_sprite.setScale(sf::Vector2f(background_scale_x, background_scale_y));
	
	//bozza statica del giocatore
	sf::Texture player(player_png, player_png_len);
	sf::Sprite player_sprite(player); //serve per renderlo drawable
	//player_sprite.setPosition(sf::Vector2f(desktop.size.x / 2, desktop.size.y * 0.8)); 

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		//sfondo
		window.clear();
		window.draw(background_sprite);

		//giocatore
		window.draw(player_sprite);

		window.display();
	}
}
