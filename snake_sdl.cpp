#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <set>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

bool gameOver;
bool fruitAvailable;
const int HEIGHT=20;
const int WIDTH=20;
const int SCOREHEIGHT=HEIGHT;
const int SCOREWIDTH=7;
const int SCORE_X_OFFSET = WIDTH;
const int SCALE=20;
enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction dir;
Direction lastDir;
int score = 0;
int highScore = 0;

std::vector<std::pair<int, int>> snake; // Snake body
std::vector<std::pair<int, int>> fruit; // Fruit

// Define functions
void DrawBoard(SDL_Renderer* renderer);
void DrawSnake(SDL_Renderer* renderer);
void DrawScoreboard(SDL_Renderer* renderer, int score);
void MoveSnake();
void CreateFruit(SDL_Renderer* renderer);
void UserInput(SDL_Event& event);
void Logic();
void Setup();
void Teardown(SDL_Renderer* renderer, SDL_Window* window);
void LoadHighScore();
void WriteHighScore();

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Snake", 
                                            100, 
                                            100, 
                                            (WIDTH + SCOREWIDTH) * SCALE, 
                                            HEIGHT * SCALE, 
                                            SDL_WINDOW_ALLOW_HIGHDPI);
    // Check window is created
    if (!window)
    {
        cerr << "Could not create window: %s\n", SDL_GetError();
        return 1;
    }
    // Check TTF is initialised
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    cout << "Creating Render";
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 
                            -1,
                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    cout << "Creating Boards";

    SDL_Event windowEvent;

    Setup();

    while(!gameOver) {
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT) {
                gameOver = true; // Exit game loop
            } else if (windowEvent.type == SDL_KEYDOWN) {
                UserInput(windowEvent); // Handle user input
            }
        }
        SDL_RenderClear(renderer);
        DrawBoard(renderer);
        DrawScoreboard(renderer, score);
        DrawSnake(renderer);
        MoveSnake();
        CreateFruit(renderer);
        Logic();
        SDL_RenderPresent(renderer);
        SDL_Delay(80);
    }

    Teardown(renderer, window);
    return 0;
 }

void Setup() {
    gameOver = false;
    fruitAvailable = false;
    dir = STOP;
    lastDir = STOP;
    score = 0;
    // Set starting position of the snake at the center of the grid
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2}); // Initial snake head

    // Seed random number generator
    srand(time(0));

    // Load highScore
    LoadHighScore();
}

void Teardown(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void DrawBoard(SDL_Renderer* renderer) {
    // Draw the background 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Black
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            SDL_Rect rect = {i * SCALE, j * SCALE, SCALE, SCALE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void DrawScoreboard(SDL_Renderer* renderer, int score) {
    // Draw the scoreboard background
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light grey
    SDL_Rect scoreboard = {
        SCORE_X_OFFSET * SCALE,   // X position (right of grid)
        0,                        // Y position (top of screen)
        SCOREWIDTH * SCALE,       // Width of the scoreboard
        SCOREHEIGHT * SCALE       // Height of the scoreboard
    };
    SDL_RenderFillRect(renderer, &scoreboard);

    // Render the score text
    string scoreText = "Score: " + std::to_string(score);
    
    // Render the highScore text
    string highScoreText = "High Score: " + std::to_string(highScore);

    // Load font
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return;  
    }

    // Create text surface with the score and highScore
    SDL_Color textColor = {0, 0, 0, 255}; // Black color for text
    SDL_Surface* scoreTextSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Surface* highScoreTextSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
    if (!scoreTextSurface || !highScoreTextSurface) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    // Create texture from the surface
    SDL_Texture* scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
    SDL_Texture* highScoreTextTexture = SDL_CreateTextureFromSurface(renderer, highScoreTextSurface);

    if (!scoreTextTexture || !highScoreTextTexture) {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(scoreTextSurface);
        TTF_CloseFont(font);
        return;
    }

    // Get the width and height of the text
    int textWidth = scoreTextSurface->w;
    int textHeight = scoreTextSurface->h;

    // Define where to render the score text
    SDL_Rect scoreTextRect = {
        SCORE_X_OFFSET * SCALE + 10, // Add some padding from the left
        10,                          // Padding from the top
        textWidth,                   // Width of the text
        textHeight                   // Height of the text
    };

    // Define where to render the highScore text
    SDL_Rect highScoreTextRect = {
        SCORE_X_OFFSET * SCALE + 10, // Add some padding from the left
        40,                          // Padding from the top
        textWidth + 30,              // Width of the text
        textHeight                   // Height of the text
    };

    // Render the text
    SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);
    SDL_RenderCopy(renderer, highScoreTextTexture, NULL, &highScoreTextRect);

    // Clean up
    SDL_FreeSurface(scoreTextSurface);
    SDL_FreeSurface(highScoreTextSurface);
    SDL_DestroyTexture(scoreTextTexture);
    SDL_DestroyTexture(highScoreTextTexture);
    TTF_CloseFont(font);
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
        score++;
    }

    // Check if score exceeds high score
    if(score > highScore) {
        highScore = score;
        WriteHighScore();
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

// Writes the highscore variable to the game_data.txt file.
void WriteHighScore() {
        ofstream outFile("game_data.txt");
        outFile << "Highscore: " << highScore;
        outFile.close();
}

// Loads the stored highscore value from game_data.txt into the highScore variable
void LoadHighScore() {
    ifstream inFile("game_data.txt");
    if (inFile) {
        string line;
        if (getline(inFile, line)) {
            size_t pos = line.find(":");    // finds position of the first occurence of :
            if (pos != string::npos) { // if char is not not found (confusing aye right?!)
                highScore = stoi(line.substr(pos + 1)); //convert from string to int
            }
        }
    }
}