#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;

enum Derection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};


struct Cell {
    RectangleShape cell;
    Cell() { cell.setSize(Vector2f(30, 30)); }
};


struct Tail {
    int x;
    int y;
};


void expand_tail(std::vector<Tail> &tail, int x, int y) {
    for (int i = tail.size() - 1; i >= 0; --i) {
        if (i == 0) {
            tail[i].x = x;
            tail[i].y = y;
        }
        else
        {
            tail[i].x = tail[i - 1].x;
            tail[i].y = tail[i - 1].y;
        }
    }
}

int main()
{
    srand(time(0));
    int gx = 30, gy = 30;
    Derection der = Derection::STOP;
    
    RenderWindow window(sf::VideoMode(1280, 720), "SFML window");
    
    std::vector<std::vector<Cell>> grid(600/30);
    for (int i = 0; i < grid.size(); ++i) {
        grid[i].resize(800 / 30);
        for (int j = 0; j < grid[i].size(); ++j) {
            gx += 30;
            grid[i][j].cell.setPosition(gx, gy);

        }
        gy += 30;
        gx = 30;
    }

    unsigned int hX = 10;
    unsigned int hY = 10;

    std::vector<Tail> tail;

    int pX = rand() % 20;
    int pY = rand() % 20;
    int time = 0;
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //logic
        if (!(der == RIGHT || der == LEFT)) {
            if (Keyboard::isKeyPressed(Keyboard::D))
                der = RIGHT;
            if (Keyboard::isKeyPressed(Keyboard::A))
                der = LEFT;
        }
        if (!(der == UP || der == DOWN)) {
            if (Keyboard::isKeyPressed(Keyboard::W))
                der = UP;
            if (Keyboard::isKeyPressed(Keyboard::S))
                der = DOWN;
        }

        if (time == 20) {
            switch (der)
            {
            case UP:
                expand_tail(tail, hX, hY);
                hY -= 1;
                break;
            case DOWN:
                expand_tail(tail, hX, hY);
                hY += 1;
                break;
            case LEFT:
                expand_tail(tail, hX, hY);
                hX -= 1;
                break;
            case RIGHT:
                expand_tail(tail, hX, hY);
                hX += 1;
                break;
            }
            time = 0;
        }
        time++;

        // Clear screen
        window.clear(Color::Black);
       
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                grid[i][j].cell.setFillColor(Color::Black);
                grid[i][j].cell.setOutlineColor(Color::Black);
                if (i == hY && j == hX) {
                    grid[i][j].cell.setFillColor(Color::Red);
                    grid[i][j].cell.setOutlineColor(Color::Red);
                }
                if (i == pY && j == pX)
                    grid[i][j].cell.setFillColor(Color::Yellow);
                if (hY == pY && hX == pX) {
                    grid[i][j].cell.setFillColor(Color::White);
                    pY = rand() % 20;
                    pX = rand() % 20;
                    tail.push_back(Tail{ -10,-10 });
                }
                for (auto& vec : tail) {
                    if (hY == vec.y && hX == vec.x)
                        window.close();
                    if (i == vec.y && j == vec.x) {
                        grid[i][j].cell.setFillColor(Color::Red);
                        grid[i][j].cell.setOutlineColor(Color::Red);
                    }
                }
                window.draw(grid[i][j].cell);

            }
        }
       
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}