#include <iostream>
#include <print>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


/*
===============================================================================
REFERENCES
===============================================================================
* https://www.sfml-dev.org/tutorials/3.0/graphics/text/
* https://www.sfml-dev.org/tutorials/3.0/window/inputs/
* https://www.sfml-dev.org/tutorials/3.0/getting-started/visual-studio/#creating-and-configuring-an-sfml-project
* https://www.sfml-dev.org/tutorials/3.0/system/angle/#angles-in-sfml
* https://www.sfml-dev.org/tutorials/3.0/graphics/sprite/#loading-a-texture
* https://www.sfml-dev.org/tutorials/3.0/graphics/view/
*
* https://stackoverflow.com/questions/26857901/sfml-atan2-function-and-deceleration in order to understand how atan2 func works
*
* https://www.youtube.com/watch?v=cWgox89PZro&list=PLz6j8tWRKzOHQPOL5gGY4Ev6DoMRB2gee&index=2
* https://www.youtube.com/watch?v=vz1jGgSVsUQ
* https://www.youtube.com/watch?v=ulct06sYvoM
* https://www.youtube.com/watch?v=LZXJEF39fm8
* Font: Roboto Mono by Google Fonts (https://fonts.google.com/specimen/Roboto+Mono)
===============================================================================
*/


void checkCollision(sf::Sprite& policeCar, sf::Sprite& mine, sf::Sprite& suspect, float windowWidth, float windowHeight, std::vector<sf::Vector2f>& mineVectors, bool& gameOver, std::vector<sf::Vector2f>& suspectVectors, int& score, sf::Sound& boomEffect, sf::Sound& handcuffEffect)
{
	if (policeCar.getGlobalBounds().getCenter().x > windowWidth - (policeCar.getGlobalBounds().size.x / 2.0f))
	{
		policeCar.setPosition({ windowWidth - (policeCar.getGlobalBounds().size.x / 2.0f), policeCar.getGlobalBounds().getCenter().y });
	}
	if (policeCar.getGlobalBounds().getCenter().x < policeCar.getGlobalBounds().size.x / 2.0f)
	{
		policeCar.setPosition({ policeCar.getGlobalBounds().size.x / 2.0f, policeCar.getGlobalBounds().getCenter().y });
	}
	if (policeCar.getGlobalBounds().getCenter().y > windowHeight - (policeCar.getGlobalBounds().size.y / 2.0f))
	{
		policeCar.setPosition({ policeCar.getGlobalBounds().getCenter().x, windowHeight - (policeCar.getGlobalBounds().size.y / 2.0f) });
	}
	if (policeCar.getGlobalBounds().getCenter().y < (policeCar.getGlobalBounds().size.y / 2.0f))
	{
		policeCar.setPosition({ policeCar.getGlobalBounds().getCenter().x, (policeCar.getGlobalBounds().size.y / 2.0f) });
	}


	for (int i = 0; i < mineVectors.size(); i++)
	{
		if (std::abs(policeCar.getGlobalBounds().getCenter().x - mineVectors[i].x) <= ((policeCar.getGlobalBounds().size.x / 2.0f) + (mine.getGlobalBounds().size.x / 2.0f)) && std::abs(policeCar.getGlobalBounds().getCenter().y - mineVectors[i].y) <= ((policeCar.getGlobalBounds().size.y / 2.0f) + (mine.getGlobalBounds().size.y / 2.0f)))
		{
			gameOver = true;
			boomEffect.play();
			return;
		}
	}

	for (int j = 0; j < suspectVectors.size(); j++)
	{
		if (std::abs(policeCar.getGlobalBounds().getCenter().x - suspectVectors[j].x) <= ((policeCar.getGlobalBounds().size.x / 2.0f) + (suspect.getGlobalBounds().size.x / 2.0f)) && std::abs(policeCar.getGlobalBounds().getCenter().y - suspectVectors[j].y) <= ((policeCar.getGlobalBounds().size.y / 2.0f) + (suspect.getGlobalBounds().size.y / 2.0f)))
		{
			score += 1;
			handcuffEffect.play();
			suspectVectors[j] = { -1,-1 }; // in drawSuspect function this spesific vector ignored to reDraw
		}
	}
}

