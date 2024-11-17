#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

bool gameOver;
const int height=20;
const int width=40;
int x,y,fx,fy,score;
enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction dir;

std::vector<std::pair<int, int>> snake; // Snake body

void Setup(){
    gameOver = false;
    x = width/2;
    y = height/2;
    score = 0;
    dir = STOP;       // Initial direction
    snake = { {width / 2, height / 2} }; // spawns a snake
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void  DrawGrid(){
    gotoxy(0, 0);
    // draw exterior of grid
    for(int i=0; i < width; i++){
        cout<<"_";
    }cout<<endl;

    for(int i=0; i < height; i++){
        for(int j=0; j < width; j++){
            if(j == 0 || j == width -1){
                cout<<"|";
            }
            else {
                cout<<" ";
            }
        }cout<<endl;
    }
    
    for(int i=0; i < width; i++){
        cout<<"_";
    }cout<<endl;
}

void DrawSnake(){
// Iterate through the snake vector, which stores the snake's body as pairs of (x, y) coordinates.
    for(const auto& segment : snake) {
        int x = segment.first;
        int y = segment.second;

        gotoxy(x, y);        // Move cursor to (x, y)
        std::cout << "#";    // Draw the snake segment
    }     
}

void MoveSnake() {
    for (int i = snake.size() - 1; i >= 0; --i) {
        snake[i] = snake[i - 1]; // Set current segment to the position of the previous one
    }

    switch(dir) {
        case LEFT:
            snake[0].first -= 1; // minus one from x
            break;
        case RIGHT:
            snake[0].first += 1; // add one to x
        case UP:
            snake[0].second += 1; // add one to y
        case DOWN:
            snake[0].second -= 1; //minus one from y
        default:
            break; // No movement if dir == STOP
    }
}

void DrawFruit() {

}

void Draw(){
    system("CLS");  // clear screen
    DrawGrid();
    DrawSnake();
    DrawFruit();
}


void UserInput(){
    // Takes user input
    if (_kbhit()) {
        switch (_getch()) {
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'x': gameOver = true; break; // Exit the game
        }
    }
}

void Logic(){
    // logic around the snakes body

    // if snake hits wall

    // if snake hits itself

    // if snake eats fruit
}
 
 int main(){

    Setup();

    while(!gameOver){
        Draw();
        UserInput();
        MoveSnake();
        Logic();
        _sleep(100);
    }
 }
