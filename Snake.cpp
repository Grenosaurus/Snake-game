/*
 Snake game for Windows OS (Made: Jauaries).
*/

// C++ packets
#include <iostream>
#include <string>
#include <conio.h> // Windows only
#include <Windows.h> // Windows only
#include <dos.h> // Not found for Linux
#include <time.h>
#include <cstdlib>

// Definitions used in the program
#define MAXCELLSIZE 100
#define MINCELLSIZE 0
#define SLEEPTIME 25
#define MAXFRAMERATEX 119
#define MAXFRAMERATEY 29
#define POINT 10


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;


void goXY(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;

    SetConsoleCursorPosition(console, CursorPosition);
}


// Hidding the cursor (Taken from: http://www.cplusplus.com/forum/beginner/90498/)
void setCursor(bool visible, DWORD size)
{
    if (size == 0)
    {
        size = 20;
    }

    CONSOLE_CURSOR_INFO lpCursor;

    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;

    SetConsoleCursorInfo(console, &lpCursor);
}


// Randomly generates a point where the cells will be
class Point
{
private:
    int x;
    int y;

public:
    Point()
    {
        x = y = POINT;
    }

    Point(int x, int y)
    {
        this -> x = x;
        this -> y = y;
    }

    void setPoint(int x, int y)
    {
        this -> x = x;
        this -> y = y;
    }

    // Draw food
    void drawFood()
    {
        goXY(x, y);

        std::cout << "$" << std::endl;
    }

    // Draws cells
    void drawSnake()
    {
        goXY(x, y);

        std::cout << "o" << std::endl;
    }

    // Erases the cell
    void erase()
    {
        goXY(x, y);

        std::cout << " " << std::endl;
    }

    void moveUP()
    {
        y--;

        if (y < 0)
        {
            y = MAXFRAMERATEY;
        }
    }

    void moveDOWN()
    {
        y++;

        if (y > MAXFRAMERATEY)
        {
            y = 0;
        }
    }

    void moveRIGHT()
    {
        x++;

        if (x > MAXFRAMERATEX)
        {
            x = 0;
        }
    }

    void moveLEFT()
    {
        x--;

        if (x < 0)
        {
            x = MAXFRAMERATEX;
        }
    }

     // Debugging window
    void debug()
    {
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }

    void copyPosition(Point * p)
    {
        p->x = x;
        p->y = y;
    }

    // Get the X-axis value
    int getX()
    {
        return x;
    }

    // Get the Y-axis value
    int getY()
    {
        return y;
    }

    int isEqual(Point * p)
    {
        if (p->x == x && p->y == y)
        {
            return 1;
        }
        return 0;
    }

};



// Generates food and snake cells
class Snake
{
private:
    Point * cell[MAXCELLSIZE];
    Point food; // Food cell
    int cellSize; // Snake size
    int gameState; // State of the game (0 => Game OVER | 1 => Game ON)
    char direction; // direction of the head cell

public:
    Snake() // Might cause overload
    {
        cellSize = 1; // Starting default size
        cell[MINCELLSIZE] = new Point(20, 20);
        
        for (int i = 1; i < MAXCELLSIZE; i++)
        {
            cell[i] = NULL;
        }

        food.setPoint(rand() % MAXFRAMERATEX, rand() % MAXFRAMERATEY);

        gameState = 0;
    }

    // Adds a new cell
    void addCell(int x, int y)
    {
        cell[cellSize++] = new Point(x, y);
    }

    /*
     'Defining cells movement. The 'if' statements block the snake from moving 
     to the opposite direction
    */
    void turnUP()
    {
        if (direction != 's')
        direction = 'w';
    }

    void turnDOWN()
    {   
        if (direction != 'w')
        direction = 's';
    }

    void turnRIGHT()
    {
        if (direction != 'a')
        direction = 'd';
    }

    void turnLEFT()
    {
        if (direction != 'd')
        direction = 'a';
    }

    /*
     Keyboard controls
    */
    void move()
    {
        system("cls"); // Cleans the screen

        if (gameState == 0)
        {
            if (!selfCollision())
            {
                std::cout << "Welcome to Snake Game!" << std::endl;
                std::cout << "Press any key to start!" << std::endl;

                _getch();

                gameState = 1;
                cellSize = 1;
            }

            // If self collision happens
            if (selfCollision())
            {
                std::cout << "Head collided with body!" << std::endl;
                std::cout << "Try again!" << std::endl;

                _getch();

                gameState = 1;
                cellSize = 1;
            }
        }


        // All the cells follow the first cell (head)
        for (int i = cellSize - 1; i > 0; i--)
        {
            cell[i-1]->copyPosition(cell[i]);
        }

        // Controlling the first cell (head)
        switch (direction)
        {
        case 'w':
            cell[MINCELLSIZE]->moveUP();
            break;

        case 's':
            cell[MINCELLSIZE]->moveDOWN();
            break;

        case 'd':
            cell[MINCELLSIZE]->moveRIGHT();
            break;

        case 'a':
            cell[MINCELLSIZE]->moveLEFT();
            break;
        }

        // Game restarts due to collision | Means snake died
        if (selfCollision())
        {
            gameState = 0;
        }

        // Snakes eats the fruit
        if (food.getX() == cell[0]->getX() && food.getY() == cell[0]->getY())
        {
            addCell(0, 0);

            food.setPoint(rand() % MAXFRAMERATEX, rand() % MAXFRAMERATEY);
        }

        // Snake
        for (int i = 0; i < cellSize; i++)
        {
            cell[i]->drawSnake();
        }

        food.drawFood();

        Sleep(SLEEPTIME); // Defiens how fast the operates
    }

    // Debugging
    void debug()
    {
        for (int i = 0; i < cellSize; i++)
        {
            cell[i]->debug();
        }
    }

    // Head collids with rest of the cells
    int selfCollision()
    {
        for (int i = 1; i < cellSize; i++)
        {
            if (cell[MINCELLSIZE]->isEqual(cell[i]))
            {
                return 1;
            }
        }
        return 0;
    }

};




// Main function
int main()
{
    setCursor(0, 0);
    srand((unsigned)time(NULL));

    Snake snake;
    char operation = 'l';

    do
    {
        if (_kbhit())
        {
            operation = _getch();
        }

        switch (operation)
        {
        case 'w':
        case 'W':
            snake.turnUP();
            break;

        case 's':
        case 'S':
            snake.turnDOWN();
            break;

        case 'd':
        case 'D':
            snake.turnRIGHT();
            break;

        case 'a':
        case 'A':
            snake.turnLEFT();
            break;
        }

        snake.move();
    }
    while (operation != 'e');

    return 0;
}

