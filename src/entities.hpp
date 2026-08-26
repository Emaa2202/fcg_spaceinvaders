/*--------------------------------------
----------------------------------------
Questo file contiene:
    Player
    PlayerBullet
    Shield
    Nuke
    Enemy
    EnemyBullet
    ShieldCharger
    Nukeship
    Explosion
----------------------------------------
--------------------------------------*/
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <vector>

inline void centerOrigin(sf::Sprite& sprite) { //inline per evitare dichiarazione multipla
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f)); 
}


struct Player {
    int lifes = 3;
    int shields = 2;
    int nukes = 0;
    int score = 0;
    int level = 1;

    sf::Sprite sprite;
    sf::Clock cooldown; //cooldown proiettili

    //animazione
    int frameWidth;
    int frameHeight;
    int currentFrame = 0;
    sf::Clock cronometro_animaz;
    float sec_per_frame = 0.1;

    Player(const sf::Texture& texture):  
        sprite(texture)
    {   
        frameWidth = texture.getSize().x / 3.0; //2 frame, dim divise per 2
        frameHeight = texture.getSize().y;
        sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight})); //sprite predefinito, y sempre 0 perchè uso hpp 
        
        centerOrigin(sprite);
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
        shields = 2;
        nukes = 0; 
    }

    bool canShoot() {
        return cooldown.getElapsedTime().asSeconds() >= 0.45;
    }

    void animate() {
        if(cronometro_animaz.getElapsedTime().asSeconds() >= sec_per_frame) {
            currentFrame = (currentFrame + 1) % 3; 

            int rectX = currentFrame * frameWidth;
            sprite.setTextureRect(sf::IntRect({rectX, 0}, {frameWidth, frameHeight})); //cambio sprite

            cronometro_animaz.restart();
        }
    }

};

struct playerBullet {
	float speed = 45.0;
	sf::Sprite sprite;

	playerBullet(const sf::Texture& texture, sf::Vector2f pos_iniziale) :
        sprite(texture)
	{
		centerOrigin(sprite);
		sprite.setPosition(pos_iniziale);
	}
};


struct Shield {
    sf::Sprite sprite;
    sf::Clock clock; //durata scudo
    sf::Clock cooldown; //cooldown

    Shield(const sf::Texture& texture) :
        sprite(texture)
    {
        centerOrigin(sprite);
        sprite.setScale(sf::Vector2f(0.5, 0.5));
        sprite.setColor(sf::Color(255, 255, 255, 100)); //leggermente trasparente
    }

};


struct Nuke {   
    sf::Sprite sprite;
    float speed = 15.0;

    Nuke(const sf::Texture& texture) :
        sprite(texture)
    {
        centerOrigin(sprite);
        sprite.setScale(sf::Vector2f(0.4, 0.5));
    }
};


enum enemyType {
    Type1,
    Type2,
    Type3
};

struct Enemy {
    bool isAlive = true;
    int points;
    int col; //colonna per capire se puo sparare
    enemyType type;
    sf::Sprite sprite;
    sf::Clock enemyBullet_clock; //spostato qui per farli sparare anche assieme 

    //animazione
    int frameWidth;
    int frameHeight;
    int currentFrame = 0;
    float sec_per_frame; //al momento utilizzato solo per Type1
    sf::Clock cronometro_animaz;

    Enemy(const sf::Texture& texture, enemyType init_type, int column, sf::Vector2f pos) :
        sprite(texture),
        type(init_type),
        col(column)
    {
        if(type == Type3) frameWidth = texture.getSize().x / 2.0; //2 frame, dim divise per 2
        else frameWidth = texture.getSize().x / 3.0; 
        frameHeight = texture.getSize().y;

        sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight})); //sprite predefinito, y sempre 0 perchè uso hpp 

        centerOrigin(sprite);
        sprite.setPosition(pos);

        switch(type){
            case Type1:
                sprite.setScale(sf::Vector2f(0.6, 0.6));
                sec_per_frame = 0.15;
                points = 10;
            break;

            case Type2:
                sprite.setScale(sf::Vector2f(0.7, 0.7));
                sec_per_frame = 0.35;
                points = 15;
            break;

            case Type3:
                sprite.setScale(sf::Vector2f(0.8, 0.8));
                sec_per_frame = 0.5;
                points = 20;
            break;
        }
    }

    void animate() {
        if(cronometro_animaz.getElapsedTime().asSeconds() >= sec_per_frame) {
            if(type == Type3)currentFrame = 1 - currentFrame; //alterna i frame 
            else currentFrame = (currentFrame + 1) % 3;

            //currentFrame = 0 ---> X = 0
            //currentFrame = 1 ---> X = frameWidth
            int rectX = currentFrame * frameWidth;
            sprite.setTextureRect(sf::IntRect({rectX, 0}, {frameWidth, frameHeight})); //cambio sprite

            cronometro_animaz.restart();
        }
    }

    bool isFrontEnemy(const Enemy& target, const std::vector<Enemy>& enemies) {
        float targetY = target.sprite.getPosition().y;

        for(const auto& enemy : enemies) {
            if(enemy.isAlive && enemy.col == target.col && enemy.sprite.getPosition().y > targetY) return false;
        }

        return true;
    }

};

