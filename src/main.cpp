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
	
	//giocatore (statico)
	sf::Texture player(player_png, player_png_len);
	sf::Sprite player_sprite(player); //serve per renderlo drawable

	//sposta origine al centro della texture
    float player_centro_x = static_cast<float>(player.getSize().x) / 2.0;
    float player_centro_y = static_cast<float>(player.getSize().y) / 2.0;
    player_sprite.setOrigin(sf::Vector2f(player_centro_x, player_centro_y));

	player_sprite.setScale(sf::Vector2f(0.5, 0.5));
	player_sprite.setPosition(sf::Vector2f(static_cast<float>(desktop.size.x) / 2, static_cast<float>(desktop.size.y) * 0.8)); 




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
