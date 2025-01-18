#include "utils.h"

int main() {
    huffmanTree* tree = NULL;
    errorId_t status;

    status = createHuffmanTree(tree, "/home/fawzi/Desktop/work-space/projects/c++/data-comporession/src/test.txt");
    if (status == SUCCESS) {
        puts("success");
    } else if (status == FILE_OPENING_ERROR) {
        puts("file opening error");
    }
}