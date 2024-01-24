//This is a simple snake game
//Author: Tushar Das

#include <bits/stdc++.h>
#include "include/SDL.h"
#include "include/SDL_ttf.h"
#include "include/SDL_mixer.h"
#undef main

using namespace std;

const int SCREEN_WIDTH = 945;
const int SCREEN_HEIGHT = 615;
const int GRID_SIZE = 15;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Mix_Chunk* wavFile;
Mix_Chunk* gameOver;

struct Snake {
    vector<pair<int, int>> body;
    char direction;
};

struct Food {
    int x, y;
};

Snake snake;
Food food;
Food sp;

int ptime = 0;
int px=SCREEN_WIDTH/2, py=SCREEN_HEIGHT/2;

bool quit ;
bool pause ;
bool f=true;
int score;
int highScore = 0;
int buttonid;
int o=0;
int regfoodcount = 0;

void drawCirc (int X, int Y, int radius)
{
    while (radius--) {
            for (int angle = 0; angle < 360; angle++) {
                int x = X + radius * cos(angle * M_PI / 180);
                int y = Y + radius * sin(angle * M_PI / 180);
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
}

void generateFood() {
    food.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
    if ((food.x >= 60 && food.x < 60+3*GRID_SIZE &&
        food.y >= 60 && food.y < 60+8*GRID_SIZE) ||
        (food.x >= 60 && food.x < 60+8*GRID_SIZE &&
        food.y >= 60 && food.y < 60+3*GRID_SIZE) ||
        (food.x >= (SCREEN_WIDTH-60-(3*GRID_SIZE)) && food.x < (SCREEN_WIDTH-60-(3*GRID_SIZE))+3*GRID_SIZE &&
        food.y >= (SCREEN_HEIGHT-60-(8*GRID_SIZE)) && food.y < (SCREEN_HEIGHT-60-(8*GRID_SIZE))+8*GRID_SIZE) ||
        (food.x >= (SCREEN_WIDTH-60-(8*GRID_SIZE)) && food.x < (SCREEN_WIDTH-60) &&
        food.y >= (SCREEN_HEIGHT-60-(3*GRID_SIZE)) && food.y < (SCREEN_HEIGHT-60)) ||
        (food.x >= ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE) && food.x < (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)+3*GRID_SIZE) &&
        food.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && food.y < ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE+6*GRID_SIZE)) ||
        (food.x >= (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE) && food.x < ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE+11*GRID_SIZE) &&
        food.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && food.y < (((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE)+3*GRID_SIZE)) 
        {
            generateFood();
        }
}

void generateSpFood() {
    sp.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    sp.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
    if ((sp.x >= 60 && sp.x <= 60+3*GRID_SIZE &&
        sp.y >= 60 && sp.y <= 60+8*GRID_SIZE) ||
        (sp.x >= 60 && sp.x <= 60+8*GRID_SIZE &&
        sp.y >= 60 && sp.y <= 60+3*GRID_SIZE) ||
        (sp.x >= (SCREEN_WIDTH-60-(3*GRID_SIZE)) && sp.x <= (SCREEN_WIDTH-60-(3*GRID_SIZE))+3*GRID_SIZE &&
        sp.y >= (SCREEN_HEIGHT-60-(8*GRID_SIZE)) && sp.y <= (SCREEN_HEIGHT-60-(8*GRID_SIZE))+8*GRID_SIZE) ||
        (sp.x >= (SCREEN_WIDTH-60-(8*GRID_SIZE)) && sp.x <= (SCREEN_WIDTH-60) &&
        sp.y >= (SCREEN_HEIGHT-60-(3*GRID_SIZE)) && sp.y <= (SCREEN_HEIGHT-60)) ||
        (sp.x >= ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE) && sp.x <= (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)+3*GRID_SIZE) &&
        sp.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && sp.y <= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE+6*GRID_SIZE)) ||
        (sp.x >= (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE) && sp.x <= ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE+11*GRID_SIZE) &&
        sp.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && sp.y <= (((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE)+3*GRID_SIZE)) 
        {
            generateSpFood();
        }
        ptime = SDL_GetTicks();
        regfoodcount = 0;
}

