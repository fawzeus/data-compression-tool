#include "utils.h"

int main() {
    huffmanNode* tree = NULL;
    errorId_t status = SUCCESS;

    status = encode(&tree, "/home/fawzi/Desktop/work-space/projects/c++/data-comporession/src/test.txt");
    if (status == SUCCESS) {
        puts("success");
    } else if (status == FILE_OPENING_ERROR) {
        puts("file opening error");
    }
}