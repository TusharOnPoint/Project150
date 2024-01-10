#include <iostream>
#include <vector>
#include <string>
#include "include/SDL.h"
#undef main

using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20;
const int INITIAL_LENGTH = 5;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

struct Snake {
    vector<pair<int, int>> body;
    char direction;
};

struct Food {
    int x, y;
};

Snake snake;
Food food;

bool quit = false;
bool pause = false;

void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    snake.body.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.direction = 'R';

    food.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void generateFood() {
    food.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void processInput (){
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (snake.direction != 'D') snake.direction = 'U';
                        break;
                    case SDLK_DOWN:
                        if (snake.direction != 'U') snake.direction = 'D';
                        break;
                    case SDLK_LEFT:
                        if (snake.direction != 'R') snake.direction = 'L';
                        break;
                    case SDLK_RIGHT:
                        if (snake.direction != 'L') snake.direction = 'R';
                        break;
                    case SDLK_SPACE:
                        pause = !pause;
                        break;
                }
            }
        }
}

void update() {
    pair<int, int> newHead = snake.body.front();

    switch (snake.direction) {
        case 'U':
            newHead.second -= GRID_SIZE;
            break;
        case 'D':
            newHead.second += GRID_SIZE;
            break;
        case 'L':
            newHead.first -= GRID_SIZE;
            break;
        case 'R':
            newHead.first += GRID_SIZE;
            break;
    }

    snake.body.insert(snake.body.begin(), newHead);

    if (newHead.first == food.x && newHead.second == food.y) {
        generateFood();
    } else {
        snake.body.pop_back();
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 202, 74, 255);
    SDL_Rect obs = {60, 60, 3*GRID_SIZE, 8*GRID_SIZE};
    SDL_RenderFillRect(renderer, &obs);
    SDL_Rect obs1 = {60, 60, 8*GRID_SIZE, 3*GRID_SIZE};
    SDL_RenderFillRect(renderer, &obs1);
    SDL_Rect obs2 = {SCREEN_WIDTH-60-(3*GRID_SIZE), SCREEN_HEIGHT-60-(8*GRID_SIZE), 3*GRID_SIZE, 8*GRID_SIZE};
    SDL_RenderFillRect(renderer, &obs2);
    SDL_Rect obs3 = {SCREEN_WIDTH-60, SCREEN_HEIGHT-60-3*GRID_SIZE, -8*GRID_SIZE, 3*GRID_SIZE};
    SDL_RenderFillRect(renderer, &obs3);
    SDL_Rect obs4 = {(SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE, ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE), 3*GRID_SIZE, 12*GRID_SIZE};
    SDL_RenderFillRect(renderer, &obs4);
    SDL_Rect obs5 = {((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE, ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE), 11*GRID_SIZE, 3*GRID_SIZE};
    SDL_RenderFillRect(renderer, &obs5);
    
    
    // Render Snake
    SDL_SetRenderDrawColor(renderer, 242, 243, 245, 0);
    for (const auto& segment : snake.body) {
        SDL_Rect rect = {segment.first, segment.second, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Render Food
    SDL_SetRenderDrawColor(renderer, 255, 174, 66, 255);
    SDL_Rect foodRect = {food.x, food.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_RenderPresent(renderer);
}

bool checkCollision() {
    // Check collision with screen boundaries
    if (snake.body.front().first < 0 || snake.body.front().first >= SCREEN_WIDTH ||
        snake.body.front().second < 0 || snake.body.front().second >= SCREEN_HEIGHT) {
        cout << "Game Over! Collision with screen boundaries." << endl;
        return true;
    }
    // Check collision with itself
    for (size_t i = 1; i < snake.body.size(); ++i) {
        if (snake.body.front().first == snake.body[i].first && snake.body.front().second == snake.body[i].second) {
            cout << "Game Over! Collision with itself." << endl;
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[])
{
    initialize();

    while (!quit) {
        processInput ();
        update();
        if(checkCollision())
            break;
        render();

        SDL_Delay(100);
    }

    close();
    return 0;
}