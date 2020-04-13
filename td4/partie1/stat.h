#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#define SIZE_BUF 1024
#define PTR_NULL 2

// Print type file
// \param path    Name/path of the file
// \return        -1 on open failed, 0 on success
int print_type_file(const char *path, const int mode);

// Print permission file
// \param path    Name/path of the file
// \return        -1 on open failed, 0 on success
int print_perm_file(const char *path, const int mode);

// Print ID owner/ ownership file
// \param path    Name/path of the file
// \return        -1 on open failed, 0 on success
int print_ID_owner(const char *path, const int mode);

// Print total size, in bytes, of file
// \param path    Name/path of the file
// \return        -1 on open failed, 0 on success
int print_size_file(const char *path, const int mode);

// Print read of a directory
// \param path    Name/path of the file
// \return        -1 on open failed, 0 on success
int read_dir(const char *path);
