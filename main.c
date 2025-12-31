#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>

int video_downloader(char *input);

int main() {
    bool done = false;
    char input[256] = "";
    size_t inputSize = 0;
    char title[] = "LoadTube";
    const SDL_Color titleColor = {255, 255, 255, 255};
    const SDL_Color textColor = {0, 0, 0, 255};

    SDL_Init(SDL_INIT_VIDEO);

    // Window creation
    SDL_Window *pwindow = SDL_CreateWindow("Video downloader", 900, 600, 0);
    SDL_SetWindowPosition(pwindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, NULL);

    //Error check at the init of TTF
    if (!TTF_Init()) {
        SDL_Log("TTF_Init error: %s", SDL_GetError());
        return 1;
    }

    //Loading fonts
    //Adjust the fonts by the one you want (this one works only on macOS)
    TTF_Font *font = TTF_OpenFont("fonts/AovelSansRounded-rdDL.ttf", 18);
    TTF_Font *titleFont = TTF_OpenFont("fonts/AovelSansRounded-rdDL.ttf", 36 );
    if (!font || !titleFont) {
        SDL_Log("Impossible to open the fonts");
        return 1;
    }

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
    int wInput, hInput;
    const float sizeinputX = 400, sizeinputY = 40;
    SDL_GetWindowSize(pwindow, &wInput, &hInput);
    const SDL_FRect inputBox = {(wInput-sizeinputX) / 2.0f, (hInput-sizeinputY) / 2.0f, sizeinputX, sizeinputY};


    //Title creation
    SDL_Surface *ptitleSurface = TTF_RenderText_Blended(titleFont, title, SDL_strlen(title), titleColor);
    SDL_Texture *ptitleTexture = SDL_CreateTextureFromSurface(prenderer, ptitleSurface);
    SDL_FRect titleDst = {(wInput - ptitleSurface->w) / 2.0f,200, ptitleSurface->w, ptitleSurface->h};

    SDL_DestroySurface(ptitleSurface);


    //Button creation
    int wButton, hButton;
    const float sizebuttonX = 100, sizebuttonY = 40;
    SDL_GetWindowSize(pwindow, &wButton, &hButton);
    const SDL_FRect buttonBox = {(wButton - sizebuttonX) / 2.0f, (hButton - sizebuttonY) / 2.0f + 60, sizebuttonX, sizebuttonY};

    //Text of the button
    const char buttonText[] = "Download";
    SDL_Surface *pbuttonSurface = TTF_RenderText_Blended(font, buttonText, SDL_strlen(buttonText), textColor);
    SDL_Texture *pbuttonTexture = SDL_CreateTextureFromSurface(prenderer, pbuttonSurface);
    SDL_FRect buttonTextDst = {buttonBox.x + (buttonBox.w - pbuttonSurface->w) / 2.0f, buttonBox.y + (buttonBox.h - pbuttonSurface->h) / 2.0f, pbuttonSurface->w, pbuttonSurface->h};

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            //Event loop
            switch (event.type) {
                case SDL_EVENT_QUIT: // If the user quit the app
                    done = true;
                    break;
                case SDL_EVENT_TEXT_INPUT: // If the user is typing something
                    //Check if there isn't any buffer overflow
                    if (inputSize + SDL_strlen(event.text.text) < sizeof(input) - 1) {
                        SDL_strlcat(input, event.text.text, sizeof(input));
                        inputSize = SDL_strlen(input); // Updating the input size
                    }
                    break;
                case SDL_EVENT_KEY_DOWN: // If the user is deleting text
                    if (event.key.key == SDLK_BACKSPACE && inputSize > 0 ) {
                        input[--inputSize] = '\0'; //Deleting last character
                    }
                    else if (event.key.key == SDLK_V && (event.key.mod & (SDL_KMOD_CTRL | SDL_KMOD_GUI))) {
                        char *clipboard = SDL_GetClipboardText();
                        if (clipboard) {
                            size_t clipLen = SDL_strlen(clipboard);
                            if (inputSize + clipLen < sizeof(input) - 1) {
                                SDL_strlcat(input, clipboard, sizeof(input));
                                inputSize = SDL_strlen(input);
                            }
                            SDL_free(clipboard);
                        }
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        float mouseX = event.button.x;
                        float mouseY = event.button.y;

                        if (mouseX >= buttonBox.x && mouseX <= buttonBox.x + buttonBox.w && mouseY >= buttonBox.y && mouseY <= buttonBox.y + buttonBox.h) {
                            printf("Video link : %s\n", input);
                            video_downloader(input);
                        }
                    }
                default: // Default event (ignored)
                    break;
            }
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

        // Title "YouTube Downloader" on top of the input
        SDL_RenderTexture(prenderer, ptitleTexture, NULL, &titleDst);

        // Download button
        SDL_SetRenderDrawColor(prenderer, 255, 255, 255, 255);
        SDL_RenderFillRect(prenderer, &buttonBox);

        SDL_RenderTexture(prenderer, pbuttonTexture, NULL, &buttonTextDst);

        //If there's text then shows it
        if (inputSize > 0) {
            SDL_Surface *pinputSurface = TTF_RenderText_Blended(font, input, inputSize, textColor);
            SDL_Texture *pinputTexture = SDL_CreateTextureFromSurface(prenderer, pinputSurface);

            //Size and position of the texte inside the textinput
            SDL_FRect dst = {inputBox.x + 5, inputBox.y + 5, pinputSurface->w, pinputSurface->h};
            SDL_RenderTexture(prenderer, pinputTexture, NULL, &dst);

            SDL_DestroySurface(pinputSurface);
            SDL_DestroyTexture(pinputTexture);
        }

        SDL_RenderPresent(prenderer);
    }

    // Clean everything
    TTF_Quit();
    SDL_StopTextInput(pwindow);
    SDL_DestroyRenderer(prenderer);
    SDL_DestroyWindow(pwindow);
    SDL_Quit();

    return 0;
}

int video_downloader(char *input) {
    char command[512];
    sprintf(command, "yt-dlp -f best -o video.mp4 \"%s\"", input);
    printf("Downloading...");
    int status = system(command);
    if (status == 0) {
        printf("Download completed successfully\n");
    }
    else {
        printf("Download failed\n");
    }
    return status;
}