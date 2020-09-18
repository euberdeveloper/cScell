#include "./handlers.h"

SH_STATE arguments(a) {
printf("_arguments");
	}
SH_STATE init(first, second) {
printf("add");
	}
SH_STATE sub(first, second) {
printf("sub");
	}
SH_STATE quit() {
printf("quit");
	}