#include <iostream>
#include <print>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

struct entity {
	sf::Sprite entitySprite;
	sf::Vector2f entityVector;
};

struct policeCar {
	sf::Sprite policeCarSprite;
	sf::Vector2f policeCarVector;
};

struct spawnControl
{
	bool mineSpawned = false;
	bool suspectSpawned = false;
	bool policeSpawned = false;
};

void handleWindowCollision(policeCar& police, sf::RenderWindow& window)
{
	float windowWidth = window.getSize().x;
	float windowHeight = window.getSize().y;

	if (police.policeCarSprite.getGlobalBounds().getCenter().x > windowWidth - (police.policeCarSprite.getGlobalBounds().size.x / 2.0f))
	{
		police.policeCarSprite.setPosition({ windowWidth - (police.policeCarSprite.getGlobalBounds().size.x / 2.0f), police.policeCarSprite.getGlobalBounds().getCenter().y });
	}
	if (police.policeCarSprite.getGlobalBounds().getCenter().x < police.policeCarSprite.getGlobalBounds().size.x / 2.0f)
	{
		police.policeCarSprite.setPosition({ police.policeCarSprite.getGlobalBounds().size.x / 2.0f, police.policeCarSprite.getGlobalBounds().getCenter().y });
	}
	if (police.policeCarSprite.getGlobalBounds().getCenter().y > windowHeight - (police.policeCarSprite.getGlobalBounds().size.y / 2.0f))
	{
		police.policeCarSprite.setPosition({ police.policeCarSprite.getGlobalBounds().getCenter().x, windowHeight - (police.policeCarSprite.getGlobalBounds().size.y / 2.0f) });
	}
	if (police.policeCarSprite.getGlobalBounds().getCenter().y < (police.policeCarSprite.getGlobalBounds().size.y / 2.0f))
	{
		police.policeCarSprite.setPosition({ police.policeCarSprite.getGlobalBounds().getCenter().x, (police.policeCarSprite.getGlobalBounds().size.y / 2.0f) });
	}
}

bool checkMineCollision(policeCar& police, std::vector<entity>& mine)
{
	for (int i = 0; i < mine.size(); i++)
	{
		if (std::abs(police.policeCarSprite.getGlobalBounds().getCenter().x - mine[i].entityVector.x) <= ((police.policeCarSprite.getGlobalBounds().size.x / 2.0f) + (mine[i].entitySprite.getGlobalBounds().size.x / 2.0f)) && std::abs(police.policeCarSprite.getGlobalBounds().getCenter().y - mine[i].entityVector.y) <= ((police.policeCarSprite.getGlobalBounds().size.y / 2.0f) + (mine[i].entitySprite.getGlobalBounds().size.y / 2.0f)))
		{
			return true;
		}
	}
	return false;
}

bool checkSuspectCollision(policeCar& police, std::vector<entity>& suspect)
{
	for (int j = 0; j < suspect.size(); j++)
	{
		if (std::abs(police.policeCarSprite.getGlobalBounds().getCenter().x - suspect[j].entityVector.x) <= ((police.policeCarSprite.getGlobalBounds().size.x / 2.0f) + (suspect[j].entitySprite.getGlobalBounds().size.x / 2.0f)) && std::abs(police.policeCarSprite.getGlobalBounds().getCenter().y - suspect[j].entityVector.y) <= ((police.policeCarSprite.getGlobalBounds().size.y / 2.0f) + (suspect[j].entitySprite.getGlobalBounds().size.y / 2.0f)))
		{
			suspect[j].entityVector = { -1,-1 }; // in drawSuspect function this spesific vector ignored to reDraw
			return true;
		}
	}
	return false;
}

