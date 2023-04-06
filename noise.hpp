#pragma once

#include <SFML/Graphics.hpp>

#include <cstdint>

class Noise
{
public:
  Noise(double scale = 64);
  Noise(double scale, size_t seed);

  void setScale(double scale) {m_scale = scale;}
  double getScale(void) const {return m_scale;}

  double getPerlin1D(double x) const;

  double getSimplex2D(double x, double y) const;
  double getSimplex2D(sf::Vector2f p) const
  {return getSimplex2D(p.x, p.y);}

  uint32_t getSeed(void) const {return m_seed;}

  uint64_t h(int32_t i[], size_t size) const; //hash coordinates

private:
  double m_scale;
  uint32_t m_seed;
};

double dotProduct(double xa, double ya, double xb, double yb);
double dotProduct(sf::Vector2f a, sf::Vector2f b);

double cubic(double x);
double cubic(double a1, double a2, double w); //3x^2-2x^3
int32_t fastfloor(double fp);
uint64_t hash(uint8_t *i, size_t size);
sf::Vector2f GfH(uint64_t h); //gradient from hash
constexpr double F2 = 0.5*(1.73205080756887729352744634150587236694280525381038062805580-1);//0.5*(sqrt(3)-1.0);
constexpr double G2 = (3-1.73205080756887729352744634150587236694280525381038062805580)/6.0;//(3-sqrt(3))/6;