float handleAccelerationX(float deltaTime, float currentAccelerationX)
{

	constexpr float accelerationSlownessX = 1000.0f; // higher value make acceleration change slower

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			currentAccelerationX -= deltaTime / accelerationSlownessX;

			if (currentAccelerationX < -1)
			{
				currentAccelerationX = -1;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			currentAccelerationX += deltaTime / accelerationSlownessX;

			if (currentAccelerationX > 1)
			{
				currentAccelerationX = 1;
			}
		}
	}
	else
	{

		if (currentAccelerationX == 0)
		{
			return currentAccelerationX;
		}
		else if (currentAccelerationX > 0)
		{
			currentAccelerationX -= deltaTime / accelerationSlownessX;

			if (currentAccelerationX < 0)
			{
				currentAccelerationX = 0;
			}
		}
		else if (currentAccelerationX < 0)
		{
			currentAccelerationX += deltaTime / accelerationSlownessX;

			if (currentAccelerationX > 0)
			{
				currentAccelerationX = 0;
			}
		}
	}

	return currentAccelerationX;
}

float handleAccelerationY(float deltaTime, float currentAccelerationY)
{

	constexpr float accelerationSlownessY = 1000.0f; // higher value make acceleration change slower

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			currentAccelerationY -= deltaTime / accelerationSlownessY;

			if (currentAccelerationY < -1)
			{
				currentAccelerationY = -1;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			currentAccelerationY += deltaTime / accelerationSlownessY;

			if (currentAccelerationY > 1)
			{
				currentAccelerationY = 1;
			}
		}
	}
	else
	{
		if (currentAccelerationY == 0)
		{
			return currentAccelerationY;
		}
		else if (currentAccelerationY > 0)
		{
			currentAccelerationY -= deltaTime / accelerationSlownessY;

			if (currentAccelerationY < 0)
			{
				currentAccelerationY = 0;
			}
		}
		else if (currentAccelerationY < 0)
		{
			currentAccelerationY += deltaTime / accelerationSlownessY;

			if (currentAccelerationY > 0)
			{
				currentAccelerationY = 0;
			}
		}
	}

	return currentAccelerationY;
}

sf::IntRect updateAnimationFrames(sf::Sprite& policeCar, float deltaTimeMs, sf::Vector2f Acceleration, unsigned& stateIndex, unsigned& frameIndex, float& animationProgressMs)
{

	std::vector<sf::IntRect> idleFrames;
	for (int i = 0; i <= 7; i++)
	{
		idleFrames.push_back(sf::IntRect({ i * 384, 0 }, { 384, 192 }));
	}

	std::vector<sf::IntRect> walkFrames;
	for (int i = 0; i <= 7; i++)
	{
		walkFrames.push_back(sf::IntRect({ i * 384, 192 }, { 384, 192 }));
	}

	constexpr float animationDurationMs = 100.0f;

	if (Acceleration.x > 0.1f || Acceleration.x < -0.1f || Acceleration.y > 0.1f || Acceleration.y < -0.1f)
	{
		stateIndex = 1;
	}
	else
	{
		stateIndex = 0;
	}

	if (Acceleration.x > 0.1f)
	{
		policeCar.setScale({ 0.5f, 0.5f });
	}
	else if (Acceleration.x < -0.1f)
	{
		policeCar.setScale({ -0.5f, 0.5f });
	}

	std::vector<std::vector<sf::IntRect>> currentState;
	currentState.push_back(idleFrames);
	currentState.push_back(walkFrames);


	animationProgressMs += deltaTimeMs;

	if (animationProgressMs >= animationDurationMs)
	{
		animationProgressMs = 0.0f;

		if (frameIndex >= currentState[stateIndex].size() - 1)
		{
			frameIndex = 0;
		}
		else
		{
			++frameIndex;
		}
	}

	return currentState[stateIndex][frameIndex];
}

bool isPlaceEmptyForMine(sf::Sprite& mine, sf::Vector2f newMineVector, std::vector<sf::Vector2f> mineVectors, sf::RenderWindow& window)
{

	int mineSpriteSizePxX = mine.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mine.getGlobalBounds().size.y;

	if (newMineVector.x > (window.getSize().x - mineSpriteSizePxX) || newMineVector.x < mineSpriteSizePxX || newMineVector.y >(window.getSize().y - mineSpriteSizePxY) || newMineVector.y < mineSpriteSizePxY)
	{
		return false;
	}

	for (int i = 0; i < mineVectors.size(); i++)
	{
		if (abs(mineVectors[i].x - newMineVector.x) < mineSpriteSizePxX && abs(mineVectors[i].y - newMineVector.y) < mineSpriteSizePxY)
		{
			return false;
		}
	}

	return true;
}

