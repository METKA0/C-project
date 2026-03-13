#include <iostream>

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


int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "Shapes");
	//window.setFramerateLimit(60);

	//sf::RectangleShape shape({50.f, 100.f}); 
	//shape.setOrigin(shape.getGeometricCenter());
	//shape.setOrigin({ 25.f,50.f });
	//shape.setFillColor(sf::Color::Red);
	//shape.setPosition({ windowWidth / 2.f, windowHeight / 2.f });

	float windowWidth = window.getSize().x;
	float windowHeight = window.getSize().y;
	sf::Clock clock;


	sf::Texture background_t;
	if (background_t.loadFromFile("../../../assets/images/background.jpg"))
	{
		std::cout << "background.jpg loadfromfile error \n";
	}
	sf::Texture bread_t;
	if (bread_t.loadFromFile("../../../assets/images/bread_spritesheet.png"))
	{
		std::cout << "background.jpg loadfromfile error \n";
	}


	sf::Sprite background(background_t);
	background.setScale({ windowWidth / background_t.getSize().x,windowHeight / background_t.getSize().y });
	sf::Sprite bread(bread_t);
	bread.setTextureRect(sf::IntRect({0, 0}, {32, 46}));
	bread.setScale({ 4.0f,4.0f });
	bread.setOrigin(bread.getLocalBounds().getCenter());
	bread.setPosition({ windowWidth / 2.f, windowHeight / 2.f });

	sf::IntRect frame_1({ 0,0 }, { 32,32 });
	sf::IntRect frame_2({ 32,0 }, { 32,32 });
	sf::IntRect frame_3({ 64,0 }, { 32,32 });

	sf::IntRect frame_4({ 0,32 }, { 32,32 });
	sf::IntRect frame_5({ 32,32 }, { 32,32 });
	sf::IntRect frame_6({ 64,32 }, { 32,32 });
	sf::IntRect frame_7({ 96,32 }, { 32,32 });

	

	std::vector<sf::IntRect> idle_frames;
	idle_frames.push_back(frame_1);
	idle_frames.push_back(frame_2);
	idle_frames.push_back(frame_3);

	std::vector<sf::IntRect> walk_frames;
	walk_frames.push_back(frame_4);
	walk_frames.push_back(frame_5);
	walk_frames.push_back(frame_6);
	walk_frames.push_back(frame_7);

	unsigned int stateIndex = 0;
	unsigned int index = 0;
	float anim_timer = 0.0f;
	float anim_speed = 0.1f;

	std::vector<std::vector<sf::IntRect>> currentState;
	currentState.push_back(idle_frames);
	currentState.push_back(walk_frames);



	sf::Font font;
	if (font.openFromFile("../../../assets/fonts/RobotoMono-Regular.ttf"))
	{
		std::cout << "RobotoMono-Regular.ttf openfromfile error\n";
	}
	sf::Text text(font);
	text.setString("        --- CONTROLS --- \n\n\n"
		"[ X ]          : Cook The Bread\n\n"
		"[ Z ]          : Make Bread Transparent\n\n"
		"[ Q ] [ E ]    : Rotate Angle\n\n"
		"[ Scroll ]     : Zoom In / Out\n\n"
		"     [^]       \n"
		"[<] [v] [>]    : Move Character\n"
	);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color(255, 255, 240));

	sf::View view({ bread.getPosition(),{1920.0f,1080.0f} });
	float zoomRate = 1.0f;
	//view.zoom(1.0f);

	

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		float speed = 200 * dt;

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			else if (auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				if (mouse->delta == 1 && zoomRate > 0.2)
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		{
			bread.setColor(sf::Color(255, 255, 255, 128));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		{
			bread.rotate(sf::degrees(-speed));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
		{
			bread.rotate(sf::degrees(speed));
		}

		anim_timer += dt;

		if (anim_timer >= anim_speed)
		{
			anim_timer = 0.0f;

			if (index >= currentState[stateIndex].size() - 1)
			{
				index = 0;
			}
			else
			{
				++index;
			}
			bread.setTextureRect(currentState[stateIndex][index]);
		}

		stateIndex = 0;


		window.clear();;
		window.draw(background);
		window.draw(text);
		// window.draw(shape);
		window.draw(bread);
		window.setView(view);
		window.display();

	}



	return 0;
}