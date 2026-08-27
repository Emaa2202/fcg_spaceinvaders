/*------------------------------------------------------------
--------------------------------------------------------------
Questo file contiene:
    Costruttori di Start, End, Ui, State con relative funzioni
--------------------------------------------------------------
------------------------------------------------------------*/
#include <SFML/Audio.hpp>
#include <algorithm> //per clamp che mi semplifica il movimento 
#include <ctime> //per rand
#include <cstdlib>

#include "state.hpp"
#include "graphics/font.hpp"
#include "graphics/nukeAlert.hpp"
/*------------------
--------START-------
-------------------*/
Start::Start() :
        title(font), 
        caption(font)
    {    
        font.openFromMemory(font_ttf, font_ttf_len); //prende font embedded

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

void Start::updateCaption() {
    if(static_cast<int>(effect_clock.getElapsedTime().asSeconds()) % 2 == 0) {
        caption.setFillColor(sf::Color::Transparent);
    }
    else {
        caption.setFillColor(sf::Color::White);
    }   
}

void Start::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(caption);
}


/*------------------
---------END--------
-------------------*/
End::End() :
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

        //punti
        finalScore.setString("Punteggio: 0");
        finalScore.setCharacterSize(150);
        finalScore.setFillColor(sf::Color::White);
        
        //mex di premere invio
        caption.setString("Premi invio per giocare ancora");
        caption.setCharacterSize(50);
}

void End::updateCaption() {
        if(static_cast<int>(effect_clock.getElapsedTime().asSeconds()) % 2 == 0) {
            caption.setFillColor(sf::Color::Transparent);
        }
        else {
            caption.setFillColor(sf::Color::White);
        }   
    }

void End::update(int playerScore) {
    finalScore.setString("Punteggio: " + std::to_string(playerScore));
    
    //dim e pos punti e mex invio spostati qui per centrarli dinamicamente
    sf::FloatRect sbounds = finalScore.getLocalBounds();
    finalScore.setOrigin(sf::Vector2f(sbounds.size.x / 2, sbounds.size.y / 2));
    finalScore.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, title.getPosition().y * 2));

    sf::FloatRect cbounds = caption.getLocalBounds();
    caption.setOrigin(sf::Vector2f(cbounds.size.x / 2, cbounds.size.y / 2));
    caption.setFillColor(sf::Color::White);
    caption.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, finalScore.getPosition().y * 1.3));
}

void End::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(caption);
    window.draw(finalScore);
}


/*------------------
---------UI---------
-------------------*/
Ui::Ui() :
        livesText(font),
        scoreText(font),
        levelText(font),
        shieldText(font),
        alert(alert_texture)
    {    
        font.openFromMemory(font_ttf, font_ttf_len);
        
        //punti
        scoreText.setString("Punteggio: 0");
        scoreText.setCharacterSize(64);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x * 0.01, sf::VideoMode::getDesktopMode().size.y * 0.89)); //position rispetto a origine, globalbounds rispetto a fone testo
        
        //vite
        livesText.setString("Vite: 3");
        livesText.setCharacterSize(64);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(sf::Vector2f(scoreText.getGlobalBounds().size.x * 1.7, scoreText.getPosition().y));


        //scudi
        shieldText.setString("Scudi: 2");
        shieldText.setCharacterSize(64);
        shieldText.setFillColor(sf::Color::White);
        shieldText.setPosition(sf::Vector2f(livesText.getPosition().x + livesText.getGlobalBounds().size.x * 1.5, livesText.getPosition().y));

        //contatore livelli
        levelText.setString("Livello: 1");
        levelText.setCharacterSize(64);
        levelText.setFillColor(sf::Color::White);
        levelText.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x * 0.85, sf::VideoMode::getDesktopMode().size.y * 0.89));

        //alert colpo bonus dispinibile
        alert_texture.loadFromMemory(nukeAlert_png, nukeAlert_png_len); 
        alert.setTexture(alert_texture, true); //se non lo metto non appare
        alert.setScale(sf::Vector2f(0.7, 0.7));
        centerOrigin(alert);
        alert.setPosition(sf::Vector2f(shieldText.getPosition().x + shieldText.getGlobalBounds().size.x * 1.5, shieldText.getGlobalBounds().position.y + shieldText.getGlobalBounds().size.y / 2.0)); //origine foto in 0,0, origine testo in alto a sx
}

