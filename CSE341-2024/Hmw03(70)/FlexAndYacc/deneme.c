#include <stdio.h>

typedef struct Node Node;

typedef struct {
    union {
        int i;
        float f;
        char* str;
        int bool;
        Node* list;
    } value;
    enum { INT, FLOAT, STRING, BOOL, LIST } type;
} Value;

typedef struct Node {
    Value data;
    struct Node* next;
} Node;

int main() {
    Value data;
    data.type = INT;
    return 0;
}