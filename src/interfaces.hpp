/*--------------------------------------
----------------------------------------
Questo file contiene: 
    Struct Start (schermata iniziale)
    Struct End (schermata finale)
    Struct Ui (punti-livello-vite)
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

    End();

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
    sf::Text levelText;

    Ui();

    void update(int playerLifes, int playerScore, int level) {
        livesText.setString("Vite: " + std::to_string(playerLifes));
        scoreText.setString("Punteggio: " + std::to_string(playerScore));
        levelText.setString("Livello: " + std::to_string(level));
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(livesText);
        window.draw(scoreText);
        window.draw(levelText);
    }
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
    

    void draw(sf::RenderWindow& window) const {
        window.draw(title);
        for (const auto& c : captions) {
            window.draw(c);
        }
    }
    
};
