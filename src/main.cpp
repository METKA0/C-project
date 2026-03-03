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
* https://www.youtube.com/watch?v=cWgox89PZro&list=PLz6j8tWRKzOHQPOL5gGY4Ev6DoMRB2gee&index=2
* Font: Roboto Mono by Google Fonts (https://fonts.google.com/specimen/Roboto+Mono)
===============================================================================
*/


int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Shapes");
	window.setFramerateLimit(60);

	//sf::RectangleShape shape({50.f, 100.f}); 
	//shape.setOrigin(shape.getGeometricCenter());
	//shape.setOrigin({ 25.f,50.f });
	//shape.setFillColor(sf::Color::Red);
	//shape.setPosition({ windowWidth / 2.f, windowHeight / 2.f });

	float windowWidth = window.getSize().x;
	float windowHeight = window.getSize().y;

	sf::Texture background_t;
	background_t.loadFromFile("../../../assets/images/background.jpg");
	sf::Texture bread_t;
	bread_t.loadFromFile("../../../assets/images/bread.png");

	sf::Sprite background(background_t);
	background.setScale({ windowWidth / background_t.getSize().x,windowHeight / background_t.getSize().y });
	sf::Sprite bread(bread_t);
	bread.setPosition({ windowWidth / 2.f, windowHeight / 2.f });
	sf::FloatRect bounds = bread.getLocalBounds();
	bread.setOrigin(bounds.size / 2.f);
	bread.setScale({ 0.5,0.5 });


	sf::Font font;
	font.openFromFile("../../../assets/fonts/RobotoMono-Regular.ttf");
	sf::Text text(font);
	text.setString("       --- CONTROLS --- \n\n\n"
		"[ X ]         : Cook The Bread\n\n"
		"[ Z ]          : Make Bread Transparent\n\n"
		"[ Q ] [ E ]   : Rotate Angle\n\n"
		"    [^]       \n"
		"[<] [v] [>]   : Move Character\n"
	);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color(255, 255, 240));

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			bread.move({ -1.f, 0.f });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			bread.move({ 1.f, 0.f });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			bread.move({ 0.f, -1.f });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			bread.move({ 0.f, 1.f });
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
			bread.rotate(sf::degrees(1));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
		{
			bread.rotate(sf::degrees(-1));
		}



		window.clear();
		window.draw(background);
		window.draw(text);
		// window.draw(shape);
		window.draw(bread);
		window.display();

	}



	return 0;
}