//void checkCollision(sf::Sprite& policeCar, sf::Sprite& mine, sf::Sprite& suspect, float windowWidth, float windowHeight, std::vector<sf::Vector2f>& mineVectors, bool& gameOver, std::vector<sf::Vector2f>& suspectVectors, int& score, sf::Sound& boomEffect, sf::Sound& handcuffEffect)
//{
//	if (policeCar.getGlobalBounds().getCenter().x > windowWidth - (policeCar.getGlobalBounds().size.x / 2.0f))
//	{
//		policeCar.setPosition({ windowWidth - (policeCar.getGlobalBounds().size.x / 2.0f), policeCar.getGlobalBounds().getCenter().y });
//	}
//	if (policeCar.getGlobalBounds().getCenter().x < policeCar.getGlobalBounds().size.x / 2.0f)
//	{
//		policeCar.setPosition({ policeCar.getGlobalBounds().size.x / 2.0f, policeCar.getGlobalBounds().getCenter().y });
//	}
//	if (policeCar.getGlobalBounds().getCenter().y > windowHeight - (policeCar.getGlobalBounds().size.y / 2.0f))
//	{
//		policeCar.setPosition({ policeCar.getGlobalBounds().getCenter().x, windowHeight - (policeCar.getGlobalBounds().size.y / 2.0f) });
//	}
//	if (policeCar.getGlobalBounds().getCenter().y < (policeCar.getGlobalBounds().size.y / 2.0f))
//	{
//		policeCar.setPosition({ policeCar.getGlobalBounds().getCenter().x, (policeCar.getGlobalBounds().size.y / 2.0f) });
//	}
//
//
//	for (int i = 0; i < mineVectors.size(); i++)
//	{
//		if (std::abs(policeCar.getGlobalBounds().getCenter().x - mineVectors[i].x) <= ((policeCar.getGlobalBounds().size.x / 2.0f) + (mine.getGlobalBounds().size.x / 2.0f)) && std::abs(policeCar.getGlobalBounds().getCenter().y - mineVectors[i].y) <= ((policeCar.getGlobalBounds().size.y / 2.0f) + (mine.getGlobalBounds().size.y / 2.0f)))
//		{
//			gameOver = true;
//			boomEffect.play();
//			return;
//		}
//	}
//
//	for (int j = 0; j < suspectVectors.size(); j++)
//	{
//		if (std::abs(policeCar.getGlobalBounds().getCenter().x - suspectVectors[j].x) <= ((policeCar.getGlobalBounds().size.x / 2.0f) + (suspect.getGlobalBounds().size.x / 2.0f)) && std::abs(policeCar.getGlobalBounds().getCenter().y - suspectVectors[j].y) <= ((policeCar.getGlobalBounds().size.y / 2.0f) + (suspect.getGlobalBounds().size.y / 2.0f)))
//		{
//			score += 1;
//			handcuffEffect.play();
//			suspectVectors[j] = { -1,-1 }; // in drawSuspect function this spesific vector ignored to reDraw
//		}
//	}
//}

