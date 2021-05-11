#ifndef __EXECUTABLE_H_
#define __EXECUTABLE_H_
#include <file.h>

class Executable: public File{
    Executable(std::string name, SDL_Renderer *renderer, int y_pos):File(name, renderer, y_pos){
        SDL_Surface *img_surf = IMG_Load("resrc/executable.png");
        setIcon(img_surf, renderer, y);
    }
}

#endif //__EXECUTABLE_H_