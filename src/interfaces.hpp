/*--------------------------------------
----------------------------------------
Questo file contiene: 
    Struct Start (schermata iniziale)
    Struct End (schermata finale)
    Struct Ui (punti-livello-vite)
    Pause
----------------------------------------
--------------------------------------*/
#include <SFML/Graphics.hpp>
#include <string>


struct Start {
    sf::Font font;
    sf::Text title;
    sf::Text caption;
    sf::Clock effect_clock; //clock per effetto 

    Start();
    void updateCaption();
    void draw(sf::RenderWindow& window);
};


struct End {
    sf::Font font;
    sf::Text title;
    sf::Text caption;
    sf::Text finalScore;
    sf::Clock effect_clock; //clock per effetto 

    End();
    void updateCaption();
    void update(int playerScore);
    void draw(sf::RenderWindow& window);
    
};


struct Ui {
    sf::Font font;
    sf::Text livesText;
    sf::Text scoreText;
    sf::Text levelText;
    sf::Text shieldText;

    Ui();
    void update(int playerLifes, int playerScore, int level, int shields);
    void draw(sf::RenderWindow& window);
};


struct Pause {
    sf::Font font;
    sf::Text title;
    std::vector<sf::Text> captions;
    int selectedCaptionIndex;

    Pause();
    void up();
    void down();
    void mouse(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
};


struct Background {
    sf::Texture texture;
    sf::Sprite sprite;
    
    int frameWidth;
    int frameHeight;
    int currentFrame = 0;
    float sec_per_frame = 0.4; 
    sf::Clock anim_clock;

    Background();
    void load(const std::string& filepath, const sf::Vector2u& windowSize);
    void animate();
    void draw(sf::RenderWindow& window);
};