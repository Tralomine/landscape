#include "noise.hpp"

#include <limits>
#include <random>
#include <cstring>

Noise::Noise(double scale):
m_scale(scale),
m_seed(rand())
{}

Noise::Noise(double scale, size_t seed):
Noise(scale)
{
  m_seed = seed;
}

double Noise::getPerlin1D(double x) const
{
  std::seed_seq seed({int(x)});
  std::mt19937_64 generator(seed);
  std::uniform_real_distribution<float> distrib(0, 1);
  std::seed_seq seed2({int(x+1)});
  std::mt19937_64 generator2(seed2);
  return distrib(generator2) * cubic(x-int(x)) + distrib(generator) * cubic((int(x)+1)-x);
}

double Noise::getSimplex2D(double x, double y) const
{
  x /= m_scale*2, y /= m_scale*2;

  double s((x + y)*F2); //hairy factor 2D
  int32_t i = fastfloor(x+s); //simplex cell
  int32_t j = fastfloor(y+s);
  double t((i + j)*G2); //unskew factor
  double X0(i-t), Y0(j-t);  //simplex cell
  float x0(x-X0), y0(y-Y0);  //offset par rapport au premier point
  int i1, j1; //wich corner we take for the simplex (right/left along the main diagonal)
  if (x0 > y0) i1 = 1, j1 = 0;  //right  (0,0),(1,0),(1,1)
  else i1 = 0, j1 = 1;          //left   (0,0),(0,1),(1,1)

  float x1(x0 - i1 + G2), y1(y0 - j1 + G2);  //offset par rapport a l'angle du milieu (gauche/droite)
  float x2(x0 - 1 + 2*G2), y2(y0 - 1 + 2*G2);

  int32_t hc[3][8] = {{i, j}, {i+i1, j+j1}, {i+1, j+1}};
  sf::Vector2f g[3] = {GfH(h(hc[0], 8)), GfH(h(hc[1], 8)), GfH(h(hc[2], 8))}; //gradients

  double n0, n1, n2;//contributions
  double t0(0.5-x0*x0-y0*y0); //distance a l'angle (sort of)
  if (t0 < 0) n0 = 0; //si trop loin osef
  else //sinon plus c'est loin plus osef
    t0 *= t0,
    n0 = t0 * t0 * dotProduct(g[0], {x0, y0});
  double t1(0.5-x1*x1-y1*y1); //pareil pour le deuxieme angle
  if (t1 < 0) n1 = 0;
  else
    t1 *= t1,
    n1 = t1 * t1 * dotProduct(g[1], {x1, y1});
  double t2(0.5-x2*x2-y2*y2); //et le troisieme
  if (t2<0) n2 = 0;
  else
    t2 *= t2,
    n2 = t2 * t2 * dotProduct(g[2], {x2, y2});

  // if (t0 > 0.2499 or t1 > 0.2499 or t2 > 0.2499) return std::numeric_limits<double>::infinity();
  // if ((n0 == 0 and n1 == 0) or (n2 == 0 and n1 == 0) or (n0 == 0 and n2 == 0)) return -std::numeric_limits<double>::infinity();
  return 42*(n0+n1+n2); //scale entre [-1;1]
}


inline uint64_t hash(uint8_t *i, size_t size)
{
  uint64_t h = 0xcbf29ce484222325; //FNV hash
  for (size_t k = 0; k < size; k++) {
    h *= 0x100000001b3;
    h = h ^ i[k];
  }
  return h;
}

inline uint64_t Noise::h(int32_t i[], size_t size) const
{
  uint8_t *c = new uint8_t[size+4];
  memcpy(c, i, size);
  memcpy(c+size, &m_seed, 4);
  auto ha = hash(c, size+4);
  delete[] c;
  return ha;
}

inline double cubic(double x)
{
  return x*x*(3-2*x);
}

inline double cubic(double a1, double a2, double w)
{
  w = (w*w)*(3-2*w);
  return a1*(1-w)+a2*(w);
}

inline double dotProduct(double xa, double ya, double xb, double yb)
{
  return xa*xb+ya*yb;
}

inline double dotProduct(sf::Vector2f a, sf::Vector2f b)
{
  return dotProduct(a.x, a.y, b.x, b.y);
}

inline sf::Vector2f GfH(uint64_t h)
{
  return sf::Vector2f(cosf(h), sinf(h));
}

inline int32_t fastfloor(double fp)
{
  return (fp < 0) ? (int32_t)fp -1 : (int32_t)fp;
}