void Ui::update(int playerLifes, int playerScore, int level, int shield) {
    livesText.setString("Vite: " + std::to_string(playerLifes));
    scoreText.setString("Punteggio: " + std::to_string(playerScore));
    levelText.setString("Livello: " + std::to_string(level));
    shieldText.setString("Scudi: " + std::to_string(shield));
}

void Ui::updateAlert() {
    if(static_cast<int>(effect_clock.getElapsedTime().asSeconds()) % 2 == 0) {
        alert.setColor(sf::Color::Transparent);
    }
    else {
        alert.setColor(sf::Color::White);
    }   
}

void Ui:: drawAlert(sf::RenderWindow& window) {
    window.draw(alert);
}

void Ui:: draw(sf::RenderWindow& window) {
    window.draw(livesText);
    window.draw(scoreText);
    window.draw(shieldText);
    window.draw(levelText);
}


/*------------------
--------PAUSE-------
-------------------*/
Pause::Pause() :
        title(font) 
    {    
        font.openFromMemory(font_ttf, font_ttf_len);

        //titolo
        title.setString("Pausa");
        title.setCharacterSize(412);
        
        sf::FloatRect bounds = title.getLocalBounds();
        title.setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));

        title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, (sf::VideoMode::getDesktopMode().size.y/ 2.0) * 0.5));

        selectedCaptionIndex = 0; //prima di default
        for(int i = 0; i < 3; ++i) {
            sf::Text text(font);
            if(i == 0) text.setString("Riprendi");
            else if(i == 1) text.setString("Riavvia");
            else text.setString("Esci");
            text.setCharacterSize(128);
            
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(sf::Vector2f(textBounds.size.x / 2, textBounds.size.y / 2));
            
            //una sotto l'altra
            if(i == 0) text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, sf::VideoMode::getDesktopMode().size.y/ 2.0 * 0.6 + 300));
            else if(i == 1) text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, sf::VideoMode::getDesktopMode().size.y/ 2.0 * 0.6 + 450));
            else text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().size.x / 2.0, sf::VideoMode::getDesktopMode().size.y/ 2.0 * 0.6 + 600));

            //evidenzia
            if(i == 0) text.setFillColor(sf::Color::White);
            else text.setFillColor(sf::Color(128,128,128));

            captions.push_back(text);
        }

}

void Pause::up() {
    if(selectedCaptionIndex - 1 >= 0) {
        captions[selectedCaptionIndex].setFillColor(sf::Color(128,128,128));
        selectedCaptionIndex--;
        captions[selectedCaptionIndex].setFillColor(sf::Color::White);
    }
    else {
        down();
        down();
    }
}

void Pause::down() {
    if(selectedCaptionIndex + 1 < 3) {
        captions[selectedCaptionIndex].setFillColor(sf::Color(128,128,128));
        selectedCaptionIndex++;
        captions[selectedCaptionIndex].setFillColor(sf::Color::White);

    }
    else {
        up();
        up();
    } 
}

void Pause::mouse(sf::Vector2f mousePos) {
    for(int i =0; i < 3; i++) {
        if(captions[i].getGlobalBounds().contains(mousePos)) { //se mouse è su scritta
            if(selectedCaptionIndex != i){  //se =i ignora e resta di default
                captions[selectedCaptionIndex].setFillColor(sf::Color(128,128,128)); //scelta precedente 
                selectedCaptionIndex = i; //aggiorna scelta corrente
                captions[selectedCaptionIndex].setFillColor(sf::Color::White); //scelta corrente 
            }
        }   
    }
}

void Pause::draw(sf::RenderWindow& window) {
    window.draw(title);
    for(const auto& c : captions) {
        window.draw(c);
    }
}


/*------------------
-----BACKGROUND-----
-------------------*/
Background::Background() :
                sprite(texture) 
            {

            }

