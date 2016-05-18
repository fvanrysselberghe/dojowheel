#include "SFML\Graphics.hpp"
#include <iostream>
#include "Main.h"

#define FRAMES_PER_SECOND 50

int main(int argc, char *argv[])
{
	sf::Image image;
	if (argc != 2)
	{
		if (!image.loadFromFile("Example.png"))
			return false;
	}
	else
	{
		if (!image.loadFromFile(argv[1]))
			return false;
	}

	auto scannedImage = asDotImage(image);
	auto dotImageView = createView(scannedImage);

	//Initialize
	sf::RenderWindow window(sf::VideoMode(400, 400), "DojoWheel");

	short selectedLine = 0;

	auto timePerGameTick = sf::seconds(1.0f / FRAMES_PER_SECOND);

	auto clock = sf::Clock{};
	auto elapsed = sf::Time::Zero;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		elapsed += clock.restart();

		//Update
		while (elapsed > timePerGameTick)
		{
			elapsed -= timePerGameTick;
			
			fadeLines(dotImageView, selectedLine);
			selectedLine = selectedLine + 1 < dotImageView.size() ? ++selectedLine : 0;
		}
		
		render(window, dotImageView);
	}

	return 0;
}

std::vector<std::vector<sf::Color>> asDotImage( sf::Image &image)
{
	sf::RenderWindow debugWindow(sf::VideoMode(image.getSize().x, image.getSize().y), "Debug");
	debugWindow.setFramerateLimit(20);
	debugWindow.setPosition(sf::Vector2i{ 20,20 });
	
	auto imageDimensions = image.getSize();
	auto center = sf::Vector2f{ static_cast<float>(imageDimensions.x / 2), static_cast<float>(imageDimensions.y / 2) };

	auto scannedImage = std::vector<std::vector<sf::Color>>{360/10};
	auto scanLine = createScanLine(center);
	
	for (auto &imageLine : scannedImage)
	{
		//read a line
		for (auto pixelIndex = 0;
		pixelIndex < 32;
			++pixelIndex)
		{
			unsigned int x = static_cast<unsigned int>(std::round(std::abs(scanLine[pixelIndex].x)));
			unsigned int y = static_cast<unsigned int>(std::round(std::abs(scanLine[pixelIndex].y)));

			auto pixelColor = sf::Color::Cyan;
			if (x < image.getSize().x && y < image.getSize().y)
				pixelColor = image.getPixel(x, y);
			imageLine.push_back(pixelColor);
		}
		
		//draw
		debugWindow.clear();

		sf::Texture imageTexture;
		imageTexture.loadFromImage(image);

		sf::Sprite imageSprite{ imageTexture };
		debugWindow.draw(imageSprite);

		for (auto &pixel : scanLine)
		{
			sf::CircleShape dot{ 1.0f };
			dot.setFillColor(sf::Color::Black);
			dot.setPosition(pixel);
			debugWindow.draw(dot);
		}
		debugWindow.display();


		//goto next line
		for (auto &scanDot : scanLine)
		{
			sf::Transform transform;
			transform.rotate(10, scanLine.front());

			scanDot = transform.transformPoint(scanDot);
		}
	}

	debugWindow.close();

	return scannedImage;
}

std::vector<sf::Vector2f> createScanLine(sf::Vector2f &center)
{
	auto scanLine = std::vector<sf::Vector2f>{ 32, center };
	auto offset = 0.0f;
	for (auto &scanPixel : scanLine)
	{
		scanPixel.x += offset;
		offset += 1.0f;
	}
	return scanLine;
}

std::vector<std::vector<sf::CircleShape>> createView(const std::vector < std::vector<sf::Color> > &image)
{
	auto view = std::vector < std::vector<sf::CircleShape>>{};
	view.reserve(image.size());

	auto defaultShape = sf::CircleShape{ 2.5f };
	defaultShape.setPosition(200.0f, 200.0f);
	
	auto directionOffset = 0.0f;
	for (auto &imageLine: image)
	{ 
		auto singleLine = std::vector<sf::CircleShape>{};
		singleLine.reserve(imageLine.size());

		auto pixelOffset = 0.0f;
		for (auto &imagePixel : imageLine ) 
		{
			auto pixel = defaultShape;

			pixel.move(5.0f*pixelOffset, 0.0f);
			
			auto fill = imagePixel;
			fill.a = 0; //transparent;
			pixel.setFillColor(fill);

			pixelOffset += 1.0f;

			sf::Transform transform;
			transform.rotate(directionOffset * 10, sf::Vector2f{ 200.0f, 200.0f });

			pixel.setPosition(transform.transformPoint(pixel.getPosition()));
			
			singleLine.push_back(pixel);
		}

		view.push_back(singleLine);

		++directionOffset;
	}
	
	return view;

}

void fadeLines(std::vector<std::vector<sf::CircleShape>> &view, short currentLine)
{
	for (auto line = 0; line < view.size(); ++line)
	{
		if (line == currentLine)
			makeOpaque(view[line]);
		else
			makeMoreTransparent(view[line]);
	}
}

void makeOpaque(std::vector<sf::CircleShape> &line)
{
	for (auto &pixel : line)
	{
		auto color = pixel.getFillColor();
		color.a = 255;

		pixel.setFillColor(color);
	}
}

void makeMoreTransparent(std::vector<sf::CircleShape> &line)
{
	for (auto &pixel : line)
	{
		auto color = pixel.getFillColor();
		color.a = color.a > 20? color.a - 20: 0;

		pixel.setFillColor(color);
	}
}

void render(sf::RenderWindow &window, const std::vector<std::vector<sf::CircleShape>> &view)
{
	window.clear();
	for (auto &line : view)
	{
		render(window, line);
	}
	window.display();
}
void render(sf::RenderWindow &window, const std::vector<sf::CircleShape> &axis1)
{
	for (auto &pixel : axis1)
	{
		window.draw(pixel);
	}
}
