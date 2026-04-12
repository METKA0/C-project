#include <iostream>
#include <print>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

enum Direction {Up = -1, Down = 1, Right = 1, Left = -1};

struct Entity {
	sf::Sprite sprite;
	sf::Vector2f position;
};

struct PoliceCar {
	sf::Sprite sprite;
	sf::Vector2f position;
};

struct SpawnControl {
	bool mineSpawned = false;
	bool suspectSpawned = false;
	bool policeSpawned = false;
};

void handleWindowCollision(PoliceCar& police, sf::RenderWindow& window)
{
	float windowWidth = window.getSize().x;
	float windowHeight = window.getSize().y;

	if (police.sprite.getGlobalBounds().getCenter().x > windowWidth - (police.sprite.getGlobalBounds().size.x / 2.0f))
	{
		police.sprite.setPosition({ windowWidth - (police.sprite.getGlobalBounds().size.x / 2.0f), police.sprite.getGlobalBounds().getCenter().y });
	}
	if (police.sprite.getGlobalBounds().getCenter().x < police.sprite.getGlobalBounds().size.x / 2.0f)
	{
		police.sprite.setPosition({ police.sprite.getGlobalBounds().size.x / 2.0f, police.sprite.getGlobalBounds().getCenter().y });
	}
	if (police.sprite.getGlobalBounds().getCenter().y > windowHeight - (police.sprite.getGlobalBounds().size.y / 2.0f))
	{
		police.sprite.setPosition({ police.sprite.getGlobalBounds().getCenter().x, windowHeight - (police.sprite.getGlobalBounds().size.y / 2.0f) });
	}
	if (police.sprite.getGlobalBounds().getCenter().y < (police.sprite.getGlobalBounds().size.y / 2.0f))
	{
		police.sprite.setPosition({ police.sprite.getGlobalBounds().getCenter().x, (police.sprite.getGlobalBounds().size.y / 2.0f) });
	}
}

bool checkMineCollision(PoliceCar& police, std::vector<Entity>& mines)
{
	for (const Entity& mine : mines)
	{
		if (std::abs(police.sprite.getGlobalBounds().getCenter().x - mine.position.x) <= ((police.sprite.getGlobalBounds().size.x / 2.0f) + (mine.sprite.getGlobalBounds().size.x / 2.0f)) && std::abs(police.sprite.getGlobalBounds().getCenter().y - mine.position.y) <= ((police.sprite.getGlobalBounds().size.y / 2.0f) + (mine.sprite.getGlobalBounds().size.y / 2.0f)))
		{
			return true;
		}
	}
	return false;
}

bool checkSuspectCollision(PoliceCar& police, std::vector<Entity>& suspects)
{
	for (int i = 0; i < suspects.size(); i++)
	{
		if (std::abs(police.sprite.getGlobalBounds().getCenter().x - suspects[i].position.x) <= ((police.sprite.getGlobalBounds().size.x / 2.0f) + (suspects[i].sprite.getGlobalBounds().size.x / 2.0f)) && std::abs(police.sprite.getGlobalBounds().getCenter().y - suspects[i].position.y) <= ((police.sprite.getGlobalBounds().size.y / 2.0f) + (suspects[i].sprite.getGlobalBounds().size.y / 2.0f)))
		{
			suspects.erase(suspects.begin() + i);
			return true;
		}
	}
	return false;
}

sf::Vector2f normalize(sf::Vector2f velocity)
{
	if (velocity == sf::Vector2f{ 0, 0 }) { return { 0, 0 }; }

	float length = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
	assert(length > 0);
	sf::Vector2f normalizedVelocity = { velocity.x / length, velocity.y / length };
	return normalizedVelocity;
}

sf::Vector2f getNormalizedDirection()
{
	sf::Vector2f direction = {0, 0};

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) { direction.x = Left; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) { direction.x = Right; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) { direction.y = Up; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) { direction.y = Down; }

	sf::Vector2f normalizedDirection = normalize(direction);

	return normalizedDirection;
}

void handleVelocity(sf::Vector2f& velocity, float& speedPx, float deltaTimeMs)
{
	sf::Vector2f normalizedDirection = getNormalizedDirection();

	sf::Vector2f thrust = normalizedDirection * speedPx;

	sf::Vector2f friction = -velocity * 0.003f;

	sf::Vector2f acceleration = thrust + friction;

	velocity += acceleration * deltaTimeMs;
}