bool isPlaceEmptyForSuspect(sf::Sprite& suspect, sf::Sprite mine, sf::Vector2f newSuspectVector, std::vector<sf::Vector2f> suspectVectors, std::vector<sf::Vector2f> mineVectors, sf::RenderWindow& window)
{

	int suspectSpriteSizePxX = suspect.getGlobalBounds().size.x;
	int suspectSpriteSizePxY = suspect.getGlobalBounds().size.y;

	int mineSpriteSizePxX = mine.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mine.getGlobalBounds().size.y;

	if (newSuspectVector.x > (window.getSize().x - suspectSpriteSizePxX) || newSuspectVector.x < suspectSpriteSizePxX || newSuspectVector.y >(window.getSize().y - suspectSpriteSizePxY) || newSuspectVector.y < suspectSpriteSizePxY)
	{
		return false;
	}

	for (int i = 0; i < suspectVectors.size(); i++)
	{
		if (abs(suspectVectors[i].x - newSuspectVector.x) < suspectSpriteSizePxX && abs(suspectVectors[i].y - newSuspectVector.y) < suspectSpriteSizePxY)
		{
			return false;
		}
	}

	for (int i = 0; i <= suspectVectors.size(); i++)
	{
		for (int j = 0; j < mineVectors.size(); j++)
		{
			if (abs(mineVectors[j].x - newSuspectVector.x) < ((suspectSpriteSizePxX / 2.0f) + (mineSpriteSizePxX / 2.0f)) && abs(mineVectors[j].y - newSuspectVector.y) < ((suspectSpriteSizePxY / 2.0f) + (mineSpriteSizePxY / 2.0f)))
			{
				return false;
			}
		}
	}

	return true;
}

bool isPlaceEmptyForPolice(sf::Sprite& policeCar, sf::Sprite& suspect, sf::Sprite mine, sf::Vector2f newPoliceVector, std::vector<sf::Vector2f> suspectVectors, std::vector<sf::Vector2f> mineVectors, sf::RenderWindow& window)
{
	int suspectSpriteSizePxX = suspect.getGlobalBounds().size.x;
	int suspectSpriteSizePxY = suspect.getGlobalBounds().size.y;

	int mineSpriteSizePxX = mine.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mine.getGlobalBounds().size.y;

	int policeCarSpriteSizePxX = suspect.getGlobalBounds().size.x;
	int policeCarSpriteSizePxY = suspect.getGlobalBounds().size.y;

	if (newPoliceVector.x > (window.getSize().x - policeCarSpriteSizePxX) || newPoliceVector.x < policeCarSpriteSizePxX || newPoliceVector.y >(window.getSize().y - policeCarSpriteSizePxY) || newPoliceVector.y < policeCarSpriteSizePxY)
	{
		return false;
	}

	for (int i = 0; i < mineVectors.size(); i++)
	{
		if (abs(mineVectors[i].x - newPoliceVector.x) < ((mineSpriteSizePxX / 2.0f) + (policeCarSpriteSizePxX / 2.0f)) && abs(mineVectors[i].y - newPoliceVector.y) < ((mineSpriteSizePxY / 2.0f) + (policeCarSpriteSizePxY / 2.0f)))
		{
			return false;
		}
	}

	for (int i = 0; i < suspectVectors.size(); i++)
	{
		if (abs(suspectVectors[i].x - newPoliceVector.x) < ((suspectSpriteSizePxX / 2.0f) + (policeCarSpriteSizePxX / 2.0f)) && abs(suspectVectors[i].y - newPoliceVector.y) < ((suspectSpriteSizePxY / 2.0f) + (policeCarSpriteSizePxY / 2.0f)))
		{
			return false;
		}
	}

	return true;
}

void drawMine(sf::Sprite mine, int mineCount, sf::RenderWindow& window, bool& minesSpawned, std::vector<sf::Vector2f>& mineVectors)
{
	if (!minesSpawned)
	{
		for (int i = 0; i < mineCount; i++)
		{
			sf::Vector2f newMineVector = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForMine(mine, newMineVector, mineVectors, window))
			{
				mineVectors.push_back(newMineVector);
			}
			else
			{
				i--;
			}
		}
	}

	for (int temp = 0; temp < mineVectors.size(); temp++)
	{
		mine.setPosition(mineVectors[temp]);
		window.draw(mine);
	}

	minesSpawned = true;
}

