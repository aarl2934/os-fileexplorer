#ifndef __FILE_H_
#define __FILE_H_
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

enum FileType : uint8_t {dir, exe, img, vid, code, other};

class File{
private:
    SDL_Texture *icon;
    TTF_Font *font;
    SDL_Texture *phrase;
    SDL_Rect *icon_rect;
    SDL_Rect *phrase_rect;
    int y;
    FileType filetype;
public:
    File(std::string name, SDL_Renderer *renderer, int y_pos);
    void setIcon(SDL_Surface *img_surf, SDL_Renderer *renderer, int y_pos);
    int getY();
    void setFileType(FileType ft);

};

//Subclasses for the file types
class Executable: public File{
    Executable(std::string name, SDL_Renderer *renderer, int y_pos);
};


class Directory: public File{
    Directory(std::string name, SDL_Renderer *renderer, int y_pos);
};

#endif //__FILE_H_
