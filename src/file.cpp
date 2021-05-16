#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <file.h>
#include <string>




File::File(std::string n, int y_pos, std::string fp){
    y = y_pos * FILE_HEIGHT;
    name = n;
    full_path = fp;
    filetype = FileType::other;
}

File::~File(){
        SDL_DestroyTexture(icon);    
        SDL_DestroyTexture(phrase); 
}

void File::initialize(SDL_Renderer *renderer){
    
    SDL_Color color = {0, 0 , 0};
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(font, name.c_str(), color);
    phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    phrase_rect.x = 30;
    phrase_rect.y = y;
    SDL_QueryTexture(phrase, NULL, NULL, &(phrase_rect.w), &(phrase_rect.h));
    setIcon(renderer);
}
void File::render(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer, icon, NULL, &icon_rect);
    SDL_RenderCopy(renderer, phrase, NULL, &phrase_rect);
}
void File::setImageSurf(SDL_Surface* img_surf){
    image_surface = img_surf;
}
void File::setFont(TTF_Font* f){
    font = f;
}
void File::setIcon(SDL_Renderer *renderer){
    icon = SDL_CreateTextureFromSurface(renderer, image_surface);
    icon_rect.x = 0;
    icon_rect.y = y;
    icon_rect.w = 25;
    icon_rect.h = 25;

}

int File::getY(){
    return y;
}

void File::scrollUp(){
    y -= FILE_HEIGHT;
    icon_rect.y = y;
    phrase_rect.y = y;
}

void File::scrollDown(){
    y += FILE_HEIGHT;
    icon_rect.y = y;
    phrase_rect.y = y;
}

std::string File::getPath(){
    return full_path;
}

FileType File::getFileType(){
    return filetype;
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

Executable::Executable(std::string name, int y_pos, std::string full_path):File(name, y_pos, full_path){
        setFileType(FileType::exe);
       
        
}
// void Executable::setIcon(SDL_Surface *img_surf, SDL_Renderer *renderer){
//     if(image_surf == NULL){
//         image_surf = img_surf;
//     }
//     SDL_FreeSurface(image_surf);
//     icon = SDL_CreateTextureFromSurface(renderer, image_surf);
    
    
//     icon_rect.x = 0;
//     icon_rect.y = y;
//     icon_rect.w = 25;
//     icon_rect.h = 25;

// }
// Executable::~Executable(){
//     SDL_FreeSurface(image_surf);
// }

Directory::Directory(std::string name, int y_pos, std::string full_path):File(name, y_pos, full_path){
        setFileType(FileType::dir);
}

Video::Video(std::string name,  int y_pos, std::string full_path):File(name, y_pos, full_path){
        setFileType(FileType::vid);

}

Image::Image(std::string name, int y_pos, std::string full_path):File(name, y_pos, full_path){
        setFileType(FileType::img);

}
Code::Code(std::string name,  int y_pos, std::string full_path):File(name, y_pos, full_path){
        setFileType(FileType::code);
}
