#ifndef __FILE_H_
#define __FILE_H_
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#define FILE_HEIGHT 30

enum FileType : uint8_t {dir, exe, img, vid, code, other};

class File{
private:
    SDL_Texture *icon;
    TTF_Font *font;
    SDL_Texture *phrase;
    SDL_Rect icon_rect;
    SDL_Rect phrase_rect;
    int y;
    FileType filetype;
    std::string name;
    std::string full_path;
public:
    File(std::string n, int y_pos, std::string full_path);
    void setIcon(SDL_Surface *img_surf, SDL_Renderer *renderer);
    int getY();
    int getX();
    void setFileType(FileType ft);
    void initialize(SDL_Renderer *renderer);
    
    //getters and setters
    SDL_Texture* getIcon();
    SDL_Texture* getPhrase(); 
    SDL_Rect* getIconRect();
    SDL_Rect* getPhraseRect();
    TTF_Font* getFont();

};

//Subclasses for the file types
class Executable: public File{
    public:
        Executable(std::string name, int y_pos, std::string full_path);
};


class Directory: public File{
    public:
        Directory(std::string name, int y_pos, std::string full_path);
};

class Video: public File{
    public:
        Video(std::string name, int y_pos, std::string full_path);
};


class Image: public File{
    public:
        Image(std::string name,  int y_pos, std::string full_path);
};
class Code: public File{
    public:
        Code(std::string name, int y_pos, std::string full_path);
};


#endif //__FILE_H_
