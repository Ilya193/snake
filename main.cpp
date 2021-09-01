#include <SFML/Graphics.hpp>
#include <random>

using namespace sf;
using namespace std;

std::default_random_engine rng;

class Game
{
private:

    RenderWindow window;
    vector<RectangleShape> snake;
    std::vector<float> posx;
    std::vector<float> posy;
    RectangleShape cube;
    Text txt;

    int dir; // направление
    int c; // ходы
    int k; //кол-во кубов
    int game; // играет ли пользователь

public:
    Game(int x, int y, const char* s) : window(VideoMode(x, y), s), dir(1), c(0), k(0),
      cube(Vector2f(30, 30)), game(0)
    {
        Font font;
        //font.loadFromFile("Hack-Regular.ttf");
        txt.setFont(font);
        txt.setString("Run");
        txt.setFillColor(Color::Black);
        txt.setCharacterSize(24);
        txt.setPosition(50, 50);
    }

    void run()
    {
        rng.seed(std::random_device()());
        Clock clock;
        float time;

        while (window.isOpen())
        {
            time = clock.getElapsedTime().asMicroseconds();
            clock.restart();
            time /= 800;

            event(time);
            update(time);
            render();
        }
    }

    void event(float& time)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                int txtx = txt.getPosition().x + 40;
                int txty = txt.getPosition().y + 25;
                int posx = Mouse::getPosition(window).x;
                int posy = Mouse::getPosition(window).y;

                if (posx >= txtx - 40 and posx <= txtx and posy >= txty - 25 and posy <= txty and game == 0) {
                    game = 1;
                    snake.push_back(RectangleShape(Vector2f(30, 30)));
                    snake[0].setFillColor(Color::Green);
                    cube.setFillColor(Color::Red);
                    cube.setPosition(100, 100);
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                if (game != 0)
                    if (dir != 2)
                        dir = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                if (game != 0)
                    if (dir != 3)
                        dir = 1;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                if (game != 0)
                    if (dir != 0)
                        dir = 2;
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                if (game != 0)
                    if (dir != 1)
                        dir = 3;
            }
        }
    }

    void update(float& time)
    {
        if (game != 0)
        {
            if (snake[0].getPosition().x < 0 or snake[0].getPosition().x > 380 or snake[0].getPosition().y < 0 or snake[0].getPosition().y > 380)
            {
                snake.clear();
                game = 0;
                dir = 1;
            }
            else if (dir == 0)
            {
                int gx = snake[0].getPosition().x;
                int gy = snake[0].getPosition().y;
                posx.push_back(gx);
                posy.push_back(gy);
                snake[0].setPosition(gx, gy - 2);

                for (int i = 1; i < snake.size(); i++)
                    snake[i].setPosition(posx[c - 10 * i], posy[c - 10 * i]);

                c++;
            }
            else if (dir == 1)
            {
                int gx = snake[0].getPosition().x;
                int gy = snake[0].getPosition().y;
                posx.push_back(gx);
                posy.push_back(gy);
                snake[0].setPosition(gx + 2, gy);

                for (int i = 1; i < snake.size(); i++)
                    snake[i].setPosition(posx[c - 10 * i], posy[c - 10 * i]);

                c++;
            }
            else if (dir == 2)
            {
                int gx = snake[0].getPosition().x;
                int gy = snake[0].getPosition().y;
                posx.push_back(gx);
                posy.push_back(gy);
                snake[0].setPosition(gx, gy + 2);

                for (int i = 1; i < snake.size(); i++)
                    snake[i].setPosition(posx[c - 10 * i], posy[c - 10 * i]);

                c++;
            }
            else if (dir == 3)
            {
                int gx = snake[0].getPosition().x;
                int gy = snake[0].getPosition().y;
                posx.push_back(gx);
                posy.push_back(gy);
                snake[0].setPosition(gx - 2, gy);

                for (int i = 1; i < snake.size(); i++)
                    snake[i].setPosition(posx[c - 10 * i], posy[c - 10 * i]);

                c++;
            }

            collision();
        }
    }

    void collision()
    {
        int cx = cube.getPosition().x + 30;
        int cy = cube.getPosition().y + 30;

        int px = snake[0].getPosition().x;
        int py = snake[0].getPosition().y;

        if (px >= cx - 60 and px <= cx and py >= cy - 60 and py <= cy)
        {
            int l1 = random_pos(0, 380);
            int l2 = random_pos(0, 380);
            cube.setPosition(l1, l2);
            snake.push_back(RectangleShape(Vector2f(30, 30)));
            k++;
            snake[k].setFillColor(Color::Green);
        }
    }

    int random_pos(int a, int b)
    {
        std::uniform_int_distribution<int> dist_a_b(a, b);
        return dist_a_b(rng);
    }

    void render()
    {
        window.clear(Color(222, 184, 135));

        if (game == 0)
            window.draw(txt);
        else
        {
            for (const auto& t: snake)
                window.draw(t);

            window.draw(cube);
        }

        window.display();
        sleep(milliseconds(10));
    }
};

int main()
{
    Game game(400, 400, "Snake");
    game.run();
    return 0;
}