sf::IntRect updateAnimationFrames(PoliceCar& police, float deltaTimeMs, sf::Vector2f velocity, unsigned& stateIndex, unsigned& frameIndex, float& animationProgressMs)
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

	if (velocity.x > 8.0f || velocity.x < -8.0f || velocity.y > 8.0f || velocity.y < -8.0f)
	{
		stateIndex = 1;
	}
	else
	{
		stateIndex = 0;
	}

	if (velocity.x > 0.1f)
	{
		police.sprite.setScale({ 0.5f, 0.5f });
	}
	else if (velocity.x < -0.1f)
	{
		police.sprite.setScale({ -0.5f, 0.5f });
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

bool isPlaceEmptyForMine(sf::Vector2f newMinePosition, std::vector<Entity>& mines, sf::Sprite mineSprite, sf::RenderWindow& window)
{

	int mineSpriteSizePxX = mineSprite.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mineSprite.getGlobalBounds().size.y;

	if (newMinePosition.x > (window.getSize().x - mineSpriteSizePxX) || newMinePosition.x < mineSpriteSizePxX || newMinePosition.y >(window.getSize().y - mineSpriteSizePxY) || newMinePosition.y < mineSpriteSizePxY)
	{
		return false;
	}

	for (const Entity& mine : mines)
	{
		if (abs(mine.position.x - newMinePosition.x) < mineSpriteSizePxX && abs(mine.position.y - newMinePosition.y) < mineSpriteSizePxY)
		{
			return false;
		}
	}

	return true;
}

bool isPlaceEmptyForSuspect(sf::Vector2f newSuspectPosition, std::vector<Entity>& suspects, sf::Sprite suspectCarSprite, std::vector<Entity>& mines, sf::RenderWindow& window)
{

	int suspectSpriteSizePxX = suspectCarSprite.getGlobalBounds().size.x;
	int suspectSpriteSizePxY = suspectCarSprite.getGlobalBounds().size.y;

	assert(!mines.empty());
	int mineSpriteSizePxX = mines[0].sprite.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mines[0].sprite.getGlobalBounds().size.y;

	if (newSuspectPosition.x > (window.getSize().x - suspectSpriteSizePxX) || newSuspectPosition.x < suspectSpriteSizePxX || newSuspectPosition.y >(window.getSize().y - suspectSpriteSizePxY) || newSuspectPosition.y < suspectSpriteSizePxY)
	{
		return false;
	}

	for (const Entity& suspect : suspects)
	{
		if (abs(suspect.position.x - newSuspectPosition.x) < suspectSpriteSizePxX && abs(suspect.position.y - newSuspectPosition.y) < suspectSpriteSizePxY)
		{
			return false;
		}
	}

	for (const Entity& mine : mines)
	{
		if (abs(mine.position.x - newSuspectPosition.x) < ((suspectSpriteSizePxX / 2.0f) + (mineSpriteSizePxX / 2.0f)) && abs(mine.position.y - newSuspectPosition.y) < ((suspectSpriteSizePxY / 2.0f) + (mineSpriteSizePxY / 2.0f)))
		{
			return false;
		}
	}

	return true;
}

bool isPlaceEmptyForPolice(sf::Vector2f newPolicePosition, PoliceCar& police, std::vector<Entity>& suspects, std::vector<Entity>& mines, sf::RenderWindow& window)
{
	assert(!suspects.empty());
	int suspectSpriteSizePxX = suspects[0].sprite.getGlobalBounds().size.x;
	int suspectSpriteSizePxY = suspects[0].sprite.getGlobalBounds().size.y;

	assert(!mines.empty());
	int mineSpriteSizePxX = mines[0].sprite.getGlobalBounds().size.x;
	int mineSpriteSizePxY = mines[0].sprite.getGlobalBounds().size.y;

	int policeCarSpriteSizePxX = police.sprite.getGlobalBounds().size.x;
	int policeCarSpriteSizePxY = police.sprite.getGlobalBounds().size.y;

	if (newPolicePosition.x > (window.getSize().x - policeCarSpriteSizePxX) || newPolicePosition.x < policeCarSpriteSizePxX || newPolicePosition.y >(window.getSize().y - policeCarSpriteSizePxY) || newPolicePosition.y < policeCarSpriteSizePxY)
	{
		return false;
	}

	for (const Entity& mine : mines)
	{
		if (abs(mine.position.x - newPolicePosition.x) < ((mineSpriteSizePxX / 2.0f) + (policeCarSpriteSizePxX / 2.0f)) && abs(mine.position.y - newPolicePosition.y) < ((mineSpriteSizePxY / 2.0f) + (policeCarSpriteSizePxY / 2.0f)))
		{
			return false;
		}
	}

	for (const Entity& suspect : suspects)
	{
		if (abs(suspect.position.x - newPolicePosition.x) < ((suspectSpriteSizePxX / 2.0f) + (policeCarSpriteSizePxX / 2.0f)) && abs(suspect.position.y - newPolicePosition.y) < ((suspectSpriteSizePxY / 2.0f) + (policeCarSpriteSizePxY / 2.0f)))
		{
			return false;
		}
	}

	return true;
}

void drawMine(sf::Sprite mineSprite, int mineCount, sf::RenderWindow& window, SpawnControl& spawns, std::vector<Entity>& mines)
{
	if (!spawns.mineSpawned)
	{
		for (int i = 0; i < mineCount; i++)
		{
			sf::Vector2f newMinePosition = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForMine(newMinePosition, mines, mineSprite, window))
			{
				mines.push_back({ mineSprite, newMinePosition });
			}
			else
			{
				i--;
			}
		}
	}

	for (Entity& mine : mines)
	{
		mine.sprite.setPosition(mine.position);
		window.draw(mine.sprite);
	}

	spawns.mineSpawned = true;
}

