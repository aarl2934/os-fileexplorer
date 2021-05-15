#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <file.h>
#include <string>




File::File(std::string name, int y_pos){
    y = y_pos * FILE_HEIGHT;
    name = name;
    filetype = FileType::other;
}
void File::initialize(SDL_Renderer *renderer){
    font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 18);
    SDL_Color color = {0, 0 , 0};
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(font, name.c_str(), color);
    phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    phrase_rect.x = 0;
    phrase_rect.y = y;
    SDL_QueryTexture(phrase, NULL, NULL, &(phrase_rect.w), &(phrase_rect.h));
    SDL_Surface *img_surf;
    switch(filetype){
        case FileType::dir:
            img_surf = IMG_Load("resrc/directory.png");
            break;
        case FileType::exe:
            img_surf = IMG_Load("resrc/executable.png");
            break;
        case FileType::code:
            img_surf = IMG_Load("resrc/code.png");
            break;
        case FileType::img:
            img_surf = IMG_Load("resrc/image.png");
            break;
        case FileType::vid:
            img_surf = IMG_Load("resrc/video.png");
            break;
        default:
            img_surf = IMG_Load("resrc/other.png");

    }
    setIcon(img_surf, renderer, getY());
}
void File::setIcon(SDL_Surface *img_surf, SDL_Renderer *renderer, int y_pos){
    icon = SDL_CreateTextureFromSurface(renderer, img_surf);
    SDL_FreeSurface(img_surf);
    icon_rect.x = 200;
    icon_rect.y = y_pos * FILE_HEIGHT;
    icon_rect.w = 25;
    icon_rect.h = 25;

}

int File::getY(){
    return y;
}

void File::setFileType(FileType ft){
    filetype = ft;
}
SDL_Texture* File::getPhrase(){
    return phrase;
}
SDL_Texture* File::getIcon(){
    return icon;
}
SDL_Rect* File::getIconRect(){
    return &icon_rect;
}
SDL_Rect* File::getPhraseRect(){
    return &phrase_rect;
}
TTF_Font* File::getFont(){
    return font;
}
//=======================
//Subclasses constructors
//=======================

Executable::Executable(std::string name, SDL_Renderer *renderer, int y_pos):File(name, y_pos){
        setFileType(FileType::exe);
        
}

Directory::Directory(std::string name, SDL_Renderer *renderer, int y_pos):File(name, y_pos){
        setFileType(FileType::dir);
}

Video::Video(std::string name, SDL_Renderer *renderer, int y_pos):File(name, y_pos){
        setFileType(FileType::vid);

}

Image::Image(std::string name, SDL_Renderer *renderer, int y_pos):File(name, y_pos){
        setFileType(FileType::img);

}
Code::Code(std::string name, SDL_Renderer *renderer, int y_pos):File(name, y_pos){
        setFileType(FileType::code);
}