void initialize() {

    quit = false;
    pause = false;
    score = 0;
    regfoodcount = 0;
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();

    snake.body.push_back({0, 0});
    snake.body.push_back({1, 0});
    snake.body.push_back({2, 0});
    snake.body.push_back({3, 0});
    snake.direction = 'R';

    sp.x=-100;
    sp.y=-100;
    generateFood();

    
    
}

void close() {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void processInput (){
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                exit(1);
            } 
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (snake.direction != 'D') 
                        {
                            snake.direction = 'U';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_DOWN:
                        if (snake.direction != 'U') 
                        {
                            snake.direction = 'D';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_LEFT:
                        if (snake.direction != 'R') 
                        {
                            snake.direction = 'L';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (snake.direction != 'L') 
                        {
                            snake.direction = 'R';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_SPACE:
                        pause = !pause;
                        break;
                    case SDLK_ESCAPE:
                        exit(0);
                        break;
                }
            }
        }

    int x,y;
    SDL_GetMouseState(&x, &y);
    int dx = px-x, dy = py-y;
    //cout << px << " " << py << endl;
    //cout << x << " " << y << endl;
    //cout << dx << " " << dy << endl;
    //cout << endl;
    if(abs(dx)>abs(dy))
    {
        if(dx>0) 
        {
            if (snake.direction != 'R') 
                    {
                        snake.direction = 'L';
                        if (pause) pause= !pause;
                    }
        }
        else if(dx<0){
            if (snake.direction != 'L') 
                        {
                            snake.direction = 'R';
                            if (pause) pause= !pause;
                        }
        }
    }
    else if(abs(dx)<abs(dy))
    {
        if(dy>0) 
        {
            if (snake.direction != 'D') 
                    {
                        snake.direction = 'U';
                        if (pause) pause= !pause;
                    }
        }
        else if(dy<0){
            if (snake.direction != 'U') 
                        {
                            snake.direction = 'D';
                            if (pause) pause= !pause;
                        }
        }
    }

    px=x;
    py=y;
}

void update() {

    int ctime = SDL_GetTicks();
    if ((ctime-ptime >= 6000))
    {
        f=false;
        sp.x = -100;
        sp.y = -100;
    }
    pair<int, int> newHead = snake.body.front();
    switch (snake.direction) {
        case 'U':
            if (snake.body.front().second > 0)
                newHead.second -= GRID_SIZE;
            else
                newHead.second = SCREEN_HEIGHT-GRID_SIZE;
            break;
        case 'D':
            if (snake.body.front().second >= SCREEN_HEIGHT-GRID_SIZE)
                newHead.second = 0;
            else
                newHead.second += GRID_SIZE;
            break;
        case 'L':
            if (snake.body.front().first > 0)
                newHead.first -= GRID_SIZE;
            else
                newHead.first = SCREEN_WIDTH-GRID_SIZE;
            break;
        case 'R':
            if (snake.body.front().first >= SCREEN_WIDTH-GRID_SIZE)
                newHead.first = 0;
            else
                newHead.first += GRID_SIZE;
            break;
    }

    snake.body.insert(snake.body.begin(), newHead);
    

    if (regfoodcount == 7) 
    { 
        generateSpFood();
    }

    if (newHead.first == food.x && newHead.second == food.y) {
        score++;
        regfoodcount++;
        /*switch (snake.direction) {
        case 'U':
            if (snake.body.front().second > 0)
                newHead.second -= GRID_SIZE;
            else
                newHead.second = SCREEN_HEIGHT-GRID_SIZE;
            break;
        case 'D':
            if (snake.body.front().second >= SCREEN_HEIGHT-GRID_SIZE)
                newHead.second = 0;
            else
                newHead.second += GRID_SIZE;
            break;
        case 'L':
            if (snake.body.front().first > 0)
                newHead.first -= GRID_SIZE;
            else
                newHead.first = SCREEN_WIDTH-GRID_SIZE;
            break;
        case 'R':
            if (snake.body.front().first >= SCREEN_WIDTH-GRID_SIZE)
                newHead.first = 0;
            else
                newHead.first += GRID_SIZE;
            break;
    }
    snake.body.insert(snake.body.begin(), newHead);
        snake.body.pop_back();*/
        Mix_PlayChannel(-1, wavFile, 0);
        generateFood();
    } 
    else if((newHead.first >= sp.x - GRID_SIZE && newHead.first < sp.x + GRID_SIZE) && 
    (newHead.second >= sp.y - GRID_SIZE && newHead.second < sp.y + GRID_SIZE))
    {
        score+=10;
        sp.x= -100;
        sp.y= -100;
        /*switch (snake.direction) {
        case 'U':
            if (snake.body.front().second > 0)
                newHead.second -= GRID_SIZE;
            else
                newHead.second = SCREEN_HEIGHT-GRID_SIZE;
            break;
        case 'D':
            if (snake.body.front().second >= SCREEN_HEIGHT-GRID_SIZE)
                newHead.second = 0;
            else
                newHead.second += GRID_SIZE;
            break;
        case 'L':
            if (snake.body.front().first > 0)
                newHead.first -= GRID_SIZE;
            else
                newHead.first = SCREEN_WIDTH-GRID_SIZE;
            break;
        case 'R':
            if (snake.body.front().first >= SCREEN_WIDTH-GRID_SIZE)
                newHead.first = 0;
            else
                newHead.first += GRID_SIZE;
            break;
    }
    
    snake.body.insert(snake.body.begin(), newHead);
    snake.body.pop_back();*/

    Mix_PlayChannel(-1, wavFile, 0);
    }
    else {
        snake.body.pop_back();
    }
}

void renderScore() {
	SDL_Color Red = { 255, 0, 0 };

	TTF_Font* font = TTF_OpenFont((char*)"score.ttf", 16);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* Score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(score)).c_str(), Red);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, Score);
	SDL_Rect scoreRect;
	scoreRect.w = 160;
	scoreRect.h = 45;
	scoreRect.x = ((SCREEN_WIDTH) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);
}


