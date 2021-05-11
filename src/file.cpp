#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <file.h>
#include <string>

File::File(std::string name, SDL_Renderer *renderer, int y_pos){
    font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 18);
    SDL_Color color = {0, 0 , 0};
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(font, name.c_str(), color);
    phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    phrase_rect->x = 0;
    phrase_rect->y = y_pos;
    SDL_QueryTexture(phrase, NULL, NULL, &(phrase_rect->w), &(phrase_rect->h));
    y = y_pos;

}

File::setIcon(SDL_Surface *img_surf, SDL_Renderer *renderer, int y_pos){
    icon = SDL_CreateTextureFromSurface(renderer, img_surf);
    SDL_FreeSurface(img_surf);
    icon_rect->x = 200;
    icon_rect->y = y_pos;
    icon_rect->w = 25;
    icon_rect->h = 25;

}