#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <file.h>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h> 
#define WIDTH 800
#define HEIGHT 600

typedef struct AppData{
    SDL_Texture *penguin;
    TTF_Font *font;
    SDL_Texture *phrase;
    SDL_Rect penguin_rect;
    SDL_Rect phrase_rect;
    bool penguin_selected;
    bool phrase_selected;
    SDL_Point offset;
}AppData;


void initializeFiles(SDL_Renderer *renderer, std::vector<File*> files, SDL_Surface* dir_surf, SDL_Surface* exe_surf, SDL_Surface* code_surf, SDL_Surface* img_surf, SDL_Surface* vid_surf, SDL_Surface* other_surf, TTF_Font* font);
void render(SDL_Renderer *renderer, std::vector<File*> files);
void initialize();
void close(std::vector<File*> files);
std::string launch(File* file, std::string dirname);
std::vector<FileData*> getFileData(std::string current_directory);
std::vector<File*> createFiles(std::vector<FileData*> file_names, SDL_Renderer *renderer);
FileType getFileType(FileData* file);

int main(int argc, char **argv)
{
    char *home = getenv("HOME");
    printf("HOME: %s\n", home);

    // initializing SDL as Video
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    // create window and renderer
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    // initialize and perform rendering loop
    AppData data;
    std::string current_directory = home;
    std::string prevdir = current_directory;
    std::vector<FileData*> file_names = getFileData(current_directory);

    std::vector<File*> files = createFiles(file_names, renderer);
    SDL_Surface *dir_surf, *exe_surf, *code_surf, *img_surf, *vid_surf, *other_surf;
    TTF_Font* font;
    font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 18);
    dir_surf = IMG_Load("resrc/directory.png");
    exe_surf = IMG_Load("resrc/executable.png");
    code_surf = IMG_Load("resrc/code.png");
    vid_surf = IMG_Load("resrc/video.png");
    img_surf = IMG_Load("resrc/image.png");
    other_surf = IMG_Load("resrc/other.png");
    initializeFiles(renderer, files, dir_surf, exe_surf, code_surf, img_surf, vid_surf, other_surf, font);
    render(renderer, files);
    SDL_Event event;
    SDL_WaitEvent(&event);
    //XInitThreads();
    while (event.type != SDL_QUIT)
    {
        //list all the files in the current directory
        
        // .. starts at y = 0
        // render every file until y + height of file = HEIGHT
        // on scroll event, change every y to -/+ height of a file
        // on click event rerender in new folder or launch program
        //
        
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_MOUSEMOTION:
                //printf("mouse move %d %d\n", event.motion.x, event.motion.y);
                // if(data.phrase_selected){
                //     data.phrase_rect.x = event.motion.x - data.offset.x;
                //     data.phrase_rect.y = event.motion.y - data.offset.y;
                // }else if(data.penguin_selected){
                //     data.penguin_rect.x = event.motion.x - data.offset.x; 
                //     data.penguin_rect.y = event.motion.y - data.offset.y;
                // }
                break;
            case SDL_MOUSEBUTTONDOWN:
                //printf("mouse down %d\n", event.button.button);
            
                    if(event.button.button == SDL_BUTTON_LEFT){
                        for(File* file : files){
                            SDL_Rect ph_rect = *file->getPhraseRect();
                            if(
                            event.button.y >= ph_rect.y &&
                            event.button.y <= ph_rect.y + ph_rect.h){
                                
                                current_directory = launch(file, current_directory);
                            }
                        }
                    }
                
                

                break;
            case SDL_MOUSEBUTTONUP:

                break;
            case SDL_MOUSEWHEEL:
                if(event.wheel.y > 0 ){
                    for(File* file : files){
                        file->scrollUp();
                    }
                }else if(event.wheel.y < 0){
                    for(File* file : files){
                        file->scrollDown();
                    }
                }
        }
        render(renderer, files);
        if(prevdir != current_directory){ // directory changed, quit and resetup
            close(files);
            file_names = getFileData(current_directory);
            files = createFiles(file_names, renderer);
            initializeFiles(renderer, files, dir_surf, exe_surf, code_surf, img_surf, vid_surf, other_surf, font);
            render(renderer, files);
        }
        prevdir = current_directory;
    }

    // clean up
    close(files);
    TTF_CloseFont(font);
    SDL_FreeSurface(dir_surf);
    SDL_FreeSurface(exe_surf);
    SDL_FreeSurface(img_surf);
    SDL_FreeSurface(code_surf);
    SDL_FreeSurface(vid_surf);
    SDL_FreeSurface(other_surf);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void initializeFiles(SDL_Renderer *renderer, std::vector<File*> files,  SDL_Surface* dir_surf, SDL_Surface* exe_surf, SDL_Surface* code_surf, SDL_Surface* img_surf, SDL_Surface* vid_surf, SDL_Surface* other_surf, TTF_Font* font)
{
    for(File* file : files){
        FileType filetype = file->getFileType();
        file->setFont(font);
        switch(filetype){
        case FileType::dir:
            file->setImageSurf(dir_surf);
            break;
        case FileType::exe:
            file->setImageSurf(exe_surf);
            break;
        case FileType::code:
            file->setImageSurf(code_surf);
            break;
        case FileType::img:
            file->setImageSurf(img_surf);
            break;
        case FileType::vid:
            file->setImageSurf(vid_surf);
            break;
        default:
            file->setImageSurf(other_surf);
        }
    }
    for(File* file : files){
        file->initialize(renderer);
    }
}