void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    renderScore();

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
    SDL_SetRenderDrawColor(renderer, 160, 155, 158, 0);
    const auto& head = *snake.body.begin();
    SDL_Rect rect = {head.first, head.second, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 242, 243, 245, 0);
    for (auto it = std::next(snake.body.begin()); it != snake.body.end(); ++it) {
        const auto& segment = *it;
        SDL_Rect rect = {segment.first, segment.second, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Render Food
    SDL_SetRenderDrawColor(renderer, 255, 174, 66, 255);
    SDL_Rect foodRect = {food.x, food.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    drawCirc (sp.x, sp.y, GRID_SIZE);
    //SDL_Rect spRect = {sp.x, sp.y, GRID_SIZE, GRID_SIZE};
    //SDL_RenderFillRect(renderer, &spRect);

    //drawCirc (sp.x, sp.y, 7);

    SDL_RenderPresent(renderer);
}



bool checkCollision() {
    // Check collision with screen boundaries
    /*if (snake.body.front().first < 0 || snake.body.front().first >= SCREEN_WIDTH ||//Tushar
        snake.body.front().second < 0 || snake.body.front().second >= SCREEN_HEIGHT) {
        cout << "Game Over! Collision with screen boundaries." << endl;
        return true;
    }*/

    // Check collision with obstracle
    if ((snake.body.front().first >= 60 && snake.body.front().first < 60+3*GRID_SIZE &&
        snake.body.front().second >= 60 && snake.body.front().second < 60+8*GRID_SIZE) ||
        (snake.body.front().first >= 60 && snake.body.front().first < 60+8*GRID_SIZE &&
        snake.body.front().second >= 60 && snake.body.front().second < 60+3*GRID_SIZE) ||
        (snake.body.front().first >= (SCREEN_WIDTH-60-(3*GRID_SIZE)) && snake.body.front().first < (SCREEN_WIDTH-60-(3*GRID_SIZE))+3*GRID_SIZE &&
        snake.body.front().second >= (SCREEN_HEIGHT-60-(8*GRID_SIZE)) && snake.body.front().second < (SCREEN_HEIGHT-60-(8*GRID_SIZE))+8*GRID_SIZE) ||
        (snake.body.front().first >= (SCREEN_WIDTH-60-(8*GRID_SIZE)) && snake.body.front().first < (SCREEN_WIDTH-60) &&
        snake.body.front().second >= (SCREEN_HEIGHT-60-3*GRID_SIZE) && snake.body.front().second < (SCREEN_HEIGHT-60)) ||
        (snake.body.front().first >= ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE) && snake.body.front().first < (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)+3*GRID_SIZE) &&
        snake.body.front().second >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && snake.body.front().second < ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE+6*GRID_SIZE)) ||
        (snake.body.front().first >= (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE) && snake.body.front().first < ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE+11*GRID_SIZE) && //Tushar
        snake.body.front().second >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && snake.body.front().second < (((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE)+3*GRID_SIZE))
        {
        cout << "Game Over! Collision with screen obstracle." << endl;
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

void displayScore () {
    string scoreMessage = "Your Score is: ";
    scoreMessage = scoreMessage + to_string(score) + ".";
    string msg = scoreMessage + "\nHighest score is: " + to_string(highScore) + ".";

    const SDL_MessageBoxButtonData buttons[] = {
    { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
    { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Replay" },
};

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        NULL,
        "   Game Over!!!",
        msg.c_str(),
        (sizeof(buttons)/sizeof(buttons[0])),
        buttons,
        NULL
    };

    SDL_ShowMessageBox(&messageboxdata, &buttonid);
}

void renderGameOver() {
    //cout<<"Game is over.\n";

    SDL_SetRenderDrawColor(renderer, 90, 100, 70, 255);
    SDL_RenderClear(renderer);

	SDL_Color C = { 255, 255, 255 };

	TTF_Font* font = TTF_OpenFont((char*)"score.ttf", 26);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

    string msg = "GAME OVER!!!";
	SDL_Surface* GO = TTF_RenderText_Solid(font, msg.c_str(), C);
	SDL_Texture* GOMessage = SDL_CreateTextureFromSurface(renderer, GO);
	SDL_Rect GORect;
	GORect.w = 480;
	GORect.h = 180;
	GORect.x = ((SCREEN_WIDTH) / 2) - (GORect.w / 2);
	GORect.y = ((SCREEN_HEIGHT) / 2) - (GORect.h / 2);
	SDL_RenderCopy(renderer, GOMessage, NULL, &GORect);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);

	TTF_CloseFont(font);
}

void runGame(){

    initialize();
    while (!quit) {
        processInput ();
        if (!pause){
            Mix_ResumeMusic();
            update();
        } else
            Mix_PauseMusic();
        if(checkCollision())
        {
            Mix_PlayChannel(-1, gameOver, 0);
            break;
        }

        render();
        SDL_Delay((100-(0.5*score)));
    }
    ifstream inputFile("high_score.txt");

    if (!inputFile.is_open()) {
        cout << "Error opening the file!" << endl;
    }

    inputFile >> highScore;
    inputFile.close();

    if(highScore < score)
    {
        highScore = score;
        ofstream outputFile("high_score.txt");

        if (!outputFile.is_open()) {
            cout << "Error opening the file!" << endl;
        }
        outputFile << highScore << endl;
        outputFile.close();
    }

    renderGameOver();
    SDL_Delay(1000);
    displayScore();
    snake.body.clear();
    if(buttonid == 1)
        runGame();
    close();
} 

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    wavFile = Mix_LoadWAV("eat.wav");
    gameOver = Mix_LoadWAV("gameOver.wav");
    Mix_Music* BGM = Mix_LoadMUS("bgm.mp3");
    Mix_PlayMusic(BGM, -1);
    Mix_VolumeMusic(20);
    runGame();
    return 0;
}

//Name: Tushar Das
//Reg: 2021831003
//Software Engineering, IICT, SUST//This is a simple snake game
//Author: Tushar Das
