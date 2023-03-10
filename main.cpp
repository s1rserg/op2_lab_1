#include "file_pointer.h"
#include "file_stream.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Incorrect command line parameters" << endl;
        return 1;
    }
    if(!strcmp(argv[2], "FilePointer")){
        fp::menu();
    }else if(!strcmp(argv[2], "FileStream")){
        fs::menu();
    }else{
        cout << "Incorrect command line parameters" << endl;
        return 1;
    }
    return 0;
}
