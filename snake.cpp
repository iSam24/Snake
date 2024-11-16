#include <iostream>
#include <vector>
#include <conio.h> // for _kbhit() and _getch()
#include <cstdlib> // for rand()
#include <ctime>   // for srand()

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

}

void  Draw(){
    system("CLS");  // clear screen

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
}
 
 int main(){

    Setup();

    while(!gameOver){
        Draw();
        UserInput();
        Logic();
        _sleep(100);
        gameOver = true;
    }
 }
