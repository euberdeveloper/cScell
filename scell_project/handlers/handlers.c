#include "./handlers.h"

SH_STATE arguments(int a) {
	printf("_arguments");
}
SH_STATE init(int first, int second) {
	printf("add");
}
SH_STATE sub(int first, int second) {
	printf("sub");
}
SH_STATE quit() {
	printf("quit");
}