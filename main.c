#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

int main() {
    bool done = false;
    char input[256] = "";
    size_t inputSize = 0;

    SDL_Init(SDL_INIT_VIDEO);

    // Window creation
    SDL_Window *pwindow = SDL_CreateWindow("Video downloader", 900, 600, 0);
    SDL_SetWindowPosition(pwindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, NULL);

    //Checking if the window and the renderer has been created
    if (pwindow == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window creation is impossible: %s\n", SDL_GetError());
        return 1;
    }

    if (prenderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer creation is impossible: %s\n", SDL_GetError());
    }

    //Enabling text input in the window
    SDL_StartTextInput(pwindow);
    const SDL_FRect inputBox = {50,50, 300, 40};

    //Init of TTF and checking if there's an error
    if (TTF_Init() == 0) {
        SDL_Log("TTF_Init error");
        return 1;
    }

    //Loading font
    //Adjust the font by the one you want (this one works only on macOS)
    TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/SFNS.ttf", 18);
    if (!font) {
        SDL_Log("Impossible to open the font");
        return 1;
    }

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) { //Event loop
            switch (event.type) {
                case SDL_EVENT_QUIT: // If the user quit the app
                    done = true;
                    break;
                case SDL_EVENT_TEXT_INPUT: // If the user is typing something
                    //Check if there isnt any buffer overflow
                    if (inputSize + SDL_strlen(event.text.text) < sizeof(input) - 1) {
                        SDL_strlcat(input, event.text.text, sizeof(input));
                        inputSize = SDL_strlen(input); // Updating the input size
                    }
                    break;
                case SDL_EVENT_KEY_DOWN: // If the user is deleting text
                    if (event.key.key == SDLK_BACKSPACE && inputSize > 0 ) {
                        input[--inputSize] = '\0'; //Deleting last character
                    }
                    break;
                default: // Default event (ignored)
                    break;
            }

            // Background color of the window
            SDL_SetRenderDrawColor(prenderer, 30, 30, 30, 255);
            SDL_RenderClear(prenderer);

            // Background color of the input
            SDL_SetRenderDrawColor(prenderer, 255, 255, 255, 255);
            SDL_RenderFillRect(prenderer, &inputBox);

            // Outline of the input
            SDL_SetRenderDrawColor(prenderer, 0, 0, 0, 255);
            SDL_RenderRect(prenderer, &inputBox);

            //If there's text then shows it
            if (inputSize > 0) {
                const SDL_Color couleurTexte = {0, 0, 0, 255};
                SDL_Surface *psurface = TTF_RenderText_Blended(font, input, inputSize, couleurTexte);
                SDL_Texture *ptexture = SDL_CreateTextureFromSurface(prenderer, psurface);

                //Size and position of the texte inside the textinput
                SDL_FRect dst = {inputBox.x + 5, inputBox.y + 5, psurface->w, psurface->h};
                SDL_RenderTexture(prenderer, ptexture, NULL, &dst);

                SDL_DestroySurface(psurface);
                SDL_DestroyTexture(ptexture);
            }

            SDL_RenderPresent(prenderer);
        }
    }

    // Clean everything
    TTF_Quit();
    SDL_StopTextInput(pwindow);
    SDL_DestroyRenderer(prenderer);
    SDL_DestroyWindow(pwindow);
    SDL_Quit();

    return 0;
}