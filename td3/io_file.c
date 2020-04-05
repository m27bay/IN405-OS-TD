#include "io_file.h"

IO_FILE IO_open(const char *path, int access){
  if(!path) {
    printf("Error in 'IO_open()': path doesn't exist.\n");
    exit(FAILED_PTR_NULL);
  }

  IO_FILE file;

  // Init file
  file.path = NULL;
  file.access = -1;
  file.desc = -1;

  // Init the path
  file.path = malloc((strlen(path)+1)*sizeof(char));
  if(!file.path) {
    printf("Error in 'IO_open()': malloc failed\n");
    return file;
  }

  // Open the file
  // Test if file exist, creat if doesn't existe
  int fd, fd2;

  struct stat buffer;
  if(stat(path, &buffer)  ==  -1) {
    fd2 = creat(path, 0644);
    fd = open(path, access);
  }
  // Else open
  else {
    fd2 = -2;
    fd = open(path, access);
  }

  if((fd  ==  -1) || (fd2  ==  -1)) {
    printf("Error: file creation failed\nfd = %d, fd2 = %d\n", fd, fd2);
    return file;
  }

  // It's all is ok
  strcpy(file.path, path);
  file.access = access;
  file.desc = fd;

  return file;
}

int IO_close(IO_FILE file) {
  free(file.path);
  return close(file.desc);
}

int IO_remove(const char *path) {
  return unlink(path);
}

int IO_char_read(IO_FILE file, char *c) {
  // Test if we have the right to read
  if(file.access  ==  O_RDONLY)
    return read(file.desc, c, 1);

  printf("Error in 'IO_char_read()': file haven't permission to read.\n");
  return -1;
}

int IO_char_write(IO_FILE file, const char c) {
  // test if we have the right to write
  if(file.access  ==  (O_CREAT | O_WRONLY))
    return write(file.desc, &c, 1);

  printf("Error in 'IO_char_write()': file haven't permission to write.\n");
  return -1;
}

int IO_string_read(IO_FILE file, char *string, int maxSize) {
  if(file.access == O_RDONLY)
    return read(file.desc, string, maxSize);

  printf("Error in 'IO_string_read()': file haven't permission to read.\n");
  return -1;
}

int IO_string_write(IO_FILE file, const char *string, int size) {
  if(file.access == (O_CREAT | O_WRONLY)) {
    int size_write = write(file.desc, string, size);
    if(size_write!=size)
      return -1;
    return 0;
  }
  printf("Error in 'IO_string_write()': file haven't permission to write.\n");
  return -1;
}

int IO_int_read(IO_FILE file, int *n) {
  if(file.access == O_RDONLY) {
    return 0;
  }

  printf("Error in 'IO_int_read()': file haven't permission to read.\n");
  return -1;
}

int IO_int_write(IO_FILE file, const int n) {
  if(file.access == (O_CREAT | O_WRONLY)) {
    char char_write[SIZE_BUF];
    snprintf(char_write, SIZE_BUF, "%d", n);
    return write(file.desc, char_write, (int)strlen(char_write));
  }

  printf("Error in 'IO_int_write()': file haven't permission to write.\n");
  return -1;
}

void view_file(IO_FILE file) {
  // printf("All access:\nO_CREAT | O_WRONLY = %d\nO_CREAT = %d\nO_WRONLY = %d\nO_RDONLY = %d\n\n", O_CREAT | O_WRONLY, O_CREAT, O_WRONLY, O_RDONLY);
  printf("File information:\nFile discriptor: %d, File access: %d\nPath file: %s\n", file.desc, file.access, file.path);
}
