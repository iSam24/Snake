#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <set>
#include <SDL2/SDL.h>

using namespace std;

bool gameOver;
bool fruitAvailable;
const int HEIGHT=20;
const int WIDTH=40;
const int SCALE=20;
enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction dir;
Direction lastDir;

std::vector<std::pair<int, int>> snake; // Snake body
std::vector<std::pair<int, int>> fruit; // Fruit

// Define functions
void DrawGrid(SDL_Renderer* renderer);
void DrawSnake(SDL_Renderer* renderer);
void MoveSnake();
void CreateFruit(SDL_Renderer* renderer);
void UserInput(SDL_Event& event);
void Logic();
void Setup();


int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Snake", 
                                            100, 
                                            100, 
                                            WIDTH * SCALE, 
                                            HEIGHT * SCALE, 
                                            SDL_WINDOW_ALLOW_HIGHDPI);
    // Check window is created
    if (!window)
    {
        cerr << "Could not create window: %s\n", SDL_GetError();
        return 1;   // Exit if window creation fails
    }


    cout << "Creating Render";
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 
                            -1,
                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    cout << "Creating Boards";

    SDL_Event windowEvent;

    Setup();

    while(!gameOver) {
        SDL_RenderClear(renderer);
        DrawGrid(renderer);
        DrawSnake(renderer);
        UserInput(windowEvent);
        MoveSnake();
        CreateFruit(renderer);
        Logic();
        if ( SDL_PollEvent (&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            { 
                break; // Exit loop if the user tries to close the window
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(80);
    }

    SDL_DestroyWindow (window);
    SDL_Quit();

    return 0;
 }

void Setup() {
    gameOver = false;
    fruitAvailable = false;
    dir = STOP;
    lastDir = STOP;

    // Set starting position of the snake at the center of the grid
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2}); // Initial snake head

    // Seed random number generator
    srand(time(0));
}

void DrawGrid(SDL_Renderer* renderer) {
    // Draw the background grid
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light grey
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            SDL_Rect rect = {i * SCALE, j * SCALE, SCALE, SCALE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void DrawSnake(SDL_Renderer* renderer){
// Iterate through the snake vector, which stores the snake's body as pairs of (x, y) coordinates.
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
    for(const auto& segment : snake) {
        SDL_Rect rect = {segment.first * SCALE, segment.second * SCALE, SCALE, SCALE};
        SDL_RenderFillRect(renderer, &rect);
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

void UserInput(SDL_Event& event){
    // Takes user input
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w: dir = UP; break;
            case SDLK_s: dir = DOWN; break;
            case SDLK_a: dir = LEFT; break;
            case SDLK_d: dir = RIGHT; break;
            case SDLK_x: gameOver = true; break; // Exit the game
            default: break;
            }
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
    if (snake[0].first <= 0 || snake[0].first >= WIDTH -1 ||
        snake[0].second <= 0 || snake[0].second >= HEIGHT - 1) {
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

void CreateFruit(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red Colour
    if (!fruitAvailable) {
        fruit.clear(); // Ensure only one fruit exists
        int x = rand() % (WIDTH - 2) + 1;
        int y = rand() % (HEIGHT - 2) + 1;
        fruit.emplace_back(x, y);
        fruitAvailable = true;
    }
    SDL_Rect rect = {fruit[0].first * SCALE, fruit[0].second * SCALE, SCALE, SCALE};
    SDL_RenderFillRect(renderer, &rect);
}