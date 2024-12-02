#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <set>

using namespace std;

bool gameOver;
bool fruitAvailable;
const int height=20;
const int width=40;
int x,y,fx,fy,score;
enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction dir;
Direction lastDir;

std::vector<std::pair<int, int>> snake; // Snake body
std::vector<std::pair<int, int>> fruit; // Fruit
std::set<std::pair<int, int>> unique_pairs;

void DrawGrid();
void UserInput();
void gotoxy(int x, int y);

void Setup(){
    gameOver = false;
    x = width / 2;
    y = height / 2;
    score = 0;
    dir = STOP; // Wait for input to start
    lastDir = STOP;
    snake = {{width / 2, height / 2}};
    srand(static_cast<unsigned>(time(0)));
    system("CLS");
    gotoxy(width / 2 - 10, height / 2);
    cout << "Press W/A/S/D to Start";
    while (dir == STOP) {
        UserInput(); // Wait for user input
    }
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void DrawGrid(){
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
    for (int i = snake.size() - 1; i > 0; --i) {
        snake[i] = snake[i - 1]; // Set current segment to the position of the previous one
    }

    switch(dir) {
        case LEFT:
            snake[0].first -= 1; // minus one from x
            break;
        case RIGHT:
            snake[0].first += 1; // add one to x
            break;
        case UP:
            snake[0].second -= 1; // add one to y
            break;
        case DOWN:
            snake[0].second += 1; //minus one from y
            break;
        default:
            break; // No movement if dir == STOP
    }
}

void CreateFruit() {
    if (!fruitAvailable) {
        fruit.clear(); // Ensure only one fruit exists
        int x = rand() % (width - 2) + 1;
        int y = rand() % (height - 2) + 1;
        fruit.emplace_back(x, y);
        fruitAvailable = true;
    }
    gotoxy(fruit[0].first, fruit[0].second);
    cout << "*";
}

void Draw(){
    DrawGrid();
    DrawSnake();
    CreateFruit();
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
    // prevent snake going up and down and hitting itself rapidly
    if ((dir == UP && lastDir == DOWN) ||
        (dir == DOWN && lastDir == UP) ||
        (dir == LEFT && lastDir == RIGHT) ||
        (dir == RIGHT && lastDir == LEFT)) {
        dir = lastDir;  // Ignore invalid direction change
    }
    else {
        lastDir = dir;  // Update last valid direction
    }
}

void Logic() {
    // Check if snake hits wall
    if (snake[0].first <= 0 || snake[0].first >= width -1 ||
        snake[0].second <= 0 || snake[0].second >= height - 1) {
        gameOver = true;
    }

    // Check if snake eats itself
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[0] == snake[i]) {
            gameOver = true;
            return;
        }
    }

    // Check if snake eats fruit
    if (snake[0].first == fruit[0].first && snake[0].second == fruit[0].second) {
        snake.push_back(snake.back()); // Grow snake
        fruit.pop_back();              // Remove eaten fruit
        fruitAvailable = false;        // Indicate new fruit is needed
    }
}
 
int main(){

    Setup();

    while(!gameOver){
        Draw();
        UserInput();
        MoveSnake();
        Logic();
        Sleep(80);
    }
 }
