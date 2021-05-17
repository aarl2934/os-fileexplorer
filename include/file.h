#ifndef __FILE_H_
#define __FILE_H_
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#define FILE_HEIGHT 30

enum FileType : uint8_t {dir, exe, img, vid, code, other};

typedef struct FileData{
    mode_t permissions;
    std::string name;
    std::string full_path;
    bool isExecuteble;
    bool isDirectory;
    int size;
}FileData;

class File{
private:
    
    TTF_Font *font;
    SDL_Texture *phrase;
    SDL_Texture *size_phrase;
    SDL_Rect phrase_rect;
    SDL_Texture *perm_phrase;
    SDL_Rect perm_rect;
    SDL_Rect size_rect;
    SDL_Surface *image_surface;
    FileType filetype;
    std::string name;
    std::string full_path;
    mode_t permissions;
protected:
    int y;
    int size;
    SDL_Texture *icon;
    SDL_Rect icon_rect;
public:
    File(FileData* data, int y_pos);
    ~File();
    void initialize(SDL_Renderer *renderer); //setup in here
    void render(SDL_Renderer *renderer);

    void scrollUp();
    void scrollDown();
    //getters and setters
    SDL_Texture* getIcon();
    SDL_Texture* getPhrase(); 
    SDL_Rect* getIconRect();
    SDL_Rect* getPhraseRect();
    TTF_Font* getFont();
    void setIcon(SDL_Renderer *renderer);
    void setImageSurf(SDL_Surface* img_surf);
    void setFont(TTF_Font* f);
    int getY();
    std::string getPath();
    void setFileType(FileType ft);
    FileType getFileType();
    std::string getSizeString();
    std::string getPermissionString();


};

//Subclasses for the file types
class Executable: public File{
    public:
        Executable(FileData* data, int y_pos);
        ~Executable();
};


class Directory: public File{
    public:
        Directory(FileData* data, int y_pos);
};

class Video: public File{
    public:
        Video(FileData* data, int y_pos);
};


class Image: public File{
    public:
        Image(FileData* data, int y_pos);
};
class Code: public File{
    public:
        Code(FileData* data, int y_pos);
};


#endif //__FILE_H_
