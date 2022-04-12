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
    const int X = 720;
    const int Y = 720;
    srand(time(0));
    int gx = 0, gy = 0;
    Derection der = Derection::STOP;
    
    RenderWindow window(sf::VideoMode(X, Y), "SFML window");
    
    std::vector<std::vector<Cell>> grid(Y/30);
    for (int i = 0; i < grid.size(); ++i) {
        grid[i].resize(X / 30);
        
        for (int j = 0; j < grid[i].size(); ++j) {
            grid[i][j].cell.setPosition(gx, gy);
            gx += 30;
        }
        gy += 30;
        gx = 0;
    }

     int hX = 10;
     int hY = 10;

    std::vector<Tail> tail;

    int pX = 1 + rand() % 22;
    int pY = 1 + rand() % 22;
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
       
        
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[y].size(); ++x) {
                grid[y][x].cell.setFillColor(Color::Black);
                grid[y][x].cell.setOutlineColor(Color::Black);
                if (hY <= 0)
                    hY = grid.size() - 1;
                if (hY > grid.size() - 1)
                    hY = 1;
                if (hX <= 0)
                    hX = grid[y].size() - 1;
                if (hX > grid[y].size() - 1)
                    hX = 1;
                if (y == hY && x == hX) {
                    grid[y][x].cell.setFillColor(Color::Red);
                    grid[y][x].cell.setOutlineColor(Color::Red);
                }
                if (y == pY && x == pX)
                    grid[y][x].cell.setFillColor(Color::Yellow);
                if (hY == pY && hX == pX) {
                    grid[y][x].cell.setFillColor(Color::White);
                    pY = 1 + rand() % 17;
                    pX = 1 + rand() % 22;
                    tail.push_back(Tail{ -10,-10 });
                }
                for (auto& vec : tail) {
                    if (hY == vec.y && hX == vec.x)
                        window.close();
                    if (y == vec.y && x == vec.x) {
                        grid[y][x].cell.setFillColor(Color::Red);
                        grid[y][x].cell.setOutlineColor(Color::Red);
                    }
                }
                if (y == 0 || y == grid.size() - 1 || x == 0 || x == X / 30 - 1)
                    grid[y][x].cell.setFillColor(Color::White);
                window.draw(grid[y][x].cell);

            }
        }

        window.display();
    }
    return EXIT_SUCCESS;
}