struct enemyBullet {
    float speed = 15.0;
    sf::Sprite sprite;
	sf::Vector2f pos;

	enemyBullet(const sf::Texture& texture, sf::Vector2f pos_iniziale) :
		pos(pos_iniziale),
		sprite(texture)
	{
		centerOrigin(sprite);
		sprite.setPosition(pos);
	}
};


struct ShieldCharger {
    float speed = 15.0;
    sf::Sprite sprite;

    int frameWidth;
    int frameHeight;
    int currentFrame = 0;
    sf::Clock cornometro_animaz;
    float sec_per_frame = 0.2;

    ShieldCharger(const sf::Texture& texture) :
        sprite(texture)
    {
        frameWidth = texture.getSize().x / 6.0; 
        frameHeight = texture.getSize().y;

        sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight})); //sprite predefinito, y sempre 0 perchè uso hpp 

        centerOrigin(sprite);
        sprite.setScale(sf::Vector2f(0.3, 0.3));
    }

    void animate() {
        if(cornometro_animaz.getElapsedTime().asSeconds() >= sec_per_frame) {
            currentFrame = (currentFrame + 1) % 6; //alterna i frame

            int rectX = currentFrame * frameWidth;
            sprite.setTextureRect(sf::IntRect({rectX, 0}, {frameWidth, frameHeight})); //cambio sprite

            cornometro_animaz.restart();
        }
    }
};


struct BonusShip {   
    sf::Sprite sprite;
    int lifes = 3;
    float speed = 8.0;
    bool rightDirection = true;

    int frameWidth;
    int frameHeight;
    int currentFrame = 0;
    sf::Clock cornometro_animaz;
    float sec_per_frame = 0.2;

    BonusShip(const sf::Texture& texture) :
        sprite(texture)

    {
        frameWidth = texture.getSize().x / 4.0; 
        frameHeight = texture.getSize().y / 2.0;
        sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight})); 
        
        centerOrigin(sprite);
        sprite.setScale(sf::Vector2f(0.5, 0.5));
    }

    void setDirection(bool isRight) {
        rightDirection = isRight;

        int currentFrame;
        if(rightDirection) currentFrame = 0;
        else currentFrame = 1;

        int rectX = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect({rectX, 0}, {frameWidth, frameHeight})); 
    }

    void move(bool isRight) {
        if(isRight) sprite.move(sf::Vector2f(speed, 0));
        else sprite.move(sf::Vector2f(-speed, 0));
    }

    void animate() {
        if(cornometro_animaz.getElapsedTime().asSeconds() >= sec_per_frame) {
            currentFrame = (currentFrame + 1) % 4; //alterna i frame

            int rectX = currentFrame * frameWidth;
            int rectY = 0;
            if(!rightDirection) rectY = frameHeight;
            
            sprite.setTextureRect(sf::IntRect({rectX, rectY}, {frameWidth, frameHeight})); //cambio sprite

            cornometro_animaz.restart();
        }
    }
};


struct Explosion {
    float duration = 0.2;
    sf::Sprite sprite;
    sf::Clock clock; //per far durare l'espolosione

    Explosion(float scale, const sf::Texture& texture, sf::Vector2f position) :
		sprite(texture)
	{
		centerOrigin(sprite);
        sprite.setPosition(position);
        sprite.setScale(sf::Vector2f(scale, scale));
        clock.restart();
	}

    bool isExpired() {
        return clock.getElapsedTime().asSeconds() >= duration;
    }

};
