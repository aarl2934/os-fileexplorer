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

typedef struct FileData{
    std::string name;
    std::string full_path;
    bool isExecuteble;
    bool isDirectory;

}FileData;

void initialize(SDL_Renderer *renderer, std::vector<File*> files);
void render(SDL_Renderer *renderer, std::vector<File*> files);
void quit(std::vector<File*> files);
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
    printf("Creating files\n");
    std::vector<FileData*> file_names = getFileData(current_directory);
    printf("Got file names\n");
    std::vector<File*> files = createFiles(file_names, renderer);
    printf("Created files\n");
    initialize(renderer, files);
    render(renderer, files);
    SDL_Event event;
    SDL_WaitEvent(&event);
    
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
                if(data.phrase_selected){
                    data.phrase_rect.x = event.motion.x - data.offset.x;
                    data.phrase_rect.y = event.motion.y - data.offset.y;
                }else if(data.penguin_selected){
                    data.penguin_rect.x = event.motion.x - data.offset.x; 
                    data.penguin_rect.y = event.motion.y - data.offset.y;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                //printf("mouse down %d\n", event.button.button);
                if(event.button.button == SDL_BUTTON_LEFT &&
                    event.button.x >= data.phrase_rect.x &&
                    event.button.x <= data.phrase_rect.x + data.phrase_rect.w &&
                    event.button.y >= data.phrase_rect.y &&
                    event.button.y <= data.phrase_rect.y + data.phrase_rect.h ){
                        data.phrase_selected = true;
                        data.offset.x = event.button.x - data.phrase_rect.x;
                        data.offset.y = event.button.y - data.phrase_rect.y;

                    }else if(event.button.button == SDL_BUTTON_LEFT &&
                    event.button.x >= data.penguin_rect.x &&
                    event.button.x <= data.penguin_rect.x + data.penguin_rect.w &&
                    event.button.y >= data.penguin_rect.y &&
                    event.button.y <= data.penguin_rect.y + data.penguin_rect.h ){
                        
                        data.penguin_selected = true;
                        data.offset.x = event.button.x - data.penguin_rect.x;
                        data.offset.y = event.button.y - data.penguin_rect.y;
                    }

                break;
            case SDL_MOUSEBUTTONUP:
                //printf("mouse up %d\n", event.button.button);
                data.penguin_selected = false;
                data.phrase_selected = false;

                break;
        }
        render(renderer, files);

    }

    // clean up
    quit(files);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void initialize(SDL_Renderer *renderer, std::vector<File*> files)
{
    for(File* file : files){
        file->initialize(renderer);
    }
}

void render(SDL_Renderer *renderer, std::vector<File*> files)
{
    // erase renderer content
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderClear(renderer);
    
    // TODO: draw!
    

    for(File* file : files){
        SDL_RenderCopy(renderer, file->getIcon(), NULL, file->getIconRect());
        SDL_RenderCopy(renderer, file->getPhrase(), NULL, file->getPhraseRect());
        break;
    }
    // show rendered frame
    SDL_RenderPresent(renderer);
}

void quit(std::vector<File*> files){
    for(File* file : files){
        SDL_DestroyTexture(file->getIcon());    
        SDL_DestroyTexture(file->getPhrase());    
        TTF_CloseFont(file->getFont());
    }
    
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

        for(int i = 0; i < list.size(); i++){
            FileData filedata;
            printf("%s \n", list[i].c_str());
            std::string full_path = dirname + "/" + list[i];
            filedata.full_path = full_path;
            filedata.name = list[i];
            err = stat(full_path.c_str(), &fileinfo);
            if(err){

            }else if(S_ISDIR(fileinfo.st_mode)){
                filedata.isDirectory = true;   
            }else if(S_IEXEC){
                filedata.isExecuteble = true;
            }
            file_list.push_back(&filedata);
        }
    }
    return file_list;
}
std::vector<File*> createFiles(std::vector<FileData*> file_data, SDL_Renderer *renderer){
    std::vector<File*> files;
    int y_pos = 0;
    for(FileData* data : file_data){
        printf("Creating file '%s'\n", data->name.c_str());
        printf("Full Path '%s' ", data->full_path.c_str());
        FileType type = getFileType(data);
        File* currfile;
        switch(type){
            case FileType::dir:
                currfile = new Directory(data->name, renderer, y_pos);
                break;
            case FileType::exe:
                currfile = new Executable(data->name, renderer, y_pos);
                break;
            case FileType::code:
                currfile = new Code(data->name, renderer, y_pos);
                break;
            case FileType::img:
                currfile = new Image(data->name, renderer, y_pos);
                break;
            case FileType::vid:
                currfile = new Video(data->name, renderer, y_pos);
                break;
            default:
                currfile = new File(data->name, y_pos);
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
    }else if(file->name.find(".jpg") ||
             file->name.find(".jpeg")||
             file->name.find(".png") ||
             file->name.find(".tif") ||
             file->name.find(".tiff"||
             file->name.find(".gif"))){
        return FileType::img;
    }else if(file->name.find(".mp4") ||
             file->name.find(".mov")||
             file->name.find(".mkv") ||
             file->name.find(".avi") ||
             file->name.find(".webm")){
        return FileType::vid;        
    }else if(file->name.find(".h") ||
             file->name.find(".cpp")||
             file->name.find(".c") ||
             file->name.find(".py") ||
             file->name.find(".java")||
             file->name.find(".js")){
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

