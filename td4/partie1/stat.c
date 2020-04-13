#include "stat.h"

int print_type_file(const char *path, const int mode) {
  // test if path is null
  if(!path) {
    printf("Error in 'print_type_file()': Path null.\n");
    exit(PTR_NULL);
  }

  // Creat the struct stat
  struct stat stat_view;
  int success_stat=stat(path, &stat_view);

  // test stat open
  if(!success_stat) {
    // File exist and haven't errors with open file.
    char type_file[SIZE_BUF];

    // Use a mask a compare with value of special type
    // Use S_IFMT for see all file type
    switch (stat_view.st_mode & S_IFMT) {
      case S_IFSOCK: strcpy(type_file, "Sock       ");     break;
      case S_IFBLK:  strcpy(type_file, "Device     ");     break;
      case S_IFREG:  strcpy(type_file, "Common_file");     break;
      case S_IFDIR:  strcpy(type_file, "Directory  ");     break;
      case S_IFIFO:  strcpy(type_file, "FIFO       ");     break;
      default:       strcpy(type_file, "Unknown    ");     break;
    }

    // test if file is a symlink
    lstat(path, &stat_view);
    if(S_ISLNK(stat_view.st_mode))
      strcpy(type_file, "Symlink");

    // Print
    if(mode  ==  1)
      printf("Type of '%s' is: %s.\n", path, type_file);
    if(mode  ==  2)
      printf("%s  ", type_file);
  }
  else {
    //  File doesn't exist or have errors with open file.
    printf("File doesn't exist or have errors with open file.\n");
  }

  return success_stat;
}

int print_perm_file(const char *path, const int mode) {
  // test if path is null
  if(!path) {
    printf("Error in 'print_type_file()': Path null.\n");
    exit(PTR_NULL);
  }

  // Creat the struct stat
  struct stat stat_view;
  int success_stat = stat(path, &stat_view);

  // test stat
  if(!success_stat) {
    // File exist and haven't errors with open file.

    // See man 7 inode for detail
    if(mode  ==  1)
      printf("permission of '%s': ", path);

    // Test for user permission
    (stat_view.st_mode & S_IRUSR)? printf("r"): printf("-");
    (stat_view.st_mode & S_IWUSR)? printf("w"): printf("-");
    (stat_view.st_mode & S_IXUSR)? printf("x"): printf("-");

    // Test for group permission
    (stat_view.st_mode & S_IRGRP)? printf("r"): printf("-");
    (stat_view.st_mode & S_IWGRP)? printf("w"): printf("-");
    (stat_view.st_mode & S_IXGRP)? printf("x"): printf("-");

    // Test for others permission
    (stat_view.st_mode & S_IROTH)? printf("r"): printf("-");
    (stat_view.st_mode & S_IWOTH)? printf("w"): printf("-");
    (stat_view.st_mode & S_IXOTH)? printf("x"): printf("-");

    if(mode  ==  1)
      printf(" end.\n");
    if(mode  ==  2)
      printf("  ");
  }
  else {
    //  File doesn't exist or have errors with open file.
    printf("File doesn't exist or have errors with open file.\n");
  }

  return success_stat;
}

int print_ID_owner(const char *path, const int mode) {
  // test if path is null
  if(!path) {
    printf("Error in 'print_type_file()': Path null.\n");
    exit(PTR_NULL);
  }

  // Creat the struct stat
  struct stat stat_view;
  int success_stat = stat(path, &stat_view);

  // test stat
  if(!success_stat) {
    // File exist and haven't errors with open file.

    // Print
    if(mode  ==  1)
      printf("Ownership: UID=%ld, GID=%ld\n", (long) stat_view.st_uid, (long) stat_view.st_gid);
    if(mode  ==  2)
      printf("%d  ", stat_view.st_uid);
  }
  else {
    //  File doesn't exist or have errors with open file.
    printf("File doesn't exist or have errors with open file.\n");
  }

  return success_stat;
}

int print_size_file(const char *path, const int mode) {
  // test if path is null
  if(!path) {
    printf("Error in 'print_type_file()': Path null.\n");
    exit(PTR_NULL);
  }

  // Creat the struct stat
  struct stat stat_view;
  int success_stat = stat(path, &stat_view);

  // Test stat
  if(!success_stat) {
    // File exist and haven't errors with open file.

    // Print
    if(mode  ==  1)
      printf("File size: %lld bytes\n", (long long) stat_view.st_size);
    if(mode  ==  2)
      printf("%6ld  ", stat_view.st_size);
  }
  else {
    //  File doesn't exist or have errors with open file.
    printf("File doesn't exist or have errors with open file.\n");
  }

  return success_stat;
}

int read_dir(const char *path) {
  // Init directory
  DIR *dir=NULL;
  dir=opendir(path);

  // Test open directory
  if(!dir) {
    printf("Error in 'read_dir()': Can't open the directory");
    return -1;
  }

  // Print
  printf("ls: %s\n\n", path);

  // Read in the directory
  struct dirent *_dirent=NULL;
  while((_dirent=readdir(dir))) {
    if((strcmp(_dirent->d_name, ".") != 0) && (strcmp(_dirent->d_name, "..") != 0)) {

      // Print information of the file
      print_type_file(_dirent->d_name, 2);
      print_perm_file(_dirent->d_name, 2);
      print_ID_owner(_dirent->d_name, 2);
      print_size_file(_dirent->d_name, 2);
      printf("%s ", _dirent->d_name);
      printf("\n");
    }
  }
  printf("\n");

  // Return value of closedir
  return closedir(dir);
}
