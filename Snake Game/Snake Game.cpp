#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>

struct Vec2 {
    int x, y;
};

struct Data {
    int Width = 1280;
    int Height = 720;
    const int RectSize = 20;
    const size_t N = Width / RectSize;
    const size_t M = Height / RectSize;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(Width, Height), "Snake Game");
};


class MyRects
{
public:
    MyRects(Data _data) : data(_data){
        InitRects();
    }
    ~MyRects() {
        for (unsigned int x = 0; x < data.N; x++)
            delete[] _rect[x];

        delete[] _rect;
    }

    void DrawRect() {
        for (int i = 0; i < data.N; i++) {
            for (int j = 0; j < data.M; j++) {
                data.window->draw(_rect[i][j]);
            }
        }
    }
    
    void ChangeColor(int i,int j, sf::Color color) {
        _rect[i][j].setFillColor(color);
    }


    void ResetBoard() {
        for (int i = 0; i < data.N; i++) {
            for (int j = 0; j < data.M; j++) {
                _rect[i][j].setFillColor(sf::Color::White);
                if (i == data.N - 1 || i == 0 || j == data.M - 1 || j == 0) {
                    _rect[i][j].setFillColor(sf::Color::Black);
                }
            }
        }
    }


    sf::RectangleShape** _rect;
private:
    Data data;

    void InitRects() {
        _rect = new sf::RectangleShape* [data.N];
        for (int x = 0; x < data.N; x++) {
            _rect[x] = new sf::RectangleShape[data.N];
        }
        for (int i = 0; i < data.N; i++) {
            for (int j = 0; j < data.M; j++) {
                _rect[i][j] = sf::RectangleShape();
                _rect[i][j].setSize(sf::Vector2f(data.RectSize, data.RectSize));
                _rect[i][j].setPosition(sf::Vector2f(i * data.RectSize, j * data.RectSize));
                _rect[i][j].setOutlineColor(sf::Color::Black);
                _rect[i][j].setOutlineThickness(1);
                if (i == data.N - 1 || i == 0 || j == data.M - 1 || j == 0) {
                    _rect[i][j].setFillColor(sf::Color::Black);
                }
            }
        }
    }

};

struct Apple {
public:
    void InitApple(int x, int y) {
        ApplePos.x = rand() % x - 1 + 1;
        ApplePos.y = rand() % y - 1 + 1;
    }

    void EatApple(int x, int y) {
        //ChangeColor(ApplePos.x, ApplePos.y, sf::Color::White);
        ApplePos.x = rand() % x - 1 + 1;
        ApplePos.y = rand() % y - 1 + 1;
        //ChangeColor(ApplePos.x, ApplePos.y, sf::Color::Green);
    }

    Vec2 ApplePosition() {
        return ApplePos;
    }

private:
    //Data data;
    Vec2 ApplePos;

};





class Player : private MyRects {
public:
    Player(Data _data) : MyRects(_data), data(_data){
        InitPlayer();
        apple.InitApple(data.N, data.M);
        ChangeColor(apple.ApplePosition().x, apple.ApplePosition().y, sf::Color::Green);
    }
    ~Player() {}

    void InitPlayer() {
        PlayerPos.x = data.N / 2;
        PlayerPos.y = data.M / 2;

        ChangeColor(PlayerPos.x, PlayerPos.y, sf::Color::Red);
    }

    void MovePlayer(int dir) {
        CurrentDirection = dir;
    }

    void UpdateApple() {
        if (PlayerPos.x == apple.ApplePosition().x && PlayerPos.y == apple.ApplePosition().y) {
            IncreaseSnakeSize();
            ChangeColor(apple.ApplePosition().x, apple.ApplePosition().y, sf::Color::White);
            apple.EatApple(data.N, data.M);
            ChangeColor(apple.ApplePosition().x, apple.ApplePosition().y, sf::Color::Green);
        }
        ChangeColor(apple.ApplePosition().x, apple.ApplePosition().y, sf::Color::Green);
    }

    void Update(){
        if (PlayerPos.x == data.N || PlayerPos.x == 0 || PlayerPos.y == data.M || PlayerPos.y == 0) {
            Reset();
            return;
        }
        if (DeathCheck()) {
            Reset();
            return;
        }


        sf::Time elapsed = clock.getElapsedTime();

        if (elapsed.asMilliseconds() < UpdateSpeed)
            return;
        else
            clock.restart();
        

        OldPos = PlayerPos;
        ChangeColor(PlayerPos.x, PlayerPos.y, sf::Color::White);
        UpdateTail();
        switch (CurrentDirection)
        {
        case Left:
            PlayerPos.x--;
            break;
        case Right:
            PlayerPos.x++;
            break;
        case Up:
            PlayerPos.y--;
            break;
        case Down:
            PlayerPos.y++;
            break;
        default:
            break;
        }
        ChangeColor(PlayerPos.x, PlayerPos.y, sf::Color::Red);
    }

    enum Dir {
        None = 0,
        Left = 1,
        Right = 2,
        Up = 3,
        Down = 4
    };

    bool DeathCheck() {
        for (size_t i = 0; i < tail.size(); i++)
        {
            if (PlayerPos.x == tail[i].x && PlayerPos.y == tail[i].y) {
                return true;
            }
        }
        return false;
    }

    void Reset() {
        ChangeColor(PlayerPos.x, PlayerPos.y, sf::Color::White);
        PlayerPos.x = data.N / 2;
        PlayerPos.y = data.M / 2;
        ChangeColor(PlayerPos.x, PlayerPos.y, sf::Color::Red);
        if (tail.size() > 0) {
            for (size_t j = 0; j < tail.size() - 1; j++) {
                ChangeColor(tail[j].x, tail[j].y, sf::Color::White);
            }
            TailLength = 0;
            tail.erase(tail.begin(), tail.end());
        }
        ResetBoard();
        
    }


    void UpdateTail() {
        if (tail.size() == 0) {
            return;
        }
        ChangeColor(tail[0].x, tail[0].y, sf::Color::White);
        tail.push_back(OldPos);
        tail.erase(tail.begin());
        for (size_t i = 0; i < tail.size(); i++)
        {
           ChangeColor(tail[i].x, tail[i].y, sf::Color::White);
           ChangeColor(tail[i].x, tail[i].y, sf::Color::Red);
        }
        
    }


    void IncreaseSnakeSize() {
        if (tail.size() <= TailLength) {
            TailLength++;
            tail.push_back(Vec2(OldPos));
        }
        
    }


    void Draw() { DrawRect();}

   

private:
    Data data;
    Apple apple;
    Vec2 PlayerPos;
    Vec2 OldPos;
    int CurrentDirection;
    int UpdateSpeed = 100;
    sf::Clock clock;
    std::vector<Vec2> tail;
    int TailLength = 0;
};


int main()
{
    srand(time(NULL));

    Data data;
    Player p(data);
   


    data.window->setFramerateLimit(60);

    while (data.window->isOpen())
    {
        sf::Event event;
        while (data.window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                data.window->close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::W)
                    p.MovePlayer(p.Up);
                if (event.key.code == sf::Keyboard::A)
                    p.MovePlayer(p.Left);
                if (event.key.code == sf::Keyboard::S)
                    p.MovePlayer(p.Down);
                if (event.key.code == sf::Keyboard::D)
                    p.MovePlayer(p.Right);
                if (event.key.code == sf::Keyboard::Space)
                    p.IncreaseSnakeSize();
                break;
            default:
                break;
            }
        }
        
        data.window->clear();
        p.Draw();
        data.window->display();
        p.Update();
        p.UpdateApple();
    }

   



    return 0;
}