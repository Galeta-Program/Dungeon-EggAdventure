/***********************************************************************
 * File: DungeonGUI.cpp
 * Author: JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/06/06
 * Editor: JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/06
 * Description: This program is for GUI of Dungeon Game
***********************************************************************/
#include "DungeonGUI.h"

// Intent:	default constructor
// Pre:		None
// Post:	None
DungeonGUI::DungeonGUI()
{
    // create a window of game, set window mode is fullscreen
    this->mainWindow = new RenderWindow(VideoMode::getDesktopMode(), "Dungeon", sf::Style::Fullscreen);
    font->loadFromFile("resources/sansation.ttf"); // set a font file to font

    // set current background of game 
    backgroundTexture.loadFromFile("./image/mainMenuBackground.png"); // set image is background of main menu 
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(mainWindow->getSize().x / backgroundSprite.getLocalBounds().width), static_cast<float>(mainWindow->getSize().y / backgroundSprite.getLocalBounds().height));

    // set button of close save menu 
    backButton.setSize(Vector2f(200, 50));
    backButton.setPosition((mainWindow->getSize().x - backButton.getSize().x) / 2, (mainWindow->getSize().y + backButton.getSize().y) / 1.2);
    backButton.setFillColor(Color::Transparent);

    // set text of close save menu 
    backButtonText.setString("Back");
    backButtonText.setFont(*font);
    backButtonText.setCharacterSize(40);
    backButtonText.setPosition(backButton.getPosition().x + 45, backButton.getPosition().y);
    backButtonText.setFillColor(Color::Black);
    backButtonText.setStyle(Text::Bold);

    // set background of save menu
    menuBackground.loadFromFile("./image/menubackground.png");
    menuSprite.setTexture(menuBackground);
    menuSprite.setScale(static_cast<float>(mainWindow->getSize().x / menuSprite.getLocalBounds().width), static_cast<float>(mainWindow->getSize().y / menuSprite.getLocalBounds().height));

    // set background of pause menu
    pauseBGTexture.loadFromFile("./image/pauseBackground.png");
    pauseBGSprite.setTexture(pauseBGTexture);
    pauseBGSprite.setScale(static_cast<float>(mainWindow->getSize().x / pauseBGSprite.getLocalBounds().width), static_cast<float>(mainWindow->getSize().y / pauseBGSprite.getLocalBounds().height));

    // set background of operation method menu
    operationMethodTexture.loadFromFile("./image/operationMethod.png");
    operationMethodSprite.setTexture(operationMethodTexture);
    operationMethodSprite.setScale(static_cast<float>(mainWindow->getSize().x / operationMethodSprite.getLocalBounds().width), static_cast<float>(mainWindow->getSize().y / operationMethodSprite.getLocalBounds().height));

    // set background of setting volumemenu
    volumeSettingTexture.loadFromFile("./image/volumeSetting.png");
    volumeSettingSprite.setTexture(volumeSettingTexture);
    volumeSettingSprite.setScale(static_cast<float>(mainWindow->getSize().x / volumeSettingSprite.getLocalBounds().width), static_cast<float>(mainWindow->getSize().y / volumeSettingSprite.getLocalBounds().height));

    // set the button to switch to the operation method menu
    operationMethodButton.setSize(Vector2f(mainWindow->getSize().x / 2, 120));
    operationMethodButton.setPosition(mainWindow->getSize().x / 2, 0);
    operationMethodButton.setFillColor(Color::Transparent);

    // set the button to switch to the setting volume menu
    volumeSettingButton.setSize(Vector2f(mainWindow->getSize().x / 2, 120));
    volumeSettingButton.setPosition(0, 0);
    volumeSettingButton.setFillColor(Color::Transparent);

    // set the object used to view bgm volume ranges
    bgmVolumeLineTexture.loadFromFile("./image/volumeSettingLine.png");
    bgmVolumeLineSprite.setTexture(bgmVolumeLineTexture);
    bgmVolumeLineSprite.setPosition(mainWindow->getSize().x / 3, mainWindow->getSize().y / 4);

    // set the objects for adjusting bgm volume
    bgmVolumeBallTexture.loadFromFile("./image/volumeSettingBall.png");
    bgmVolumeBallSprite.setTexture(bgmVolumeBallTexture);
    bgmVolumeBallSprite.setPosition(mainWindow->getSize().x / 3 + (bgmVolumeLineTexture.getSize().x * 0.5) - 42.5, mainWindow->getSize().y / 4 + 5);

    // set the text of bgm volume
    bgmVolumeText.setFont(*font);
    bgmVolumeText.setCharacterSize(70);
    bgmVolumeText.setString(to_string(bgmVolume));
    bgmVolumeText.setFillColor(Color::Black);
    bgmVolumeText.setPosition(bgmVolumeLineSprite.getPosition().x + bgmVolumeLineTexture.getSize().x + 100, bgmVolumeLineSprite.getPosition().y);
    
    // set the object used to view sound volume ranges
    soundVolumeLineTexture.loadFromFile("./image/volumeSettingLine.png");
    soundVolumeLineSprite.setTexture(soundVolumeLineTexture);
    soundVolumeLineSprite.setPosition(mainWindow->getSize().x / 3, mainWindow->getSize().y / 1.7);

    // set the objects for adjusting sound volume
    soundVolumeBallTexture.loadFromFile("./image/volumeSettingBall.png");
    soundVolumeBallSprite.setTexture(soundVolumeBallTexture);
    soundVolumeBallSprite.setPosition(mainWindow->getSize().x / 3 + (soundVolumeLineTexture.getSize().x * 0.5) - 42.5, mainWindow->getSize().y / 1.7 + 5);

    // set set the text of sound volume
    soundVolumeText.setFont(*font);
    soundVolumeText.setCharacterSize(70);
    soundVolumeText.setString(to_string(soundVolume));
    soundVolumeText.setFillColor(Color::Black);
    soundVolumeText.setPosition(soundVolumeLineSprite.getPosition().x + soundVolumeLineTexture.getSize().x + 100, soundVolumeLineSprite.getPosition().y);
}

