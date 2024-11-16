#include <iostream>
#include <vector>
#include <conio.h> // for _kbhit() and _getch()
#include <cstdlib> // for rand()
#include <ctime>   // for srand()

using namespace std;

bool gameOver;

std::vector<std::pair<int, int>> snake; // Snake body
void Setup(){
    gameOver = false;
}

void  DrawSnake(){
    // draws the snakes body
}

void UserInput(){
    // Takes user input
}

void Logic(){
    // logic around the snakes body
}
 
 int main(){

    while(!gameOver){
        Setup();
        DrawSnake();
        UserInput();
        Logic();
        _sleep(100);
    }
 }
