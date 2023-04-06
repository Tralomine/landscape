#pragma once

#include <SFML/Graphics.hpp>

sf::Vector2f getBezier(std::vector<sf::Vector2f> points, float adv);
sf::Vector2f getCyclicBezier(std::vector<sf::Vector2f> points, float adv, int index);
std::vector<sf::Vector2f> getBlob(size_t size, size_t b = 8, size_t n = 8); //blobiness, point count

sf::VertexArray getRandomTree(float size);
sf::VertexArray getDeciduous(float size);
sf::VertexArray getConiferous(float size);


double constexpr pi(3.141592653589793238462338);
