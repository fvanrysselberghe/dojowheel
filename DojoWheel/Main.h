#pragma once

void render(sf::RenderWindow &window, const std::vector<std::vector<sf::CircleShape>> &view);
void render(sf::RenderWindow &window, const std::vector<sf::CircleShape> &axis1);

std::vector<std::vector<sf::Color>> asDotImage(sf::Image &image);

std::vector<sf::Vector2f> createScanLine(sf::Vector2f &center);
std::vector<std::vector<sf::CircleShape>> createView(const std::vector<std::vector<sf::Color>> &image);

void fadeLines(std::vector<std::vector<sf::CircleShape>> &view, short currentLine);
void makeOpaque(std::vector<sf::CircleShape> &line);
void makeMoreTransparent(std::vector<sf::CircleShape> &line);