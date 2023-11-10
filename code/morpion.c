#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define BOARD_SIZE 3
#define CELL_SIZE (WINDOW_WIDTH / BOARD_SIZE)

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int board[BOARD_SIZE][BOARD_SIZE] = {{0}};

bool isGameOver() {
    // Vérifier les lignes et colonnes
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][0] == board[i][2]) {
            return true; // ligne
        }
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[0][i] == board[2][i]) {
            return true; // colonne
        }
    }

    // Vérifier les diagonales
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        return true; // diagonale principale
    }
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[0][2] == board[2][0]) {
        return true; // diagonale inverse
    }

    // Vérifier s'il reste des mouvements possibles
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                return false; // Il reste au moins un mouvement possible
            }
        }
    }

    // Aucun mouvement possible, le jeu est un match nul
    return true;
}

void drawX(int row, int col) {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        int x = col * CELL_SIZE;
        int y = row * CELL_SIZE;

        SDL_RenderDrawLine(renderer, x, y, x + CELL_SIZE, y + CELL_SIZE);
        SDL_RenderDrawLine(renderer, x, y + CELL_SIZE, x + CELL_SIZE, y);
    }
}

void drawO(int row, int col) {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        int x = col * CELL_SIZE + CELL_SIZE / 2;
        int y = row * CELL_SIZE + CELL_SIZE / 2;
        int radius = CELL_SIZE / 2 - 10;

        for (int i = 0; i <= 360; i += 15) {
            double angle = i * 3.14159265 / 180;
            int cx = x + radius * cos(angle);
            int cy = y + radius * sin(angle);
            SDL_RenderDrawPoint(renderer, cx, cy);
        }
    }
}

void drawBoard() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Fond noir
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Lignes blanches
    for (int i = 1; i < BOARD_SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 1) {
                drawX(i, j);
            } else if (board[i][j] == 2) {
                drawO(i, j);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void drawWinner(int player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Fond noir
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    SDL_Rect textRect;

    if (player == 1 || player == 2) {
        char message[20];
        snprintf(message, sizeof(message), "Joueur %d a gagne!", player);

        TTF_Font *font = TTF_OpenFont("./Black_Future.otf", 36);  // Remplacez le chemin par le chemin de votre police de caractères TTF
        surface = TTF_RenderText_Solid(font, message, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        textRect.x = (WINDOW_WIDTH - surface->w) / 2;
        textRect.y = (WINDOW_HEIGHT - surface->h) / 2;
        textRect.w = surface->w;
        textRect.h = surface->h;

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(3000);

        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
    } else {
        // Match nul
        char message[] = "Match nul!";
        TTF_Font *font = TTF_OpenFont("./Black_Future.otf", 36);  // Remplacez le chemin par le chemin de votre police de caractères TTF
        surface = TTF_RenderText_Solid(font, message, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        textRect.x = (WINDOW_WIDTH - surface->w) / 2;
        textRect.y = (WINDOW_HEIGHT - surface->h) / 2;
        textRect.w = surface->w;
        textRect.h = surface->h;

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(3000);

        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
    }

    SDL_DestroyTexture(texture);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("Morpion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    bool quit = false;
    int currentPlayer = 1;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                int row = mouseY / CELL_SIZE;
                int col = mouseX / CELL_SIZE;

                if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == 0) {
                    board[row][col] = currentPlayer;
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;

                    if (isGameOver()) {
                        int winner = 0;
                        for (int i = 1; i <= 2; ++i) {
                            if ((board[0][0] == i && board[1][1] == i && board[2][2] == i) ||
                                (board[0][2] == i && board[1][1] == i && board[2][0] == i)) {
                                winner = i;
                                break;
                            }
                            for (int j = 0; j < BOARD_SIZE; ++j) {
                                if ((board[i][0] == i && board[i][1] == i && board[i][2] == i) ||
                                    (board[0][i] == i && board[1][i] == i && board[2][i] == i)) {
                                    winner = i;
                                    break;
                                }
                            }
                        }
                        drawBoard();
                        drawWinner(winner);
                        quit = true;  // Sortir de la boucle principale après la fin du jeu
                    }

                    drawBoard();
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
