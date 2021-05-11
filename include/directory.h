#ifndef __DIRECTORY_H_
#define __DIRECTORY_H_

class Directory: public File{
    Directory(std::string name, SDL_Renderer *renderer, int y_pos):File(name, renderer, y_pos){
        SDL_Surface *img_surf = IMG_Load("resrc/directory.png");
        setIcon(img_surf, renderer, y);
    }
}
#endif //__DIRECTORY_H_