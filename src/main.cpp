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
* Font: Roboto Mono by Google Fonts (https://fonts.google.com/specimen/Roboto+Mono)
===============================================================================
*/


int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800,800 }), "SFML works!");
	sf::CircleShape shape(50.f); // i use 100.f instead of 100 for better performance and prevent jittering (most of the time use float) --- also 100 refers to radius of the circle(ex 100.f = 200x200pixel)
	shape.setFillColor(sf::Color::Red);
	sf::Font font;
	font.openFromFile("../../../assets/fonts/RobotoMono-Regular.ttf"); // it took me an hour to figure out why it wasnt working... 3 ".." symbol on the start for go backward on build file to main file then we point where our ttf exist
	sf::Text text(font);
	text.setString("Press 'X' to change color");
	text.setPosition({ 200, 200 });
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::Yellow);

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent()) // i use const to prevent accidental change --- optional event = poll event is new version of event system ,pollevent always check the inputs if there is any data
		{
			
			if (event->is<sf::Event::Closed>()) // when event happen if statement return true
			{
				window.close(); // for close the window
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			{
				shape.move({-1.f, 0.f});
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			{
				shape.move({ 1.f, 0.f });
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				shape.move({ 0.f, -1.f });
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				shape.move({ 0.f, 1.f });
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
			{
				shape.setFillColor(sf::Color::Yellow);
			}

		}

		window.clear();
		window.draw(text);
		window.draw(shape);
		window.display();

	}



	return 0;
}