void drawSuspect(sf::Sprite suspectCarSprite, const int& suspectCount, sf::RenderWindow& window, SpawnControl& spawns, std::vector<Entity>& suspects, std::vector<Entity>& mines)
{
	if (!spawns.suspectSpawned)
	{
		for (int i = 0; i < suspectCount; i++)
		{
			sf::Vector2f newSuspectPosition = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForSuspect(newSuspectPosition, suspects, suspectCarSprite, mines, window))
			{
				suspects.push_back({ suspectCarSprite, newSuspectPosition });
				suspects[i].sprite.setTextureRect(sf::IntRect({ 384 * i, 0 }, { 384, 192 }));
				suspects[i].sprite.setPosition(suspects[i].position);
			}
			else
			{
				i--;
			}
		}
	}

	for (const Entity& suspect : suspects)
	{
		window.draw(suspect.sprite);
	}

	spawns.suspectSpawned = true;
}

void drawPolice(const int suspectCount, sf::RenderWindow& window, std::vector<Entity>& suspects, std::vector<Entity>& mines, PoliceCar& police, SpawnControl& spawns)
{
	if (!spawns.policeSpawned)
	{
		for (int i = 0; i < 1; i++)
		{
			sf::Vector2f newPolicePosition = { (float)(std::rand() % window.getSize().x) , (float)(std::rand() % window.getSize().y) };

			if (isPlaceEmptyForPolice(newPolicePosition, police, suspects, mines, window))
			{
				police.sprite.setPosition(newPolicePosition);
				window.draw(police.sprite);
			}
			else
			{
				i--;
			}
		}
	}

	window.draw(police.sprite);

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

void resetLevel(int& mineCount, SpawnControl& spawns, std::vector<Entity>& suspects, std::vector<Entity>& mines, sf::Vector2f& velocity)
{
	if (suspects.empty())
	{
		spawns.mineSpawned = false;
		spawns.suspectSpawned = false;
		spawns.policeSpawned = false;
		suspects.clear();
		mines.clear();
		if (mineCount < 25) { mineCount += 1; }
		velocity = { 0.0f,0.0f };
	}
}

void handleGameOver(bool& gameOver, int& score, int& mineCount, SpawnControl& spawns, std::vector<Entity>& suspects, std::vector<Entity>& mines, sf::Vector2f& velocity)
{
	if (gameOver)
	{
		spawns.mineSpawned = false;
		spawns.suspectSpawned = false;
		spawns.policeSpawned = false;
		suspects.clear();
		mines.clear();
		score = 0;
		mineCount = 10;

		velocity = { 0.0f,0.0f };

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
	std::vector<Entity> mines;

	bool gameOver = false;
	bool isArrested = false;

	const int suspectCount = 5; // sprite sheet have only 5 different car
	std::vector<Entity> suspects;

	int score = 0;

	PoliceCar police = { policeCarSprite,{5000.f,5000.f} };

	SpawnControl spawns;

	float speedPx = 2.5f;

	sf::Vector2f velocity = { 0 , 0 };

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

		handleVelocity(velocity,speedPx,deltaTimeMs);
		sf::Vector2f displacementPx = velocity * (deltaTimeMs * 0.001f);
		sf::Vector2f policeNewPosition = police.sprite.getPosition() + displacementPx;
		police.sprite.setPosition(policeNewPosition);

		handleWindowCollision(police, window);
		if (checkMineCollision(police, mines)) { gameOver = true; }
		if (checkSuspectCollision(police, suspects)) { isArrested = true; score++; }

		police.sprite.setTextureRect((updateAnimationFrames(police, deltaTimeMs, velocity, stateIndex, frameIndex, animationProgressMs)));

		window.clear();
		window.draw(background);
		window.draw(text);
		scoreText.setString(std::to_string(score));
		window.draw(scoreText);
		drawMine(mineSprite, mineCount, window, spawns, mines);
		drawSuspect(suspectCarSprite, suspectCount, window, spawns, suspects, mines);
		drawPolice(suspectCount, window, suspects, mines, police, spawns);
		window.display();

		handleEffects(isArrested, gameOver, boomEffect, handcuffEffect);
		resetLevel(mineCount, spawns, suspects, mines, velocity);
		handleGameOver(gameOver, score, mineCount, spawns, suspects, mines, velocity);
	}
	return 0;
}