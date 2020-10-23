#include <stdio.h>
#include <string.h>
#if defined(TARGET_OD) || defined(TARGET_LINUX)
#include <sys/stat.h>
#include <errno.h>
#endif
#include "fsutils.h"

FILE *fopen_home(const char *filename, const char *mode)
{
#if defined(TARGET_OD) || defined(TARGET_LINUX)
    static char fnamepath[2048];
    const char *path = getenv("HOME");
    struct stat info;

    snprintf(fnamepath, sizeof(fnamepath), "%s/.sm64-port/", path, filename);
    if (stat(fnamepath, &info) != 0) {
        fprintf(stderr, "Creating '%s' for the first time...\n", fnamepath);
        mkdir(fnamepath, 0700);
        if (stat(fnamepath, &info) != 0) {
            fprintf(stderr, "Unable to create '%s': %s\n", fnamepath, strerror(errno));
            abort();
        }
    }

    snprintf(fnamepath, sizeof(fnamepath), "%s/.sm64-port/%s", path, filename);
    
    return fopen(fnamepath, mode);
#else
    return fopen(filename, mode);
#endif
}