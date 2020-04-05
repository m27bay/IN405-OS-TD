#include "stat.h"

void test_print_type_file() {
  // test file
  printf("\n");
  system("echo hello world >file1.txt");
  if(!print_type_file("file1.txt", 1))
    printf("'print_type_file()' test has succeeded.\n");
  else
    printf("'print_type_file()' test has failed.\n");
  system("rm file1.txt");

  // test dir
  printf("\n");
  system("mkdir dir1");
  if(!print_type_file("dir1", 1))
    printf("'print_type_file()' test has succeeded.\n");
  else
    printf("'print_type_file()' test has failed.\n");
  system("rmdir dir1");

  // test link
  printf("\n");
  system("mkdir dir1");
  system("ln -s dir1 link_dir1");
  if(!print_type_file("link_dir1", 1))
    printf("'print_type_file()' test has succeeded.\n");
  else
    printf("'print_type_file()' test has failed.\n");
  system("rm link_dir1");
  system("rmdir dir1");
}

void test_print_perm_file(){
  printf("\n");
  system("echo hello world >file1.txt");
  if(!print_perm_file("file1.txt", 1))
    printf("'print_perm_file()' test has succeeded.\n");
  else
    printf("'print_perm_file()' test has failed.\n");
  system("rm file1.txt");
}

void test_print_ownership(){
  printf("\n");
  system("echo hello world >file1.txt");
  if(!print_ID_owner("file1.txt", 1))
    printf("'print_ID_owner()' test has succeeded.\n");
  else
    printf("'print_ID_owner()' test has failed.\n");
  system("rm file1.txt");
}

void test_print_size(){
  printf("\n");
  system("echo hello world >file1.txt");
  if(!print_size_file("file1.txt", 1))
    printf("'print_size_file()' test has succeeded.\n");
  else
    printf("'print_size_file()' test has failed.\n");
  system("rm file1.txt");
}

void test_read_dir(){
  printf("\n");
  char pwd[1024];
  getcwd(pwd, 1024);
  if(!read_dir(pwd))
    printf("'read_dir()' test has succeeded.\n");
  else
    printf("'read_dir()' test has failed.\n");
}

int main(int argc, char const *argv[]) {
  test_print_type_file();
  test_print_perm_file();
  test_print_ownership();
  test_print_size();
  test_read_dir();

  return 0;
}
