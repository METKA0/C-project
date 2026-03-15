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
* https://www.youtube.com/watch?v=cWgox89PZro&list=PLz6j8tWRKzOHQPOL5gGY4Ev6DoMRB2gee&index=2
* https://www.youtube.com/watch?v=vz1jGgSVsUQ
* https://www.youtube.com/watch?v=ulct06sYvoM
* https://www.youtube.com/watch?v=LZXJEF39fm8
* Font: Roboto Mono by Google Fonts (https://fonts.google.com/specimen/Roboto+Mono)
===============================================================================
*/


//void checkCollision(sf::Sprite& bread, float windowWidth, float windowHeight)
//{
//	if (bread.getGlobalBounds().getCenter().x > windowWidth - (bread.getGlobalBounds().size.x / 2.0f))
//	{
//		bread.setPosition({ windowWidth - (bread.getGlobalBounds().size.x / 2.0f), bread.getGlobalBounds().getCenter().y });
//	}
//	if (bread.getGlobalBounds().getCenter().x < bread.getGlobalBounds().size.x / 2.0f)
//	{
//		bread.setPosition({ bread.getGlobalBounds().size.x / 2.0f, bread.getGlobalBounds().getCenter().y });
//	}
//	if (bread.getGlobalBounds().getCenter().y > windowHeight - (bread.getGlobalBounds().size.y / 2.0f))
//	{
//		bread.setPosition({ bread.getGlobalBounds().getCenter().x, windowHeight - (bread.getGlobalBounds().size.y / 2.0f) });
//	}
//	if (bread.getGlobalBounds().getCenter().y < (bread.getGlobalBounds().size.y / 2.0f))
//	{
//		bread.setPosition({ bread.getGlobalBounds().getCenter().x, (bread.getGlobalBounds().size.y / 2.0f) });
//	}
//}

void checkCollision(sf::Sprite& bread, float windowWidth, float windowHeight, float breadPozX, float breadPozY, float breadSizeX, float breadSizeY)
{
	if (breadPozX > windowWidth - (breadSizeX / 2.0f))
	{
		bread.setPosition({ windowWidth - (breadSizeX / 2.0f), breadPozY });
	}
	if (breadPozX < breadSizeX / 2.0f)
	{
		bread.setPosition({ breadSizeX / 2.0f , breadPozY });
	}
	if (breadPozY > windowHeight - (breadSizeY / 2.0f))
	{
		bread.setPosition({ breadPozX , windowHeight - (breadSizeY / 2.0f) });
	}
	if (breadPozY < (breadSizeY / 2.0f))
	{
		bread.setPosition({ breadPozX , (breadSizeY / 2.0f) });
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "Shapes");

	float windowWidth = window.getSize().x;
	float windowHeight = window.getSize().y;
	sf::Clock clock;


	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile(ASSETS_PATH "images/background.jpg"))
	{
		std::println("background.jpg loadfromfile error");
	}
	sf::Texture breadTexture;
	if (!breadTexture.loadFromFile(ASSETS_PATH "images/bread_spritesheet.png"))
	{
		std::println("bread_spritesheet.png loadfromfile error");
	}


	sf::Sprite background(backgroundTexture);
	background.setScale({ windowWidth / backgroundTexture.getSize().x,windowHeight / backgroundTexture.getSize().y });
	sf::Sprite bread(breadTexture);
	bread.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));
	bread.setScale({ 4.0f,4.0f });
	bread.setOrigin(bread.getLocalBounds().getCenter());
	bread.setPosition({ windowWidth / 2.f, windowHeight / 2.f });

	std::vector<sf::IntRect> idleFrames;
	idleFrames.push_back(sf::IntRect({ 0, 0 }, { 32, 32 }));
	idleFrames.push_back(sf::IntRect({ 32, 0 }, { 32, 32 }));
	idleFrames.push_back(sf::IntRect({ 64, 0 }, { 32, 32 }));

	std::vector<sf::IntRect> walkFrames;
	walkFrames.push_back(sf::IntRect({ 0, 32 }, { 32, 32 }));
	walkFrames.push_back(sf::IntRect({ 32, 32 }, { 32, 32 }));
	walkFrames.push_back(sf::IntRect({ 64, 32 }, { 32, 32 }));
	walkFrames.push_back(sf::IntRect({ 96, 32 }, { 32, 32 }));

	unsigned int stateIndex = 0;
	unsigned int frameIndex = 0;
	float animationProgressSec = 0.0f;
	constexpr float animationDurationSec = 0.1f;

	std::vector<std::vector<sf::IntRect>> currentState;
	currentState.push_back(idleFrames);
	currentState.push_back(walkFrames);

	sf::Font font;
	if (!font.openFromFile(ASSETS_PATH "fonts/RobotoMono-Regular.ttf"))
	{
		std::println("RobotoMono-Regular.ttf openfromfile error");
	}
	sf::Text text(font);
	text.setString("        --- CONTROLS --- \n\n\n"
		"[ X ]          : Cook The Bread\n\n"
		"[ Scroll ]     : Zoom In / Out\n\n"
		"     [^]       \n"
		"[<] [v] [>]    : Move Character\n"
	);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color(255, 255, 240));

	sf::View view({ bread.getPosition(),{1920.0f,1080.0f} });
	float zoomRate = 1.0f;

	while (window.isOpen())
	{
		float deltaTimeSec = clock.restart().asSeconds();
		float speed = 200.0f * deltaTimeSec;

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			else if (auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				if (mouse->delta == 1 && zoomRate > 0.1)
				{
					zoomRate -= 0.1;
					std::cout << zoomRate;
					std::cout << "\n";
					view.setSize({ windowWidth * zoomRate , windowHeight * zoomRate });
				}
				else if (mouse->delta == -1 && zoomRate < 1)
				{
					zoomRate += 0.1;
					std::cout << zoomRate;
					std::cout << "\n";
					view.setSize({ windowWidth * zoomRate , windowHeight * zoomRate });

				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			bread.move({ -speed, 0.f });
			view.move({ -speed, 0.f });
			stateIndex = 1;
			bread.setScale({ -4, 4 });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			bread.move({ speed, 0.f });
			view.move({ speed, 0.f });
			stateIndex = 1;
			bread.setScale({ 4, 4 });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			bread.move({ 0.f, -speed });
			view.move({ 0.f, -speed });
			stateIndex = 1;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			bread.move({ 0.f, speed });
			view.move({ 0.f, speed });
			stateIndex = 1;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
		{
			bread.setColor(sf::Color(186, 142, 35));
		}

		animationProgressSec += deltaTimeSec;

		if (animationProgressSec >= animationDurationSec)
		{
			animationProgressSec = 0.0f;

			if (frameIndex >= currentState[stateIndex].size() - 1)
			{
				frameIndex = 0;
			}
			else
			{
				++frameIndex;
			}
			bread.setTextureRect(currentState[stateIndex][frameIndex]);
		}

		stateIndex = 0;

		float breadPozX = bread.getGlobalBounds().getCenter().x;
		float breadPozY = bread.getGlobalBounds().getCenter().y;
		float breadSizeX = bread.getGlobalBounds().size.x;
		float breadSizeY = bread.getGlobalBounds().size.y;

		checkCollision(bread, windowWidth, windowHeight, breadPozX, breadPozY, breadSizeX, breadSizeY);


		window.clear();;
		window.draw(background);
		window.draw(text);
		window.draw(bread);
		//window.setView(view);
		window.display();

	}



	return 0;
}