float handleAccelerationX(float deltaTime, float currentAccelerationX)
{

	constexpr float accelerationSlownessX = 1000.0f; // higher value make acceleration change slower

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			currentAccelerationX -= deltaTime / (accelerationSlownessX * 0.5);

			if (currentAccelerationX < -1)
			{
				currentAccelerationX = -1;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			currentAccelerationX += deltaTime / (accelerationSlownessX * 0.5);

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
			currentAccelerationY -= deltaTime / (accelerationSlownessY * 0.5);

			if (currentAccelerationY < -1)
			{
				currentAccelerationY = -1;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			currentAccelerationY += deltaTime / (accelerationSlownessY * 0.5);

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

sf::IntRect updateAnimationFrames(policeCar& police, float deltaTimeMs, sf::Vector2f Acceleration, unsigned& stateIndex, unsigned& frameIndex, float& animationProgressMs)
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
		police.policeCarSprite.setScale({ 0.5f, 0.5f });
	}
	else if (Acceleration.x < -0.1f)
	{
		police.policeCarSprite.setScale({ -0.5f, 0.5f });
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

bool isPlaceEmptyForMine(sf::Vector2f newMineVector, std::vector<entity>& mine, sf::Sprite mineSprite, sf::RenderWindow& window)
{

	int mineSpriteSizePxX = mineSprite.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mineSprite.getGlobalBounds().size.y;

	if (newMineVector.x > (window.getSize().x - mineSpriteSizePxX) || newMineVector.x < mineSpriteSizePxX || newMineVector.y >(window.getSize().y - mineSpriteSizePxY) || newMineVector.y < mineSpriteSizePxY)
	{
		return false;
	}

	for (int i = 0; i < mine.size(); i++)
	{
		if (abs(mine[i].entityVector.x - newMineVector.x) < mineSpriteSizePxX && abs(mine[i].entityVector.y - newMineVector.y) < mineSpriteSizePxY)
		{
			return false;
		}
	}

	return true;
}

bool isPlaceEmptyForSuspect(sf::Vector2f newSuspectVector, std::vector<entity>& suspect, sf::Sprite suspectCarSprite, std::vector<entity>& mine, sf::RenderWindow& window)
{

	int suspectSpriteSizePxX = suspectCarSprite.getGlobalBounds().size.x;
	int suspectSpriteSizePxY = suspectCarSprite.getGlobalBounds().size.y;

	int mineSpriteSizePxX = mine[0].entitySprite.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mine[0].entitySprite.getGlobalBounds().size.y;

	if (newSuspectVector.x > (window.getSize().x - suspectSpriteSizePxX) || newSuspectVector.x < suspectSpriteSizePxX || newSuspectVector.y >(window.getSize().y - suspectSpriteSizePxY) || newSuspectVector.y < suspectSpriteSizePxY)
	{
		return false;
	}

	for (int i = 0; i < suspect.size(); i++)
	{
		if (abs(suspect[i].entityVector.x - newSuspectVector.x) < suspectSpriteSizePxX && abs(suspect[i].entityVector.y - newSuspectVector.y) < suspectSpriteSizePxY)
		{
			return false;
		}
	}

	for (int i = 0; i <= suspect.size(); i++)
	{
		for (int j = 0; j < mine.size(); j++)
		{
			if (abs(mine[j].entityVector.x - newSuspectVector.x) < ((suspectSpriteSizePxX / 2.0f) + (mineSpriteSizePxX / 2.0f)) && abs(mine[j].entityVector.y - newSuspectVector.y) < ((suspectSpriteSizePxY / 2.0f) + (mineSpriteSizePxY / 2.0f)))
			{
				return false;
			}
		}
	}

	return true;
}

bool isPlaceEmptyForPolice(sf::Vector2f newPoliceVector, policeCar& police, std::vector<entity>& suspect, std::vector<entity>& mine, sf::RenderWindow& window)
{
	int suspectSpriteSizePxX = suspect[0].entitySprite.getGlobalBounds().size.x;
	int suspectSpriteSizePxY = suspect[0].entitySprite.getGlobalBounds().size.y;

	int mineSpriteSizePxX = mine[0].entitySprite.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mine[0].entitySprite.getGlobalBounds().size.y;

	int policeCarSpriteSizePxX = police.policeCarSprite.getGlobalBounds().size.x;
	int policeCarSpriteSizePxY = police.policeCarSprite.getGlobalBounds().size.y;

	if (newPoliceVector.x > (window.getSize().x - policeCarSpriteSizePxX) || newPoliceVector.x < policeCarSpriteSizePxX || newPoliceVector.y >(window.getSize().y - policeCarSpriteSizePxY) || newPoliceVector.y < policeCarSpriteSizePxY)
	{
		return false;
	}

	for (int i = 0; i < mine.size(); i++)
	{
		if (abs(mine[i].entityVector.x - newPoliceVector.x) < ((mineSpriteSizePxX / 2.0f) + (policeCarSpriteSizePxX / 2.0f)) && abs(mine[i].entityVector.y - newPoliceVector.y) < ((mineSpriteSizePxY / 2.0f) + (policeCarSpriteSizePxY / 2.0f)))
		{
			return false;
		}
	}

	for (int i = 0; i < suspect.size(); i++)
	{
		if (abs(suspect[i].entityVector.x - newPoliceVector.x) < ((suspectSpriteSizePxX / 2.0f) + (policeCarSpriteSizePxX / 2.0f)) && abs(suspect[i].entityVector.y - newPoliceVector.y) < ((suspectSpriteSizePxY / 2.0f) + (policeCarSpriteSizePxY / 2.0f)))
		{
			return false;
		}
	}

	return true;
}

void drawMine(sf::Sprite mineSprite, int mineCount, sf::RenderWindow& window, spawnControl& spawns, std::vector<entity>& mine)
{
	if (!spawns.mineSpawned)
	{
		for (int i = 0; i < mineCount; i++)
		{
			sf::Vector2f newMineVector = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForMine(newMineVector, mine, mineSprite, window))
			{
				mine.push_back({ mineSprite, newMineVector });
			}
			else
			{
				i--;
			}
		}
	}

	for (int temp = 0; temp < mine.size(); temp++)
	{
		mine[temp].entitySprite.setPosition(mine[temp].entityVector);
		window.draw(mine[temp].entitySprite);
	}

	spawns.mineSpawned = true;
}

void drawSuspect(sf::Sprite suspectCarSprite, const int& suspectCount, int& currentSuspectCount, sf::RenderWindow& window, spawnControl& spawns, std::vector<entity>& suspect, std::vector<entity>& mine)
{

	currentSuspectCount = 0;

	if (!spawns.suspectSpawned)
	{
		for (int i = 0; i < suspectCount; i++)
		{
			sf::Vector2f newSuspectVector = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForSuspect(newSuspectVector, suspect, suspectCarSprite, mine, window))
			{
				suspect.push_back({ suspectCarSprite, newSuspectVector });
			}
			else
			{
				i--;
			}
		}
	}

	for (int temp = 0; temp < suspect.size(); temp++)
	{
		suspect[temp].entitySprite.setTextureRect(sf::IntRect({ 384 * temp, 0 }, { 384, 192 }));
		suspect[temp].entitySprite.setPosition(suspect[temp].entityVector);

		if (suspect[temp].entityVector != sf::Vector2f{ -1, -1 }) //suspectCar's location changes to -1,-1 if the policeCar collide with an suspectCar so before we drawing we check if car should be redraw again or stay destroyed 
		{
			window.draw(suspect[temp].entitySprite);
			currentSuspectCount++;
		}
	}

	spawns.suspectSpawned = true;
}

void drawPolice(const int suspectCount, sf::RenderWindow& window, std::vector<entity>& suspect, std::vector<entity>& mine, policeCar& police, spawnControl& spawns)
{
	if (!spawns.policeSpawned)
	{
		for (int i = 0; i < 1; i++)
		{
			sf::Vector2f newPoliceVector = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForPolice(newPoliceVector, police, suspect, mine, window))
			{
				police.policeCarSprite.setPosition(newPoliceVector);
				window.draw(police.policeCarSprite);
			}
			else
			{
				i--;
			}
		}
	}

	window.draw(police.policeCarSprite);

	spawns.policeSpawned = true;
}

