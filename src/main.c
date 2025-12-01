#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bank.h"
#include "file_ops.h"

int main(void) {
    /* seed random (if you add salt or randomness later) */
    srand((unsigned)time(NULL));

    load_db();
    printf("Loaded %d user(s) from '%s'.\n", user_count, FILE_NAME);
    main_menu();
    return 0;
}
