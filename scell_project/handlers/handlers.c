#include "./handlers.h"

SH_STATE arguments(int a) {
	printf("_arguments\n");
}
SH_STATE init(int first, int second) {
	printf("add\n");
}
SH_STATE sub(int first, int second) {
	printf("sub\n");
}
SH_STATE quit() {
	printf("quit\n");
}