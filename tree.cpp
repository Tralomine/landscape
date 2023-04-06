#include <iostream>
#include <cmath>

#include "tree.hpp"

sf::Vector2f getBezier(std::vector<sf::Vector2f> points, float adv)
{
  std::vector<sf::Vector2f> v;
  while (points.size() > 2) {
    v.empty();
    for (size_t i = 0; i < points.size()-1; i++) {
      v.push_back(points[i] + (points[i+1] - points[i]) * adv);
    }
    points = v;
  }
  return points[0] + (points[1] - points[0]) * adv;
}

sf::Vector2f getCyclicBezier(std::vector<sf::Vector2f> points, float adv, int i)
{
  std::vector<sf::Vector2f> v(3);
  size_t s = points.size();
  v[1] = points[(i+1)%s];
  v[0] = 0.5f*(points[i%s]+v[1]);
  v[2] = 0.5f*(points[(i+2)%s]+v[1]);

  return getBezier(v, adv);
}

std::vector<sf::Vector2f> getBlob(size_t size, size_t b, size_t n)
{
  std::vector<sf::Vector2f> v;
  for (size_t i = 0; i < b; i++) {
    v.push_back({size*cosf(2*pi/b*i) + rand()%(size/2) - size/4,
                 size*sinf(2*pi/b*i) + rand()%(size/2) - size/4});
  }

  std::vector<sf::Vector2f> blob;
  for (size_t i = 0; i < b; i++) {
    float adv(0);
    while (adv < 1) {
      blob.push_back(getCyclicBezier(v, adv, i));
      adv += 1.f/n;
    }
  }
  // blob.push_back({getCyclicBezier(v, 1, v.size()-1)});

  return blob;
}


sf::VertexArray getRandomTree(float size)
{
  return rand()%5>1?getDeciduous(size):getConiferous(size);
}

sf::VertexArray getDeciduous(float size)
{
  sf::VertexArray t(sf::Triangles);//51

  float height = (rand()%12+20)/4.f*size;
  //trunk
  t.append({{-size/5.f, 0}});
  t.append({{size/5.f, 0}});
  t.append({{0, -height}});

  //leaves
  for (int i = 5; i <= 20; i++) {
    if (rand()%400 < i*i) {
      size_t leaves = rand()%(i/5)+1;
      for (size_t k = 0; k < leaves; k++) {
        float x = 20.f/(20+i)*(rand()%(int)(height/2)-(height/4)), y = -height/20*i;

        // t[(i-4)*3] = {{x, y}};   //branches
        // t[(i-4)*3+1] = {{0, y+(height/10)}};
        // t[(i-4)*3+2] = {{0, y+(85-i)*(height/600)}};

        auto blob = getBlob(size, 12+rand()%5, 4);
        for (size_t j = 0; j < blob.size()-1; j++) {
          t.append({sf::Vector2f(x, y)});
          t.append({sf::Vector2f(x+blob[j].x, y+0.5*blob[j].y)});
          t.append({sf::Vector2f(x+blob[j+1].x, y+0.5*blob[j+1].y)});
        }
        t.append({sf::Vector2f(x, y)});
        t.append({sf::Vector2f(x+blob[blob.size()-1].x, y+0.5*blob[blob.size()-1].y)});
        t.append({sf::Vector2f(x+blob[0].x, y+0.5*blob[0].y)});
      }
    }
  }

  return t;
}

sf::VertexArray getConiferous(float size)
{
  float height = (rand()%12+20)/4.f*size; // ~ 5-8 x size -> 110-176
  float spike = height/12+rand()%12;
  float shift = height/25;
  float fall = 1.5f + rand()%15/10.f;

  std::vector<sf::Vector2f> v;
  for (float i = 0; i <= height; i += spike) {
    v.push_back({-0.4f*height+0.3f*i, -i+fall*spike});
    v.push_back({-0.2f*height+0.2f*i, -i});
  }
  for (float i = height; i >= 0; i -= spike) {
    v.push_back({0.2f*height-0.2f*i, -i});
    v.push_back({0.4f*height-0.3f*i, -i+fall*spike});
  }

  for (size_t i = 0; i < v.size(); i++) {
    v[i] += {rand()%(int)(shift*100)/100.f-shift/2, rand()%(int)(shift*100)/100.f-shift/2};
  }

  std::vector<sf::Vector2f> v2;
  for (size_t i = 0; i < v.size(); i++) {
    float adv(0);
    while (adv < 1) {
      v2.push_back(getCyclicBezier(v, adv, i));
      adv += 0.25;
    }
  }

  sf::VertexArray t(sf::Triangles);
  for (size_t j = 0; j < v2.size()-1; j++) {
    t.append({{0, v2[j].y-12}});
    t.append({v2[j]});
    t.append({v2[j+1]});
    t.append({{0, v2[j].y-12}});
    t.append({{0, v2[j+1].y-12}});
    t.append({v2[j+1]});
  }
  t.append({{0, v2[v2.size()-1].y-12}});
  t.append({v2[v2.size()-1]});
  t.append({v2[0]});
  t.append({{0, v2[v2.size()-1].y-12}});
  t.append({{0, v2[0].y-12}});
  t.append({v2[0]});

  return t;
}
