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


void checkCollision(sf::Sprite& bread, float windowWidth, float windowHeight)
{
	if (bread.getGlobalBounds().getCenter().x > windowWidth - (bread.getGlobalBounds().size.x / 2.0f))
	{
		bread.setPosition({ windowWidth - (bread.getGlobalBounds().size.x / 2.0f), bread.getGlobalBounds().getCenter().y });
	}
	if (bread.getGlobalBounds().getCenter().x < bread.getGlobalBounds().size.x / 2.0f)
	{
		bread.setPosition({ bread.getGlobalBounds().size.x / 2.0f, bread.getGlobalBounds().getCenter().y });
	}
	if (bread.getGlobalBounds().getCenter().y > windowHeight - (bread.getGlobalBounds().size.y / 2.0f))
	{
		bread.setPosition({ bread.getGlobalBounds().getCenter().x, windowHeight - (bread.getGlobalBounds().size.y / 2.0f) });
	}
	if (bread.getGlobalBounds().getCenter().y < (bread.getGlobalBounds().size.y / 2.0f))
	{
		bread.setPosition({ bread.getGlobalBounds().getCenter().x, (bread.getGlobalBounds().size.y / 2.0f) });
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
		if (currentAccelerationX > 0)
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
		if (currentAccelerationY > 0)
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

sf::IntRect updateAnimationFrames(sf::Sprite& car, float deltaTimeMs, sf::Vector2f Acceleration, unsigned& stateIndex, unsigned& frameIndex, float& animationProgressMs)
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
		car.setScale({ 0.5f, 0.5f });
	}
	else if (Acceleration.x < -0.1f)
	{
		car.setScale({ -0.5f, 0.5f });
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

bool isPlaceEmpty(sf::Vector2f newMineVector, std::vector<sf::Vector2f> mineVectors)
{

	int halfSizeOfSpritePx = 48;

	for (int i = 0; i < mineVectors.size(); i++)
	{
		if (abs(mineVectors[i].x - newMineVector.x) < halfSizeOfSpritePx || abs(mineVectors[i].y - newMineVector.y) < halfSizeOfSpritePx)
		{
			return false;
		}
	}

	return true;
}

void drawMine(sf::Sprite mine, int mineCount, sf::RenderWindow& window, bool& minesSpawned, std::vector<sf::Vector2f>& mineVectors)
{

	int spaceFromBordersPx = 100;

	if (!minesSpawned)
	{
		for (int i = 0; i < mineCount; i++)
		{
			sf::Vector2f newMineVector = { (float)(std::rand() % (window.getSize().x - spaceFromBordersPx)) , (float)(std::rand() % (window.getSize().y - spaceFromBordersPx))};

			if (isPlaceEmpty(newMineVector, mineVectors))
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


	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile(ASSETS_PATH "images/background.png"))
	{
		std::println("background.jpg loadfromfile error");
	}
	sf::Texture carTexture;
	if (!carTexture.loadFromFile(ASSETS_PATH "images/carSpriteSheet.png"))
	{
		std::println("carSprite.png loadfromfile error");
	}
	sf::Texture mineTexture;
	if (!mineTexture.loadFromFile(ASSETS_PATH "images/mine.png"))
	{
		std::println("mine.png loadfromfile error");
	}

	sf::Sprite mine(mineTexture);
	mine.setTextureRect(sf::IntRect({ 0, 0 }, { 96, 96 }));
	mine.setScale({ 0.5f,0.5f });
	sf::Sprite background(backgroundTexture);
	background.setScale({ windowWidth / backgroundTexture.getSize().x,windowHeight / backgroundTexture.getSize().y });
	sf::Sprite car(carTexture);
	car.setTextureRect(sf::IntRect({ 0, 0 }, { 384, 192 }));
	car.setScale({ 0.5f,0.5f });
	car.setOrigin(car.getLocalBounds().getCenter());
	car.setPosition({ windowWidth / 2.f, windowHeight / 2.f });

	sf::Font font;
	if (!font.openFromFile(ASSETS_PATH "fonts/RobotoMono-Regular.ttf"))
	{
		std::println("RobotoMono-Regular.ttf openfromfile error");
	}
	sf::Text text(font);
	text.setString("        --- CONTROLS --- \n\n\n"
		"     [^]       \n"
		"[<] [v] [>]    : Move Character\n"
	);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color(172, 50, 50));
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
		std::print("x = {}  -  y = {} \n", acceleration.x, acceleration.y);
		car.move({ acceleration.x * displacementPx , acceleration.y * displacementPx });

		checkCollision(car, windowWidth, windowHeight);

		car.setTextureRect((updateAnimationFrames(car, deltaTimeMs, acceleration, stateIndex, frameIndex, animationProgressMs)));

		window.clear();;
		window.draw(background);
		window.draw(text);
		drawMine(mine, mineCount, window, minesSpawned, mineVectors);
		window.draw(car);
		window.display();

	}


	return 0;
}