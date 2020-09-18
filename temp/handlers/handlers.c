#include "./handlers.h"

SH_STATE arguments(int a) {
    printf("arguments %d\n", a);
}

SH_STATE init(int first, int second) {
    printf("init %d %d\n", first, second);
}

SH_STATE sub(int first, int second) {
    printf("sub %d %d\n", first, second);
}

SH_STATE quit() {
    printf("quit\n");
}