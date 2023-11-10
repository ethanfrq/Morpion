#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void openGame() {
    system("./morpion");  
}

void openGitHub() {
    printf("Opening GitHub...\n");
    system("xdg-open https://github.com/ethanfrq");  // Utilisez le lien réel que vous souhaitez ouvrir
}

int main(void) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Menu SDL",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_OPENGL);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("Black_Future.otf", 48);

    if (font == NULL) {
        printf("Could not load font: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "ETHAN", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    textRect.x = 400 - textSurface->w / 2;
    textRect.y = 300 - textSurface->h / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    int quit = 0;

    while (!quit) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                puts("Fin du programme.");
                break;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        openGame();
                        break;

                    case SDLK_2:
                        openGitHub();
                        break;

                    case SDLK_ESCAPE:
                        quit = 1;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Affichage du menu
        char menuText[50];
        sprintf(menuText, "1: PLAY GAME | 2: GITHUB");

        TTF_Font *menuFont = TTF_OpenFont("Black_Future.otf", 24);
        SDL_Surface *menuSurface = TTF_RenderText_Solid(menuFont, menuText, textColor);
        SDL_Texture *menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
        SDL_Rect menuRect = {400 - menuSurface->w / 2, 350, menuSurface->w, menuSurface->h};
        SDL_RenderCopy(renderer, menuTexture, NULL, &menuRect);
        SDL_DestroyTexture(menuTexture);
        SDL_FreeSurface(menuSurface);
        TTF_CloseFont(menuFont);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
