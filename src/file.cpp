#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <file.h>
#include <string>
#include <iostream>
#include <sys/stat.h>




File::File(FileData* data, int y_pos){
    y = y_pos * FILE_HEIGHT;
    name = data->name;
    full_path = data->full_path;
    size = data->size;
    permissions = data->permissions;
    filetype = FileType::other;
    icon_rect.w = 18;
    icon_rect.h = 25;
    icon_rect.x = 5;
}

File::~File(){
        SDL_DestroyTexture(icon);    
        SDL_DestroyTexture(phrase); 
        SDL_DestroyTexture(size_phrase);
        SDL_DestroyTexture(perm_phrase);
}

void File::initialize(SDL_Renderer *renderer){
    
    SDL_Color color = {0, 0 , 0};
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(font, name.c_str(), color);

    char* size_string = new char[strlen(getSizeString().c_str())];
    strcpy(size_string, getSizeString().c_str());
    SDL_Surface *size_surf = TTF_RenderText_Solid(font, size_string, color);

    char* perm_string = new char[strlen(getPermissionString().c_str())];
    strcpy(perm_string, getPermissionString().c_str());
    SDL_Surface *perm_surf = TTF_RenderText_Solid(font, perm_string, color);


    phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    size_phrase = SDL_CreateTextureFromSurface(renderer, size_surf);
    perm_phrase = SDL_CreateTextureFromSurface(renderer, perm_surf);
    SDL_FreeSurface(phrase_surf);
    SDL_FreeSurface(size_surf);
    SDL_FreeSurface(perm_surf);

    size_rect.x = 400;
    size_rect.y = y;

    phrase_rect.x = 30;
    phrase_rect.y = y;

    perm_rect.x = 600;
    perm_rect.y = y;

    SDL_QueryTexture(phrase, NULL, NULL, &(phrase_rect.w), &(phrase_rect.h));
    SDL_QueryTexture(size_phrase, NULL, NULL, &(size_rect.w), &(size_rect.h));
    SDL_QueryTexture(perm_phrase, NULL, NULL, &(perm_rect.w), &(perm_rect.h));

    setIcon(renderer);
}
void File::render(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer, icon, NULL, &icon_rect);
    SDL_RenderCopy(renderer, phrase, NULL, &phrase_rect);
    SDL_RenderCopy(renderer, size_phrase, NULL, &size_rect);
    SDL_RenderCopy(renderer, perm_phrase, NULL, &perm_rect);

}
void File::setImageSurf(SDL_Surface* img_surf){
    image_surface = img_surf;
}
void File::setFont(TTF_Font* f){
    font = f;
}
void File::setIcon(SDL_Renderer *renderer){
    icon = SDL_CreateTextureFromSurface(renderer, image_surface);
    
    icon_rect.y = y;

}

std::string File::getSizeString(){

    std::string ret;
    if (filetype == FileType::dir){
        ret = "";
    }else if(size < 1024){
        ret = std::to_string(size) + " Bytes";
    }else if(size < 1024*1024){
        ret = std::to_string(size/1024) + " KB";
    }else if (size < 1024*1024*1024){
        ret = std::to_string(size/1024/1024) + " MB";
    }else{
        ret = std::to_string(size/1024/1024/1024) + " GB";
    }
    return ret;
}

std::string File::getPermissionString(){
    std::string ret;
    ret.append(((permissions & S_IRUSR) == S_IRUSR ? "r" : "-"));
    ret.append((permissions & S_IWUSR) == S_IWUSR ? "w" : "-");
    ret.append((permissions & S_IXUSR) == S_IXUSR ? "x" : "-");
    ret.append((permissions & S_IRGRP) == S_IRGRP? "r" : "-");
    ret.append((permissions & S_IWGRP) == S_IWGRP ? "w" : "-");
    ret.append((permissions & S_IXGRP) == S_IXGRP ? "x" : "-");
    ret.append((permissions & S_IROTH) == S_IROTH ? "r" : "-");
    ret.append((permissions & S_IWOTH) == S_IWOTH ? "w" : "-");
    ret.append((permissions & S_IXOTH) == S_IXOTH ? "x" : "-");
    return ret;
}

int File::getY(){
    return y;
}

void File::scrollUp(){
    y -= FILE_HEIGHT;
    icon_rect.y = y;
    phrase_rect.y = y;
    size_rect.y = y;
    perm_rect.y = y;
}

void File::scrollDown(){
    y += FILE_HEIGHT;
    icon_rect.y = y;
    phrase_rect.y = y;
    size_rect.y = y;
    perm_rect.y = y;
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

Executable::Executable(FileData* data, int y_pos):File(data, y_pos){
        setFileType(FileType::exe);
        icon_rect.w = 25;
        icon_rect.h = 25;
        icon_rect.x = 0;

}

Directory::Directory(FileData* data, int y_pos):File(data, y_pos){
        setFileType(FileType::dir);
        icon_rect.w = 25;
        icon_rect.h = 25;
        icon_rect.x = 0;

}

Video::Video(FileData* data, int y_pos):File(data, y_pos){
        setFileType(FileType::vid);
        icon_rect.w = 25;
        icon_rect.h = 20;
        icon_rect.x = 0;

}

Image::Image(FileData* data, int y_pos):File(data, y_pos){
        setFileType(FileType::img);
        icon_rect.w = 25;
        icon_rect.h = 20;
        icon_rect.x = 0;

}
Code::Code(FileData* data, int y_pos):File(data, y_pos){
        setFileType(FileType::code);
        icon_rect.w = 25;
        icon_rect.h = 25;
        icon_rect.x = 0;

}