void drawSuspect(sf::Sprite suspect, sf::Sprite mine, int& suspectCount, int& currentSuspectCount, sf::RenderWindow& window, bool& suspectsSpawned, std::vector<sf::Vector2f>& suspectVectors, std::vector<sf::Vector2f>& mineVectors)
{

	currentSuspectCount = 0;

	if (!suspectsSpawned)
	{
		for (int i = 0; i < suspectCount; i++)
		{
			sf::Vector2f newSuspectVector = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForSuspect(suspect, mine, newSuspectVector, suspectVectors, mineVectors, window))
			{
				suspectVectors.push_back(newSuspectVector);
			}
			else
			{
				i--;
			}
		}
	}

	for (int temp = 0; temp < suspectVectors.size(); temp++)
	{
		suspect.setTextureRect(sf::IntRect({ 384 * temp, 0 }, { 384, 192 }));
		suspect.setPosition(suspectVectors[temp]);

		if (suspectVectors[temp] != sf::Vector2f{ -1, -1 }) //suspectCar's location changes to -1,-1 if the policeCar collide with an suspectCar so before we drawing we check if car should be redraw again or stay destroyed 
		{
			window.draw(suspect);
			currentSuspectCount++;
		}
	}

	suspectsSpawned = true;
}

void drawPolice(sf::Sprite& policeCar, sf::Sprite suspect, sf::Sprite mine, const int suspectCount, sf::RenderWindow& window, bool& policeSpawned, std::vector<sf::Vector2f>& suspectVectors, std::vector<sf::Vector2f>& mineVectors, sf::Vector2f& newPoliceVector)
{
	if (!policeSpawned)
	{
		for (int i = 0; i < 1; i++)
		{
			newPoliceVector = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForPolice(policeCar, suspect, mine, newPoliceVector, suspectVectors, mineVectors, window))
			{
				policeCar.setPosition(newPoliceVector);
				window.draw(policeCar);
			}
			else
			{
				i--;
			}
		}
	}

	window.draw(policeCar);

	policeSpawned = true;
}