void close(std::vector<File*> files){
    int i = files.size();
    while(files.size() > 0){
        i--;
        delete files[i];
        files.pop_back();
    }
}


void render(SDL_Renderer *renderer, std::vector<File*> files)
{
    // erase renderer content
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderClear(renderer);
    
    // TODO: draw!
    

    for(File* file : files){
        if(file->getY() > HEIGHT || file->getY() < 0){
            continue;
        }
        file->render(renderer);
        
    }
    // show rendered frame
    SDL_RenderPresent(renderer);
}


std::string launch(File* file, std::string dirname){
    FileType filetype = file->getFileType();
    int pid;
    switch (filetype)
    {
    case  exe:/* constant-expression */
        pid = fork();
        if(pid == 0){
            char* filename = new char[strlen(file->getPath().c_str())];
            strcpy(filename, file->getPath().c_str());
            char * command_list[2] = {filename, 0};
            execv(filename, command_list);
            exit(0);
        }
        break;
    case dir:
        dirname = file->getPath();
        break;
    default:
        pid = fork();
        if(pid == 0){
            char* filename = new char[strlen(file->getPath().c_str())];
            strcpy(filename, file->getPath().c_str());
            printf("%s\n", filename);
            const char* xdg = "/usr/bin/xdg-open";
            char * command_list[3] = {(char *const)xdg, (char *const)filename, 0};//{(char *const)xdg, (char *const)filename};
            execv(xdg, command_list);
            exit(0);
        }else if (pid == -1){
            fprintf(stderr, "Not able to fork");
        }else{
            int status;
            waitpid(pid, &status, 0);
        }
        break;
    }
    return dirname;
}

