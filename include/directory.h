#ifndef __DIRECTORY_H_
#define __DIRECTORY_H_

class Directory: public File{
    Executable(std::string name, SDL_Renderer renderer):File(name, renderer){
        SDL_Surface *img_surf = IMG_Load("resrc/directory.png");
        setIcon(img_surf, renderer, y);
    }
}
#endif