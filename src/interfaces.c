#include "interfaces.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define DESKTOPD_DEFAULT_SCRIPTS_ENABLED_DIR \
    "~/.local/share/desktopd/scripts-enabled"

/// \brief Get desktopd script-enabled directory path.
void _desktopd_scripts_enabled_dir(char *buf, uint64_t buf_len)
{
    const char *dir = getenv("DESKTOPD_SCRIPTS_ENABLED_DIR");
    if (dir == NULL) {
        char *home = getenv("HOME");
        snprintf(buf, buf_len, "%s%s",
            home, DESKTOPD_DEFAULT_SCRIPTS_ENABLED_DIR + 1);
        return;
    }
    int len = strlen(dir);
    int max_len = (len >= buf_len) ? buf_len - 1 : len;
    strncpy(buf, dir, max_len);
    buf[max_len] = '\0';
}

void string_free(char *string)
{
    free(string);
}

int32_t desktop_property_number_of_desktops()
{
    char path[512];
    char dir[128];
    char ret[32];
    char *ptr;

    // Make path.
    _desktopd_scripts_enabled_dir(dir, sizeof(dir));
    sprintf(path, "%s/%s", dir, "NumberOfDesktops");

    // Check sciprt enabled.
    if (access(path, X_OK) != 0) {
        return -1;
    }

    FILE *f = popen(path, "r");
    if (f == NULL) {
        return -1;
    }

    while (fgets(ret, sizeof(ret), f) != NULL) {
        long n = strtol(ret, &ptr, 10);
        return n;

        pclose(f);
    }

    return -1;
}

int32_t desktop_property_current_desktop()
{
    char path[512];
    char dir[128];
    char ret[32];
    char *ptr;

    // Make path.
    _desktopd_scripts_enabled_dir(dir, sizeof(dir));
    sprintf(path, "%s/%s", dir, "CurrentDesktop");

    // Check script enabled.
    if (access(path, X_OK) != 0) {
        return -1;
    }

    FILE *f = popen(path, "r");
    if (f == NULL) {
        return -1;
    }

    while (fgets(ret, sizeof(ret), f) != NULL) {
        long n = strtol(ret, &ptr, 10);
        pclose(f);

        return n;
    }

    pclose(f);

    return -1;
}

bool desktop_method_insert_desktop(int32_t value)
{
    char path[512];
    char dir[128];

    // Make path.
    _desktopd_scripts_enabled_dir(dir, sizeof(dir));
    sprintf(path, "%s/%s %d", dir, "InsertDesktop", value);

    // Check script enabled.
    if (access(path, X_OK) != 0) {
        return false;
    }

    FILE *f = popen(path, "r");
    if (f == NULL) {
        return false;
    }

    if (pclose(f) == 0) {
        return true;
    }

    return false;
}
