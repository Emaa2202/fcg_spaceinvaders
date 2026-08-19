/*--------------------------------------
----------------------------------------
Questo file contiene:
    Player
    PlayerBullet
    Enemy
    EnemyBullet
    Explosion
----------------------------------------
--------------------------------------*/
#include <SFML/Graphics.hpp>
#include <vector>


struct Player {
    sf::Sprite sprite;
    int lifes = 3;
    int score = 0;
    sf::Clock cooldown; //cooldown proiettili
    int level = 1;

    Player(const sf::Texture& texture):  
        sprite(texture)
    {   
        //sposta origine di player al centro dello sprite
        float player_centro_x = static_cast<float>(texture.getSize().x) / 2.0;
        float player_centro_y = static_cast<float>(texture.getSize().y) / 2.0;
        sprite.setOrigin(sf::Vector2f(player_centro_x, player_centro_y));    
        sprite.setScale(sf::Vector2f(0.3, 0.4));
        
        sprite.setPosition(sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().size.x) / 2.0, static_cast<float>(sf::VideoMode::getDesktopMode().size.y) * 0.8));    
	    
    }

    void resetPosition() {
        sprite.setPosition(sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().size.x) / 2.0, static_cast<float>(sf::VideoMode::getDesktopMode().size.y) * 0.8)); 
    }

    void resetAll() {
        sprite.setPosition(sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().size.x) / 2.0, static_cast<float>(sf::VideoMode::getDesktopMode().size.y) * 0.8));
        lifes = 3;
        score = 0;
        level = 1; 
    }

    bool canShoot() {
        return cooldown.getElapsedTime().asSeconds() >= 0.45;
    }

};


struct playerBullet {
	float speed;
	sf::Sprite sprite;

	playerBullet(const sf::Texture& texture, sf::Vector2f pos_iniziale) :
        sprite(texture),
		speed(45.0)
	{
		float centro_x = static_cast<float>(texture.getSize().x) / 2.0;
        float centro_y = static_cast<float>(texture.getSize().y) / 2.0;
        sprite.setOrigin(sf::Vector2f(centro_x, centro_y));
		
		sprite.setPosition(pos_iniziale);
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

    Enemy(const sf::Texture& texture, enemyType init_type, int column, sf::Vector2f pos) :
        sprite(texture),
        type(init_type),
        isAlive(true),
        col(column)
    {
        frameWidth = texture.getSize().x / 2.0; //2 frame, dim divise per 2
        frameHeight = texture.getSize().y;

        sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight})); //sprite predefinito, y sempre 0 perchè uso hpp 

        float centro_x = static_cast<float>(frameWidth) / 2; //centro calcolato su singolo frame
        float centro_y = static_cast<float>(frameHeight) / 2;
        sprite.setOrigin(sf::Vector2f(centro_x, centro_y));

        sprite.setPosition(pos);

        switch(type){
            case Type1:
                sprite.setScale(sf::Vector2f(0.6, 0.6));
                points = 10;
            break;

            case Type2:
                sprite.setScale(sf::Vector2f(1.0, 1.0));
                points = 15;
            break;

            case Type3:
                sprite.setScale(sf::Vector2f(0.8, 0.8));
                points = 20;
            break;
        }
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