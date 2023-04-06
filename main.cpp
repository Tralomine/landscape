#include <SFML/Graphics.hpp>
#include <fstream>
#include "json.hpp"
#include "tree.hpp"
#include "noise.hpp"


inline sf::Color jsonArrayToColor(json::Value v)
{
  return sf::Color((int)(v[0]), (int)(v[1]), (int)(v[2]));
}

inline double simplex2O(Noise n, double x, double y)  //2 octaves
{
  return n.getSimplex2D(x, y)*2+n.getSimplex2D(2*x, 2*y);
}


int main(int argc, char const *argv[]) {
  srand(time(NULL));
  int WIDTH = sf::VideoMode::getDesktopMode().width;
  int HEIGHT = sf::VideoMode::getDesktopMode().height;
  int SUNSIZE = WIDTH/15;
  constexpr int STEP = 4;

  Noise n;
  sf::Transform tf = sf::Transform::Identity;

  json::Object palettes;
  {
    std::ifstream fp("palettes.json", std::ios::in | std::ios::binary);
    if (!fp.is_open()) exit(-1);
    std::string str( (std::istreambuf_iterator<char>(fp)),
                     (std::istreambuf_iterator<char>() ) );
    fp.close();
    size_t i(0);
    palettes = json::parse(str, i);
  }

  json::Object palette = palettes["palettes"][rand()%palettes["palettes"].size()];
  sf::Color bgColor = jsonArrayToColor(palette["background"]);
  sf::Color sunColor = jsonArrayToColor(palette["sun"]);
  std::vector<sf::Color> fgColor(palette["mountains"].size());
  for (size_t i = 0; i < fgColor.size(); i++) {
    fgColor[i] = jsonArrayToColor(palette["mountains"][i]);
  }

  bool fullscreen = true;
  sf::ContextSettings settings;
  settings.antialiasingLevel = 4;
  sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "landscape", fullscreen?sf::Style::Fullscreen:sf::Style::None, settings);

  sf::RectangleShape rec({(float)WIDTH, HEIGHT/(2.f+fgColor.size())});
  rec.setFillColor(sunColor);
  app.draw(rec);
  rec.move(0, HEIGHT/(2+fgColor.size()));
  rec.setFillColor(bgColor);
  app.draw(rec);
  for (size_t i = 0; i < 6; i++) {
    rec.move(0, HEIGHT/(2+fgColor.size()));
    rec.setFillColor(fgColor[i]);
    app.draw(rec);
  }
  app.display();

  sf::CircleShape sun(SUNSIZE, 100);
  sun.setFillColor(sunColor);
  sun.setOrigin(SUNSIZE, SUNSIZE);
  sun.setPosition(rand()%(WIDTH-2*SUNSIZE)+SUNSIZE, rand()%(HEIGHT/4)+HEIGHT/3);

  sf::VertexArray stars(sf::Points);
  sf::VertexArray moonShade(sf::Points);
  if (palette.find("night") != palette.end() && (bool)palette["night"] == true) {
    sun.move(0, -rand()%(HEIGHT/3));
    for (size_t i = 0; i < 500; i++) {
      stars.append({{(float)(rand()%WIDTH), (float)(rand()%HEIGHT)}, sf::Color::White});
    }
    sf::Color moonShadeColor = jsonArrayToColor(palette["moonShade"]);
    float moonShadeProp = (rand()%1000)/2000.f-0.25f; // -.25 .25
    sf::Vector2f const &sp(sun.getPosition());
    for (int x = -SUNSIZE; x < SUNSIZE; x++) {
      for (int y = -SUNSIZE; y < SUNSIZE; y++) {
        if (x*x+y*y < SUNSIZE*SUNSIZE && simplex2O(n, x, y) > moonShadeProp) {
          moonShade.append({{x+sp.x, y+sp.y}, moonShadeColor});
        }
      }
    }
    /**
    moonShade.setPrimitiveType(sf::Triangles);
    for (int x = -SUNSIZE; x < SUNSIZE; x+=STEP) {
      for (int y = -SUNSIZE; y < SUNSIZE; y+=STEP) {
        if (x*x+y*y < (SUNSIZE+STEP)*(SUNSIZE+STEP)) {
          if (simplex2O(n, x, y) > moonShadeProp &&
              simplex2O(n, x+STEP, y+STEP) > moonShadeProp) {
            if (simplex2O(n, x+STEP, y) > moonShadeProp) {
              moonShade.append({{x+sp.x, y+sp.y}, moonShadeColor});
              moonShade.append({{x+sp.x+STEP, y+sp.y}, moonShadeColor});
              moonShade.append({{x+sp.x+STEP, y+sp.y+STEP}, moonShadeColor});
            }
            if (simplex2O(n, x, y+STEP) > moonShadeProp) {
              moonShade.append({{x+sp.x, y+sp.y}, moonShadeColor});
              moonShade.append({{x+sp.x, y+sp.y+STEP}, moonShadeColor});
              moonShade.append({{x+sp.x+STEP, y+sp.y+STEP}, moonShadeColor});
            }
          } else if (simplex2O(n, x+STEP, y) > moonShadeProp &&
                     simplex2O(n, x, y+STEP) > moonShadeProp) {
            if (simplex2O(n, x, y) > moonShadeProp) {
              moonShade.append({{x+sp.x, y+sp.y}, moonShadeColor});
              moonShade.append({{x+sp.x+STEP, y+sp.y}, moonShadeColor});
              moonShade.append({{x+sp.x, y+sp.y+STEP}, moonShadeColor});
            }
            if (simplex2O(n, x+STEP, y+STEP) > moonShadeProp) {
              moonShade.append({{x+sp.x+STEP, y+sp.y+STEP}, moonShadeColor});
              moonShade.append({{x+sp.x+STEP, y+sp.y}, moonShadeColor});
              moonShade.append({{x+sp.x, y+sp.y+STEP}, moonShadeColor});
            }
          }
        }
      }
    }
    for (size_t i = 0; i < moonShade.getVertexCount(); i++) {
      sf::Vector2f ms = moonShade[i].position-sp;
      if (ms.x*ms.x+ms.y*ms.y > SUNSIZE*SUNSIZE) {
        double theta(acos(ms.x/SUNSIZE));
        if (ms.y < 0) theta *= -1;
        moonShade[i].position = sf::Vector2f(SUNSIZE*cosf(theta), SUNSIZE*sinf(theta))+sp;
      }
    }**/
  }

  sf::VertexArray bg(sf::TriangleFan);
  bg.append({{0, 0}, bgColor});
  bg.append({{0, (float)HEIGHT}, bgColor});
  bg.append({{(float)WIDTH, (float)HEIGHT}, bgColor});
  bg.append({{(float)WIDTH, 0}, bgColor});

  std::vector<sf::VertexArray> mountains(fgColor.size());
  std::vector<sf::VertexArray> trees(fgColor.size());
  for (size_t j = 0; j < fgColor.size(); j++) {
    int off = rand()%2000 + 2000*j;
    mountains[j].setPrimitiveType(sf::TriangleStrip);
    trees[j].setPrimitiveType(sf::Triangles);
    for (int i(-((j+1)*WIDTH/20.f)); i-STEP < WIDTH + (j+1)*WIDTH/20.f; i += STEP) {
      float height = HEIGHT/4+(HEIGHT/10)*j + (1-0.1f*j)*(HEIGHT/4*n.getPerlin1D((i+off)/((float)WIDTH/8)) + HEIGHT/12*n.getPerlin1D((i+off)/((float)WIDTH/20)) + HEIGHT/120*n.getPerlin1D((i+off)/((float)WIDTH/80)));
      mountains[j].append({{(float)i, (float)HEIGHT}, fgColor[j]});
      mountains[j].append({{(float)i, height}, fgColor[j]});

      if (!(rand()%(3+3*j/2))) {  //tree density
        sf::VertexArray tree = getRandomTree(6+3*j);  // 6+3j = tree size
        int treeSize = (rand()%20+5)*(j+1)/2;
        for (size_t k = 0; k < tree.getVertexCount(); k++) {
          trees[j].append({{tree[k].position.x+i, tree[k].position.y+height+treeSize}, fgColor[j]}); // plant the tree in the ground so the seams of the trunk aren't visibles
        }
      }
    }
  }

  while (app.isOpen()) {
    sf::Event event;
    while (app.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        app.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::F:
            fullscreen = !fullscreen;
            app.create(sf::VideoMode(WIDTH, HEIGHT), "landscape", fullscreen?sf::Style::Fullscreen:sf::Style::Default, settings);
            break;
          case sf::Keyboard::P:
            if (settings.antialiasingLevel < 16) settings.antialiasingLevel++;
            app.create(sf::VideoMode(WIDTH, HEIGHT), "landscape", fullscreen?sf::Style::Fullscreen:sf::Style::Default, settings);
            break;
          case sf::Keyboard::M:
            if (settings.antialiasingLevel > 0) settings.antialiasingLevel--;
            app.create(sf::VideoMode(WIDTH, HEIGHT), "landscape", fullscreen?sf::Style::Fullscreen:sf::Style::Default, settings);
            break;
          default:
            break;
        }
      }
    }

    tf = sf::Transform::Identity;

    app.clear();
    app.draw(bg);
    app.draw(stars);
    app.draw(sun);
    app.draw(moonShade);
    for (size_t i = 0; i < fgColor.size(); i++) {
      tf.translate(0.1*(sf::Mouse::getPosition(app).x-WIDTH/2), 0.02*sf::Mouse::getPosition(app).y);
      app.draw(mountains[i], tf);
      app.draw(trees[i], tf);
    }
    app.display();

  }

  return 0;
}
