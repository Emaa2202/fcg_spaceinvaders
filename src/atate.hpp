struct State {
    //risorse generali
    sf::RenderWindow window;

    sf::Texture background;
    sf::Sprite background_sprite;

    Player player;

	std::vector<playerBullet> playerBullets;
    sf::Texture playerBullet_texture;

    std::vector<Enemy> enemies;
    int rows = 6;
    int columns = 12;
    sf::Texture enemy1_texture;
    sf::Texture enemy3_texture;
    sf::Texture enemy2_texture;

    sf::Clock move_clock; //per spostamento nemici
    bool right_dir = true; //direzione nemici, prima era sotto ma mi serve persistente

    sf::Texture enemyBullet_texture;
    std::vector<enemyBullet> enemyBullets;

    sf::Texture explosion_texture;
    sf::Sprite explosion_sprite;
    std::vector<Explosion> explosions;

    bool gameOver = false; //per far apparirre la schermata gameover
    bool startScreen = true;

    Ui ui;
    Start start;
    End end;

    sf::Clock gameoverTransition_clock; //per non far apparire la schermata gameover instantaneamente
    bool gameoverTransition = false;

    sf::Clock nextLevelTransition_clock; 
    bool nextLevelTransition = false;

    int level = 1; //contatore livelli
    
    //posizionamento nemici
    void initEnemies() {
        float screenWidth = static_cast<float>(sf::VideoMode::getDesktopMode().size.x);
        float screenHeight = static_cast<float>(sf::VideoMode::getDesktopMode().size.y);

        float distX = 250; //distanze tra nemici
        float distY = 130;

        float gridWidth = (columns - 1) * distX; //dimensioni griglia
        float gridHeight = (rows - 1) * distY;

        float startX = (screenWidth - gridWidth) / 2; //posizionamento effettivo griglia
        float startY = screenHeight * 0.07;
        
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                float posX = startX + (j * distX);
                float posY = startY + (i * distY);
            
                if(i == 4 ||i == 5){
                    enemies.push_back(Enemy(enemy1_texture, Type1, j, sf::Vector2f(posX, posY)));
                } 
                else if(i == 2 || i == 3){
                    enemies.push_back(Enemy(enemy2_texture, Type2, j, sf::Vector2f(posX, posY)));
                }
                else {
                    enemies.push_back(Enemy(enemy3_texture, Type3, j, sf::Vector2f(posX, posY)));
                }
            }
        }
    }

    
    State() :
        //caricamento texture e collegamento agli sprite
        background(spacebackground_jpg, spacebackground_jpg_len),
        background_sprite(background),

        playerBullet_texture(playerBullet_png, playerBullet_png_len),

        enemy1_texture(enemy1_sheet_png, enemy1_sheet_png_len),
        enemy2_texture(enemy2_sheet_png, enemy2_sheet_png_len),
        enemy3_texture(enemy3_sheet_png, enemy3_sheet_png_len),

        enemyBullet_texture(enemyBullet_png, enemyBullet_png_len),
        
        explosion_texture(explosion_png, explosion_png_len),
        explosion_sprite(explosion_texture)
    
    {   
        //creazione finestra
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode({desktop.size.x, desktop.size.y}), "Space Invaders");
        window.setFramerateLimit(60);

        //sfondo di dimensione dello schermo
        double background_scale_x = (static_cast<float>(desktop.size.x) / background.getSize().x); 
        double background_scale_y = (static_cast<float>(desktop.size.y) / background.getSize().y);
        background_sprite.setScale(sf::Vector2f(background_scale_x, background_scale_y));

        initEnemies();        
        
    }
};