void resetGame(int& mineCount, int& currentSuspectCount, bool& minesSpawned, bool& suspectsSpawned, bool& policeSpawned, bool& gameOver, int& score, std::vector<sf::Vector2f>& suspectVectors, std::vector<sf::Vector2f>& mineVectors, sf::Vector2f& acceleration)
{

	if (currentSuspectCount == 0)
	{
		minesSpawned = false;
		suspectsSpawned = false;
		policeSpawned = false;
		suspectVectors.clear();
		mineVectors.clear();
		if (mineCount < 25) { mineCount += 1; }
		acceleration = { 0.0f,0.0f };
	}


	if (gameOver)
	{
		minesSpawned = false;
		suspectsSpawned = false;
		policeSpawned = false;
		suspectVectors.clear();
		mineVectors.clear();
		score = 0;
		mineCount = 10;

		acceleration = { 0.0f,0.0f };

		gameOver = false;
	}


}

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "Shapes");

	float windowWidth = window.getSize().x;
	float windowHeight = window.getSize().y;
	sf::Clock clock;

	srand(time(0));
	int mineCount = 10;
	bool minesSpawned = false;
	std::vector<sf::Vector2f> mineVectors;

	bool gameOver = false;

	int suspectCount = 5; // sprite sheet have only 5 different car
	int currentSuspectCount = 0; // number of suspect on the screen
	bool suspectsSpawned = false;
	std::vector<sf::Vector2f> suspectVectors;

	int score = 0;


	sf::Vector2f newPoliceVector;
	bool policeSpawned = false;

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile(ASSETS_PATH "images/background.png"))
	{
		std::println("background.jpg loadfromfile error");
	}
	sf::Texture policeCarTexture;
	if (!policeCarTexture.loadFromFile(ASSETS_PATH "images/policeCarSpriteSheet.png"))
	{
		std::println("carSprite.png loadfromfile error");
	}
	sf::Texture mineTexture;
	if (!mineTexture.loadFromFile(ASSETS_PATH "images/mine.png"))
	{
		std::println("mine.png loadfromfile error");
	}
	sf::Texture suspectCarsTexture;
	if (!suspectCarsTexture.loadFromFile(ASSETS_PATH "images/suspectCarsSpriteSheet.png"))
	{
		std::println("suspectCarsSpriteSheet.png loadfromfile error");
	}

	sf::Sprite background(backgroundTexture);
	background.setScale({ windowWidth / backgroundTexture.getSize().x,windowHeight / backgroundTexture.getSize().y });

	sf::Sprite mine(mineTexture);
	mine.setTextureRect(sf::IntRect({ 0, 0 }, { 96, 96 }));
	mine.setScale({ 0.5f,0.5f });
	mine.setOrigin(mine.getLocalBounds().getCenter());

	sf::Sprite policeCar(policeCarTexture);
	policeCar.setTextureRect(sf::IntRect({ 0, 0 }, { 384, 192 }));
	policeCar.setScale({ 0.5f,0.5f });
	policeCar.setOrigin(policeCar.getLocalBounds().getCenter());
	//policeCar.setPosition({ windowWidth / 2.f, windowHeight / 2.f });

	sf::Sprite suspectCars(suspectCarsTexture);
	suspectCars.setTextureRect(sf::IntRect({ 0, 0 }, { 384, 192 }));
	suspectCars.setScale({ 0.5f,0.5f });
	suspectCars.setOrigin(suspectCars.getLocalBounds().getCenter());

	sf::SoundBuffer boomBuffer;
	if (!boomBuffer.loadFromFile(ASSETS_PATH "sounds/boomEffect.mp3"))
	{
		std::println("boomEffect.mp3 loadfromfile error");
	}
	sf::SoundBuffer handcuffBuffer;
	if (!handcuffBuffer.loadFromFile(ASSETS_PATH "sounds/handcuffEffect.mp3"))
	{
		std::println("handcuffEffect.mp3 loadfromfile error");
	}

	sf::Sound boomEffect(boomBuffer);
	boomEffect.setVolume(2000.0f);
	sf::Sound handcuffEffect(handcuffBuffer);
	handcuffEffect.setVolume(200.0f);

	sf::Music backgroundMusic;
	if (!backgroundMusic.openFromFile(ASSETS_PATH "sounds/backgroundMusic.mp3"))
	{
		std::println("backgroundMusic.mp3 openfromfile error");
	}

	backgroundMusic.play();
	backgroundMusic.setLooping(true);

	sf::Font font;
	if (!font.openFromFile(ASSETS_PATH "fonts/RobotoMono-Regular.ttf"))
	{
		std::println("RobotoMono-Regular.ttf openfromfile error");
	}
	sf::Text text(font);
	text.setString("--- CONTROLS --- \n\n"
		"     [^]       \n"
		"[<] [v] [>]    : Move Character\n"
	);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color(172, 50, 50));

	sf::Text scoreText(font);
	scoreText.setString(std::to_string(0));
	scoreText.setCharacterSize(365);
	scoreText.setFillColor(sf::Color(0, 50, 100, 50));
	scoreText.setOrigin(scoreText.getLocalBounds().getCenter());
	scoreText.setPosition({ windowWidth / 2.f, windowHeight / 2.f });



	sf::Vector2f acceleration = { 0 , 0 };

	unsigned int stateIndex = 0;
	unsigned int frameIndex = 0;
	float animationProgressMs = 0.0f;

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}


		float deltaTimeMs = clock.restart().asSeconds() * 1000;
		float speedPx = 0.3f;
		float displacementPx = speedPx * deltaTimeMs;

		acceleration = { handleAccelerationX(deltaTimeMs,acceleration.x),handleAccelerationY(deltaTimeMs, acceleration.y) };
		//std::print("x = {}  -  y = {}  ---- gameOver = {} ---- score = {}\n", acceleration.x, acceleration.y, gameOver, score);
		policeCar.move({ acceleration.x * displacementPx , acceleration.y * displacementPx });

		checkCollision(policeCar, mine, suspectCars, windowWidth, windowHeight, mineVectors, gameOver, suspectVectors, score, boomEffect, handcuffEffect);

		policeCar.setTextureRect((updateAnimationFrames(policeCar, deltaTimeMs, acceleration, stateIndex, frameIndex, animationProgressMs)));

		resetGame(mineCount, currentSuspectCount, minesSpawned, suspectsSpawned, policeSpawned, gameOver, score, suspectVectors, mineVectors, acceleration);

		window.clear();
		window.draw(background);
		window.draw(text);
		scoreText.setString(std::to_string(score));
		window.draw(scoreText);
		drawMine(mine, mineCount, window, minesSpawned, mineVectors);
		drawSuspect(suspectCars, mine, suspectCount, currentSuspectCount, window, suspectsSpawned, suspectVectors, mineVectors);
		drawPolice(policeCar, suspectCars, mine, suspectCount, window, policeSpawned, suspectVectors, mineVectors, newPoliceVector);
		window.display();

	}


	return 0;
}