void Background::load(const std::string& filepath, const sf::Vector2u& windowSize) {
    texture.loadFromFile(filepath);
        
    sprite.setTexture(texture);
        
    frameWidth = texture.getSize().x / 5;
    frameHeight = texture.getSize().y;
        
    //primo frame
    sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
        
    float scaleX = static_cast<float>(windowSize.x) / frameWidth;
    float scaleY = static_cast<float>(windowSize.y) / frameHeight;
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
}
    
void Background::animate() {
    if(anim_clock.getElapsedTime().asSeconds() >= sec_per_frame) {
        currentFrame = (currentFrame + 1) % 5; 
        
        int rectX = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect({rectX, 0}, {frameWidth, frameHeight}));
        
        anim_clock.restart();
    }
}

void Background::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}


/*------------------
--------STATE-------
-------------------*/
State::State(const std::string& path_to_media) :
        mediaDir(path_to_media),
        //caricamento texture e collegamento agli sprite
        player(assets.player_texture),
        shield(assets.shield_texture),
        shield_sound(assets.shieldSound_buffer),
        shieldCharger(assets.shieldCharger_texture),
        shieldChargerSound(assets.shieldChargerSound_buffer),
        nuke(assets.nuke_texture),
        nukeSound(assets.nukeSound_buffer),
        bonusship(assets.bonusShip_texture),
        playerBullets_sound(assets.playerBullets_buffer),
        playerExplosion_sound(assets.playerExplosion_buffer)
    
    {   
        //gestioni audio
        playerBullets_sound.setVolume(20.0);
        playerExplosion_sound.setVolume(100.0);
        shieldChargerSound.setVolume(200.0);

        //creazione finestra
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode({desktop.size.x, desktop.size.y}), "Space Invaders");
        window.setFramerateLimit(60);

        std::string pathSfondo = mediaDir + "/png/background.png"; 
        background.load(pathSfondo, window.getSize());

        initEnemies();
}

//posizionamento nemici
void State::initEnemies() {
    float screenWidth = static_cast<float>(sf::VideoMode::getDesktopMode().size.x);
    float screenHeight = static_cast<float>(sf::VideoMode::getDesktopMode().size.y);
    
    float distX = 250; //distanze tra nemici
    float distY = 130;
    
    float gridWidth = (columns - 1) * distX; //dimensioni griglia
    float gridHeight = (rows - 1) * distY;
    
    float startX = (screenWidth - gridWidth) / 2; //posizionamento effettivo griglia
    float startY = screenHeight * 0.12;
     
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            float posX = startX + (j * distX);
            float posY = startY + (i * distY);
        
            if(i == 4 ||i == 5){
                enemies.push_back(Enemy(assets.enemy1_texture, Type1, j, sf::Vector2f(posX, posY)));
            } 
            else if(i == 2 || i == 3){
                enemies.push_back(Enemy(assets.enemy2_texture, Type2, j, sf::Vector2f(posX, posY)));
            }
            else {
                enemies.push_back(Enemy(assets.enemy3_texture, Type3, j, sf::Vector2f(posX, posY)));
            }
        }
    }

    enemiesQuantity = enemies.size(); //inizializza contatore nemici
}

void State::playMusic(const std::string& trackName) { //utilizzo playMusic 1 volta nel main, 1 volta al restart e al gameover faccio soundtrack.stop()
    std::string path = audioDir + "/" + trackName;
    soundtrack.openFromFile(path);
    soundtrack.stop(); //se c era qualcosa prima, lo ferma
    soundtrack.setLooping(true);
    soundtrack.setVolume(50.0);
    soundtrack.play();
}

void State::restartGame() {
    player.resetAll();
    enemies.clear();
    playerBullets.clear();
    enemyBullets.clear();
    explosions.clear();

    gameOver = false;
    gameoverTransition = false;
    
    playMusic("soundtrack.mp3"); //riproduce audio con var soundtrack
    
    isShield = false;
    shieldChargerReleased = false;
    existsNuke = false;
    existsBonusShip = false;
    initEnemies();
}