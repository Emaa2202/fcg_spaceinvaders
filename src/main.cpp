#include <SFML/Graphics.hpp>
#include <string>

//percorso per raggiungere media, che viene copiata dentro build dallo script CMakeLists (riga script aggiunta con IA)
std::string mediaPath = "../media/"; 

int main()
{
	
	
	//recupero desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window( sf::VideoMode( { desktop.size.x, desktop.size.y } ), "Space Invaders");
	window.setFramerateLimit(60);
	
	//caricamento sfondo
	sf::Texture sfondo;
	sfondo.loadFromFile(mediaPath + "spacebackground.jpg");
	sf::Sprite sfondo_sprite(sfondo); //serve per renderlo drawable

	//bozza statica del giocatore
	sf::RectangleShape player(sf::Vector2f(50, 100));
	player.setPosition(sf::Vector2f(desktop.size.x / 2, desktop.size.y * 0.8)); 

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		//sfondo
		window.clear();
		window.draw(sfondo_sprite);

		//giocatore
		window.draw(player);

		window.display();
	}
}
