#include <SFML/Graphics.hpp>

using namespace  std;
using namespace sf;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Rysowanie linią (Vertex)");
    window.setFramerateLimit(144);
    vector<Vertex> line ={
        Vertex{Vector2f(10.f, 10.f)},
        Vertex{Vector2f(300.f, 300.f)},
    };

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear();
        // Ta linia jest poprawna
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        window.display();
    }
}
