//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{

  //The window we'll be rendering to
  SDL_Window *window = NULL;

  //The surface contained by the window
  SDL_Surface *screenSurface = NULL;

  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }

  else
  {
    //Create window
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Delay(1000);
    if (window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
      //Get window surface
      screenSurface = SDL_GetWindowSurface(window);

      //Fill the surface white
      SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

      //Update the surface
      SDL_UpdateWindowSurface(window);

      //Wait two seconds
      int quit = 0;
      SDL_Event e;
      while (!quit)
      {
        while (SDL_PollEvent(&e))
        {
          if (e.type == SDL_QUIT)
          {
            quit = 1;
          }
        }
      }
    }
  }
}