// Intent:	screen of game main menu
// Pre:		None
// Post:	None
void DungeonGUI::homeScreen()
{  
    isClick = true;
    isSaveMenuOpen = false;
    backgroundTexture.loadFromFile("./image/mainMenuBackground.png"); // set background of game is main menu backgound image

    // create and set a image of game title 
    Sprite titleSprite;
    Texture titleTexture;
    titleTexture.loadFromFile("./image/title.png");
    titleSprite.setTexture(titleTexture);
    titleSprite.setPosition((mainWindow->getSize().x - titleTexture.getSize().x) / 2, titleTexture.getSize().y / 5);  

    // create and set a button to start game
    RectangleShape startButton(Vector2f(200, 50));
    startButton.setPosition((mainWindow->getSize().x - startButton.getSize().x) / 2, (mainWindow->getSize().y + startButton.getSize().y) / 2 + 50);
    startButton.setFillColor(Color::Transparent);

    // create and set a button to open setting game menu
    RectangleShape settingButton(Vector2f(200, 50));
    settingButton.setPosition((mainWindow->getSize().x - settingButton.getSize().x) / 2, (mainWindow->getSize().y + settingButton.getSize().y) / 2 + 130);
    settingButton.setFillColor(Color::Transparent);

    // create and set a button to quit game
    RectangleShape quitButton(Vector2f(200, 50));
    quitButton.setPosition((mainWindow->getSize().x - quitButton.getSize().x) / 2, (mainWindow->getSize().y + quitButton.getSize().y) / 2 + 210);
    quitButton.setFillColor(Color::Transparent);

    // create and set a text of start game
    Text startButtonText("Start", *font, 40);
    startButtonText.setPosition(startButton.getPosition().x + 58, startButton.getPosition().y);
    startButtonText.setFillColor(Color::White);
    startButtonText.setStyle(Text::Bold);

    // create and set a text of open setting game menu
    Text settingButtonText("Setting", *font, 40);
    settingButtonText.setPosition(settingButton.getPosition().x + 40, settingButton.getPosition().y);
    settingButtonText.setFillColor(Color::White);
    settingButtonText.setStyle(Text::Bold);

    // create and set a text of quit game
    Text quitButtonText("Quit", *font, 40);
    quitButtonText.setPosition(quitButton.getPosition().x + 60, quitButton.getPosition().y);
    quitButtonText.setFillColor(Color::White);
    quitButtonText.setStyle(Text::Bold);

    // create and set image of menu chosen 
    Texture chosenTexture;
    Sprite chosenSprite;
    chosenTexture.loadFromFile("./image/mainMenuChosen.png");   
    chosenSprite.setTexture(chosenTexture);
    chosenSprite.setPosition(mainWindow->getSize().x, mainWindow->getSize().y);

    // create three boxes of save file
    saveBox1 = new SaveBox(mainWindow, "data" + to_string(1));
    saveBox2 = new SaveBox(mainWindow, "data" + to_string(2));
    saveBox3 = new SaveBox(mainWindow, "data" + to_string(3));

    // set the position of three boxes of save file
    saveBox1->setPosition((mainWindow->getSize().x - saveBox1->getSize().width) / 3, 100);
    saveBox2->setPosition((mainWindow->getSize().x - saveBox2->getSize().width) / 3, 400);
    saveBox3->setPosition((mainWindow->getSize().x - saveBox3->getSize().width) / 3, 700);

    // create and set bgm of main menu and player
    SoundBuffer bgmBuffer;
    bgmBuffer.loadFromFile("./sound/mainMenuBGM.wav");
    menuBGM.setBuffer(bgmBuffer);
    menuBGM.setLoop(true);
    menuBGM.setVolume(bgmVolume);
    menuBGM.play(); // play bgm of main menu

    // main menu of game loop
    while (mainWindow->isOpen())
    {
        while (mainWindow->pollEvent(event)) // get event from window of game
        {
            if (event.type == Event::Closed) // if click close window, close the window of game
            {
                mainWindow->close();
            }

            // if save menu is open and call saveMenuEvent() to check event is ture, isClick change to true  
            if (isSaveMenuOpen && saveMeunEvent()) isClick = true; 

            // if menu of operation method is open or menu of setting volume is open
            if (isOperationMethodOpen || isVolumeSettingOpen)
            {
                settingEvent(); // call settingEvent() to check event
                isClick = true;
            }
            
            // if mouse is pressed button
            if (event.type == Event::MouseButtonPressed)
            {
                // if use left mouse to click button and save menu, menu of  operation method, and menu of setting volume isn't open
                if (event.mouseButton.button == Mouse::Left && !isSaveMenuOpen && !isOperationMethodOpen && !isVolumeSettingOpen)
                {          
                    isClick = true;

                    if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // if mouse click start button
                    {
                        isSaveMenuOpen = true; // set save menu is open                   
                    }
                    else if (settingButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // if mouse click setting button
                    {
                        isVolumeSettingOpen = true; // set menu of setting volume is open
                        return;
                    }
                    else if (quitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // if mouse click quit button
                    {
                        gameState = 0; // close game
                        return;
                    }
                }                
            }
        }

        if (!isSaveMenuOpen && !isOperationMethodOpen && !isVolumeSettingOpen)
        {
            // if mouse is move on the start button
            if (startButton.getGlobalBounds().contains(static_cast<float>(Mouse::getPosition(*mainWindow).x), static_cast<float>(Mouse::getPosition(*mainWindow).y)))
            {
                isClick = true;
                // set the postion of chosen image to the position of start button
                chosenSprite.setPosition(startButton.getPosition().x + 5, startButton.getPosition().y);
            }

            // if mouse is move on the setting button
            if (settingButton.getGlobalBounds().contains(static_cast<float>(Mouse::getPosition(*mainWindow).x), static_cast<float>(Mouse::getPosition(*mainWindow).y)))
            {
                isClick = true;
                // set the postion of chosen image to the position of srtting button
                chosenSprite.setPosition(settingButton.getPosition().x + 5, settingButton.getPosition().y);
            }

            // if mouse is move on the quit button
            if (quitButton.getGlobalBounds().contains(static_cast<float>(Mouse::getPosition(*mainWindow).x), static_cast<float>(Mouse::getPosition(*mainWindow).y)))
            {
                isClick = true;
                // set the postion of chosen image to the position of quit button
                chosenSprite.setPosition(quitButton.getPosition().x + 5, quitButton.getPosition().y);
            }
        }        

        if (isClick) // if have event
        {
            // clear window and draw objects on window of game
            mainWindow->clear(Color::White);
            mainWindow->draw(backgroundSprite);
            mainWindow->draw(titleSprite);
            mainWindow->draw(startButton);
            mainWindow->draw(startButtonText);
            mainWindow->draw(settingButton);
            mainWindow->draw(settingButtonText);
            mainWindow->draw(quitButton);
            mainWindow->draw(quitButtonText);
            mainWindow->draw(chosenSprite);

            if (isSaveMenuOpen) drawSaveMeun();

            if (isOperationMethodOpen || isVolumeSettingOpen) drawSetttingMeun();

            mainWindow->display();
            isClick = false;
        }
        
        if (gameState != 1) return; 
    }
    
    gameState = 0; // close game
}

// Intent:	check event of save menu
// Pre:		None
// Post:	whether boxes of save clicked 
bool DungeonGUI::saveMeunEvent()
{
    // if left mouse pressed
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        // if pressed the button to close save menu
        if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
        {
            isSaveMenuOpen = false; // close save menu
        }

        // if the first of save box is clicked
        if (saveBox1->isClick(event) != "")
        {
            gameState = 2; // set to main game screen
            saveName = saveBox1->getFileName(); // set file name is name of first save file
        }

        // if the second of save box is clicked
        if (saveBox2->isClick(event) != "")
        {
            gameState = 2; // set to main game screen
            saveName = saveBox2->getFileName(); // set file name is name of second save file
        }

        // if the third of save box is clicked
        if (saveBox3->isClick(event) != "")
        {
            gameState = 2; // set to main game screen
            saveName = saveBox3->getFileName(); // set file name is name of third save file
        }

        return true;
    }

    return false;
}

// Intent:	draw the objects of save menu on window of game
// Pre:		None
// Post:	None
void DungeonGUI::drawSaveMeun()
{
    // draw the objects on window of game
    mainWindow->draw(menuSprite);
    mainWindow->draw(backButton);
    mainWindow->draw(backButtonText);
    saveBox1->drawInWindow();
    saveBox2->drawInWindow();
    saveBox3->drawInWindow();
}

// Intent:	screen of main game
// Pre:		None
// Post:	None
void DungeonGUI::gameScreen()
{
    isClick = true;
    backgroundTexture.loadFromFile("./image/market_background.png"); // set image of game background is the image of market background 

    // create and set text of pause title 
    Text pauseText("Pause", *font, 120);
    pauseText.setPosition(mainWindow->getSize().x / 2 - 170, 200);
    pauseText.setFillColor(Color::Black);
    pauseText.setStyle(Text::Bold);

    // create and set the button of continue game
    RectangleShape continueButton(Vector2f(300, 50));
    continueButton.setPosition((mainWindow->getSize().x - continueButton.getSize().x) / 2, (mainWindow->getSize().y + continueButton.getSize().y) / 2 + 50);
    continueButton.setFillColor(Color::Transparent);

    // create and set the button of setting game
    RectangleShape settingButton(Vector2f(300, 50));
    settingButton.setPosition((mainWindow->getSize().x - settingButton.getSize().x) / 2, (mainWindow->getSize().y + settingButton.getSize().y) / 2 + 130);
    settingButton.setFillColor(Color::Transparent);

    // create and set the button of back to main menu
    RectangleShape backHomeButton(Vector2f(300, 50));
    backHomeButton.setPosition((mainWindow->getSize().x - backHomeButton.getSize().x) / 2, (mainWindow->getSize().y + backHomeButton.getSize().y) / 2 + 210);
    backHomeButton.setFillColor(Color::Transparent);

    // create and set the text of continue
    Text continueButtonText("Continue", *font, 40);
    continueButtonText.setPosition(continueButton.getPosition().x + 58, continueButton.getPosition().y);
    continueButtonText.setFillColor(Color::Black);
    continueButtonText.setStyle(Text::Bold);

    // create and set the text of setting
    Text settingButtonText("Setting", *font, 40);
    settingButtonText.setPosition(settingButton.getPosition().x + 80, settingButton.getPosition().y);
    settingButtonText.setFillColor(Color::Black);
    settingButtonText.setStyle(Text::Bold);

    // create and set the text of back to main menu
    Text backHomeButtonText("Back to Menu", *font, 40);
    backHomeButtonText.setPosition(backHomeButton.getPosition().x + 20, backHomeButton.getPosition().y);
    backHomeButtonText.setFillColor(Color::Black);
    backHomeButtonText.setStyle(Text::Bold);
    
    // create a dungeon, and set volume of bgm and sound
    Dungeon dungeon(mainWindow, saveName, font);   
    dungeon.setBGMVolume(bgmVolume);
    dungeon.setSoundVolume(soundVolume);

    // main game loop
    while (mainWindow->isOpen())
    {
        while (mainWindow->pollEvent(event)) // get event from window of game
        {
            if (event.type == Event::Closed) // if click close window, close the window of game
            {
                mainWindow->close();
            }

            // if menu of puase is open and menu of operation method is open or menu of setting volume is open
            if (isPause && (isOperationMethodOpen || isVolumeSettingOpen))
            {
                settingEvent(); // call settingEvent() to check event 
                menuBGM.setVolume(bgmVolume); // set volume of main menu bgm
                dungeon.setBGMVolume(bgmVolume); // set volume of dungeon bgm
                dungeon.setSoundVolume(soundVolume); // set volume of dungeon sound
            }

            // if menu of puase is open and menu of operation method isn't open and menu of setting volume isn't open
            if (event.mouseButton.button == Mouse::Left && isPause && !isOperationMethodOpen && !isVolumeSettingOpen)
            {
                // if mouse clicke the continue button
                if (continueButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    isPause = false; // close the pause menu
                }
                
                // if mouse clicke the setting button
                if (settingButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    isVolumeSettingOpen = true; // open menu of setting volume menu
                }
                
                // if mouse clicke the back to main mwnu button
                if (backHomeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    gameState = 1; // open to main menu
                    isPause = false; // close the pause menu
                }
            }

            // if menu of merchant shop, menu of ironsmith shop, and hero backpack isn't open
            if (!dungeon.isMerchantShopMenuOpen() && !dungeon.isIronsmithShopMenuOpen() && !dungeon.isHeroBackpackOpen())
            {
                // if pressed key ESC and pause time up, can open the menu of pause
                if (Keyboard::isKeyPressed(Keyboard::Escape) && clock() - dungeon.getPauseTime() > 1000)
                {
                    isPause = true; // open menu of pause
                }
            }        

        }

        // if the menu of pause isn't open 
        if (!isPause) dungeon.systemEvent(&event); // call dungeon.systemEvent() to check event

        // clear window and draw objects on window of game
        mainWindow->clear(Color::Black);

        if (dungeon.getNowLevel() == 0) mainWindow->draw(backgroundSprite); // if dungeon current level is 0, draw image of market background

        dungeon.drawInWindow(); // draw objects of dungeon on window of game

        if (isPause)
        {
            mainWindow->draw(pauseBGSprite);
            mainWindow->draw(pauseText);
            mainWindow->draw(continueButton);
            mainWindow->draw(continueButtonText);
            mainWindow->draw(settingButton);
            mainWindow->draw(settingButtonText);
            mainWindow->draw(backHomeButton);
            mainWindow->draw(backHomeButtonText);

            if (isOperationMethodOpen || isVolumeSettingOpen) drawSetttingMeun(); // call drawSetttingMeun(), draw objects of setting 
        }
        mainWindow->display();

        if (gameState != 2) return;
    }
}

// Intent:	check event of setting menu
// Pre:		None
// Post:	None
void DungeonGUI::settingEvent()
{
    // if pressed key ESC
    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        if (isOperationMethodOpen || isVolumeSettingOpen)
        {
            isOperationMethodOpen = false; // close menu of operation method
            isVolumeSettingOpen = false; // close menu of setting volume
        }
      
        return;
    }

    // if left mouse click 
    if (event.mouseButton.button == Mouse::Left)
    {
        // if clicked the button of open menu of operation method
        if (operationMethodButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
        {
            isOperationMethodOpen = true; // open menu of operation method
            isVolumeSettingOpen = false; // close menu of setting volume
            return;
        }
        
        // if clicked the button of open menu of setting volume
        if (volumeSettingButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
        {                      
            isOperationMethodOpen = false; // close menu of operation method
            isVolumeSettingOpen = true; // open menu of setting volume
            return;
        }     
    }

    // if original x of the mouse position is -1, and left mouse pressed
    if (mouseX == -1 && Mouse::isButtonPressed(Mouse::Left) && isVolumeSettingOpen)
    { 
        if (bgmVolumeBallSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // if left mouse pressed on bgm volume ball
        {
            mouseX = Mouse::getPosition().x; // save the x of the mouse position
            isSettingBGM = true;
        }
        else if (soundVolumeBallSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // if left mouse pressed on sound volume ball
        {
            mouseX = Mouse::getPosition().x; // save the x of the mouse position
            isSettingSound = true;
        }
    }
    
    // if left mouse released
    if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
    {
        mouseX = -1; // set the x of the mouse position is -1
        isSettingBGM = isSettingSound = false;
    }

    // if original x of the mouse position isn't -1, and left mouse pressed and moving
    if (mouseX != -1 && event.type == Event::MouseMoved && Mouse::isButtonPressed(Mouse::Left))
    {
        float deltaX = Mouse::getPosition().x - mouseX; // calculate difference original x of mouse position and current x of mouse position
        Sprite sprite; // sprite to simulate

        if (isSettingBGM)
        {
            sprite = bgmVolumeBallSprite; // give sprite of bgm volume ball to sprite to simulate

            // move sprite to simulate
            if (deltaX < 0) sprite.move(deltaX - 42.5, 0); 
            else sprite.move(deltaX + 40, 0);

            // if move sprite to simulate isn't beyond the range of the bgm volume line
            if (sprite.getGlobalBounds().intersects(bgmVolumeLineSprite.getGlobalBounds()))
            {
                bgmVolumeBallSprite.move(deltaX, 0); // move sprite of bgm volume ball

                float volumeNume = bgmVolumeLineSprite.getPosition().x; // set numerator is x of bgm volume line position
                float volumeDemo = bgmVolumeLineSprite.getPosition().x + bgmVolumeLineSprite.getLocalBounds().width; // set denominator is x of bgm volume line position plus width

                // calculate the value of bgm volume
                bgmVolume = 100.0 / (volumeDemo - volumeNume) * ((bgmVolumeBallSprite.getPosition().x + bgmVolumeBallTexture.getSize().x / 2) - volumeNume);
                bgmVolumeText.setString(to_string(bgmVolume)); // set text of bgm volume
                menuBGM.setVolume(bgmVolume); // set volume of main menu bgm
            }
        }
        else if (isSettingSound)
        {
            sprite = soundVolumeBallSprite; // give sprite of sound volume ball to sprite to simulate

            // move sprite to simulate
            if (deltaX < 0) sprite.move(deltaX - 42.5, 0);
            else sprite.move(deltaX + 40, 0);

            // if move sprite to simulate isn't beyond the range of the sound volume line
            if (sprite.getGlobalBounds().intersects(soundVolumeLineSprite.getGlobalBounds()))
            {
                soundVolumeBallSprite.move(deltaX, 0); // move sprite of sound volume ball

                float volumeNume = soundVolumeLineSprite.getPosition().x; // set numerator is x of sound volume line position
                float volumeDemo = soundVolumeLineSprite.getPosition().x + soundVolumeLineSprite.getLocalBounds().width; // set denominator is x of sound volume line position plus width

                // calculate the value of sound volume
                soundVolume = 100.0 / (volumeDemo - volumeNume) * ((soundVolumeBallSprite.getPosition().x + soundVolumeBallTexture.getSize().x / 2) - volumeNume);
                soundVolumeText.setString(to_string(soundVolume)); // set text of sound volume
            }
        }
        
        mouseX = Mouse::getPosition().x; // set original x of the mouse position is current x of the mouse position
    }
}

// Intent:	draw the objects of setting menu on window of game
// Pre:		None
// Post:	None
void DungeonGUI::drawSetttingMeun()
{
    // draw the objects on window of game
    if (isOperationMethodOpen)
    {
        mainWindow->draw(operationMethodSprite);
    }
    else if (isVolumeSettingOpen)
    {
        mainWindow->draw(volumeSettingSprite);
        mainWindow->draw(bgmVolumeLineSprite);
        mainWindow->draw(bgmVolumeBallSprite);
        mainWindow->draw(bgmVolumeText);
        mainWindow->draw(soundVolumeLineSprite);
        mainWindow->draw(soundVolumeBallSprite);
        mainWindow->draw(soundVolumeText);
    }

    mainWindow->draw(operationMethodButton);
    mainWindow->draw(volumeSettingButton);
}

// Intent:	executive the gui of dungeon
// Pre:		None
// Post:	None
void DungeonGUI::exe()
{
    clock_t gameMainClock = clock();
    do
    {
        if (clock() - gameMainClock >= 10)
        {
            switch (gameState)
            {
            case 1: // if gameState is 1, call homeScreen() to show main menu
                homeScreen();
                break;
            case 2: // if gameState is 2, call gameScreen() to show main game
                gameScreen();
                break;
            }

            gameMainClock = clock();
        }       
    } while (gameState != 0); // if gameState is 0, close game
}
