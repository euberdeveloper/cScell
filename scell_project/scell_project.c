#include "./scellutils/shell_utils/shell_utils.h"
#include "./scellutils/shell_commands/shell_commands.h"

int main(int argc, char** argv) {
    /* Handle command line arguments */
	sh_handle__arguments(argv, argc);

    /* Loop that continuosly listens for shell commands */
    sh_loop();

    return 0;
}