void handleEffects(bool& isArrested, bool gameOver, sf::Sound& boomEffect, sf::Sound& handcuffEffect)
{
	if (isArrested)
	{
		handcuffEffect.play();
		isArrested = false;
	}

	if (gameOver)
	{
		boomEffect.play();
	}
}

void resetLevel(int currentSuspectCount, int& mineCount, spawnControl& spawns, std::vector<entity>& suspect, std::vector<entity>& mine, sf::Vector2f& acceleration)
{
	if (currentSuspectCount == 0)
	{
		spawns.mineSpawned = false;
		spawns.suspectSpawned = false;
		spawns.policeSpawned = false;
		suspect.clear();
		mine.clear();
		if (mineCount < 25) { mineCount += 1; }
		acceleration = { 0.0f,0.0f };
	}
}

void handleGameOver(bool& gameOver, int& score, int& mineCount, spawnControl& spawns, std::vector<entity>& suspect, std::vector<entity>& mine, sf::Vector2f& acceleration)
{
	if (gameOver)
	{
		spawns.mineSpawned = false;
		spawns.suspectSpawned = false;
		spawns.policeSpawned = false;
		suspect.clear();
		mine.clear();
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

	sf::Sprite mineSprite(mineTexture);
	mineSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 96, 96 }));
	mineSprite.setScale({ 0.5f,0.5f });
	mineSprite.setOrigin(mineSprite.getLocalBounds().getCenter());

	sf::Sprite policeCarSprite(policeCarTexture);
	policeCarSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 384, 192 }));
	policeCarSprite.setScale({ 0.5f,0.5f });
	policeCarSprite.setOrigin(policeCarSprite.getLocalBounds().getCenter());

	sf::Sprite suspectCarSprite(suspectCarsTexture);
	suspectCarSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 384, 192 }));
	suspectCarSprite.setScale({ 0.5f,0.5f });
	suspectCarSprite.setOrigin(suspectCarSprite.getLocalBounds().getCenter());

	sf::SoundBuffer boomBuffer;
	if (!boomBuffer.loadFromFile(ASSETS_PATH "sounds/boomEffect.ogg"))
	{
		std::println("boomEffect.mp3 loadfromfile error");
	}
	sf::SoundBuffer handcuffBuffer;
	if (!handcuffBuffer.loadFromFile(ASSETS_PATH "sounds/handcuffEffect.ogg")) // i convert all the mp3 to ogg because mp3 version print error msg even if it is working | source : https://en.sfml-dev.org/forums/index.php?topic=15905.0
	{
		std::println("handcuffEffect.mp3 loadfromfile error");
	}

	sf::Sound boomEffect(boomBuffer);
	boomEffect.setVolume(100.0f);
	sf::Sound handcuffEffect(handcuffBuffer);
	handcuffEffect.setVolume(50.0f);

	sf::Music backgroundMusic;
	if (!backgroundMusic.openFromFile(ASSETS_PATH "sounds/backgroundMusic.ogg"))
	{
		std::println("backgroundMusic.mp3 openfromfile error");
	}

	backgroundMusic.play();
	backgroundMusic.setLooping(true);
	backgroundMusic.setVolume(25.0f);

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


	srand(time(0));
	int mineCount = 10;
	std::vector<entity> mine;

	bool gameOver = false;
	bool isArrested = false;

	const int suspectCount = 5; // sprite sheet have only 5 different car
	int currentSuspectCount = 0; // number of suspect on the screen
	std::vector<entity> suspect;

	int score = 0;

	policeCar police = { policeCarSprite,{5000.f,5000.f} };

	spawnControl spawns;

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
		police.policeCarSprite.move({ acceleration.x * displacementPx , acceleration.y * displacementPx });

		/*checkCollision(policeCarSprite, mineSprite, suspectCarSprite, windowWidth, windowHeight, mineVectors, gameOver, suspectVectors, score, boomEffect, handcuffEffect);*/
		handleWindowCollision(police, window);
		if (checkMineCollision(police, mine)) { gameOver = true; }
		if (checkSuspectCollision(police, suspect)) { isArrested = true; score++; }

		police.policeCarSprite.setTextureRect((updateAnimationFrames(police, deltaTimeMs, acceleration, stateIndex, frameIndex, animationProgressMs)));

		window.clear();
		window.draw(background);
		window.draw(text);
		scoreText.setString(std::to_string(score));
		window.draw(scoreText);
		drawMine(mineSprite, mineCount, window, spawns, mine);
		drawSuspect(suspectCarSprite, suspectCount, currentSuspectCount, window, spawns, suspect, mine);
		drawPolice(suspectCount, window, suspect, mine, police, spawns);
		window.display();

		handleEffects(isArrested, gameOver, boomEffect, handcuffEffect);
		resetLevel(currentSuspectCount, mineCount, spawns, suspect, mine, acceleration);
		handleGameOver(gameOver, score, mineCount, spawns, suspect, mine, acceleration);
	}

	return 0;
}