std::vector<FileData*> getFileData(std::string dirname){
    struct stat info;
    struct stat fileinfo;
    std::vector<std::string> list;
    std::vector<FileData*> file_list;
    int err = stat(dirname.c_str(), &info);
    if (err == 0 && S_ISDIR(info.st_mode))
    {
        DIR* dir = opendir(dirname.c_str());
        // TODO: modify to be able to print all entries in directory in alphabetical order
        //       in addition to file name, also print file size (or 'directory' if entry is a folder)
        //       Example output:
        //         ls.cpp (693 bytes
        //         my_file.txt (62 bytes)
        //         OS Files (directory)
        struct dirent *entry;
        
        while ((entry = readdir(dir)) != NULL) {
           
            list.push_back(entry->d_name);
        }
        std::sort(list.begin(), list.end());
        for(int i = 0; i < list.size(); i++){
            if(list[i] == "."){
                continue;
            }
            FileData* filedata = new FileData;
            std::string full_path = dirname + "/" + list[i];
            filedata->full_path = full_path;
            filedata->name = list[i];
            err = stat(full_path.c_str(), &fileinfo);
            filedata->isDirectory = false;
            filedata->isExecuteble = false;
            if(S_ISDIR(fileinfo.st_mode)){
                filedata->isDirectory = true;   
            }else if((fileinfo.st_mode & S_IEXEC) != 0){
                filedata->isExecuteble = true;
            }
            filedata->permissions = fileinfo.st_mode;
            filedata->size = (int)fileinfo.st_size;
            file_list.push_back(filedata);
        }
    }
    return file_list;
}
std::vector<File*> createFiles(std::vector<FileData*> file_data, SDL_Renderer *renderer){
    std::vector<File*> files;
    int y_pos = 0;
    for(FileData* data : file_data){
        FileType type = getFileType(data);
        File* currfile;
        switch(type){
            case FileType::dir:
                currfile = new Directory(data, y_pos);
                break;
            case FileType::exe:
                currfile = new Executable(data,  y_pos);
                break;
            case FileType::code:
                currfile = new Code(data,  y_pos);
                break;
            case FileType::img:
                currfile = new Image(data,  y_pos);
                break;
            case FileType::vid:
                currfile = new Video(data, y_pos);
                break;
            default:
                currfile = new File(data, y_pos);
        }

        y_pos++;
        files.push_back(currfile);
    }
    return files;
}

FileType getFileType(FileData* file){
    if(file->name == "." || file->name == ".." || file->isDirectory){
        return FileType::dir;
    }else if(file->isExecuteble){
        return FileType::exe;
    }else if(file->name.find(".jpg") != std::string::npos ||
             file->name.find(".jpeg")!= std::string::npos||
             file->name.find(".png") != std::string::npos||
             file->name.find(".tif") != std::string::npos||
             file->name.find(".tiff") != std::string::npos||
             file->name.find(".gif") != std::string::npos){
        return FileType::img;
    }else if(file->name.find(".mp4") != std::string::npos||
             file->name.find(".mov") != std::string::npos||
             file->name.find(".mkv") != std::string::npos||
             file->name.find(".avi") != std::string::npos||
             file->name.find(".webm")!= std::string::npos){
        return FileType::vid;        
    }else if(file->name.find(".h") != std::string::npos||
             file->name.find(".cpp")!= std::string::npos||
             file->name.find(".c") != std::string::npos||
             file->name.find(".py") != std::string::npos||
             file->name.find(".java")!= std::string::npos||
             file->name.find(".js") != std::string::npos){
        return FileType::code;
    }else{
        return FileType::other;
    }

}
void listDirectory(std::string dirname, int indent)
{
    struct stat info;
    struct stat fileinfo;
    std::vector<std::string> list;
    std::string space = "";
    for(int i = 0; i < indent; i++){
        space.append("\t");
    }
    int err = stat(dirname.c_str(), &info);
    if (err == 0 && S_ISDIR(info.st_mode))
    {
        DIR* dir = opendir(dirname.c_str());
        // TODO: modify to be able to print all entries in directory in alphabetical order
        //       in addition to file name, also print file size (or 'directory' if entry is a folder)
        //       Example output:
        //         ls.cpp (693 bytes
        //         my_file.txt (62 bytes)
        //         OS Files (directory)
        struct dirent *entry;
        
        while ((entry = readdir(dir)) != NULL) {
            list.push_back(entry->d_name);
        }

        std::sort(list.begin(), list.end());
        for(int i = 0; i < list.size(); i++){
            std::string full_path = dirname + "/" + list[i];
            err = stat(full_path.c_str(), &fileinfo);
            if(err){

            }else if(S_ISDIR(fileinfo.st_mode)){
                printf("%s%s (directory)\n", space.c_str(), list[i].c_str());
                if(list[i] != "." && list[i] != ".."){
                    listDirectory(full_path, indent+1);
                }
                
            }else{
                printf("%s%s (%ld bytes)\n", space.c_str(), list[i].c_str(), fileinfo.st_size);
            }
        }

        closedir(dir);
    }
    else
    {
        fprintf(stderr, "Error: directory '%s' not found\n", dirname.c_str());
    }
}

