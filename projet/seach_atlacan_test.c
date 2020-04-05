#include "atlacan.h"

#include "stdio.h"
#include "unistd.h"
#include "string.h"

#define FAILED_PTR_NULL 2

void Found_Atlacan_brute_force2() {
  // Variables //
  int data = 0, num_entry = 0;
  char **paths;
  atl_ls(&paths, &num_entry);

  struct atl_stat st;

  // Read all file //
  for(int i=0; i<num_entry; i++) {

    // show information of the file i //
    if(!atl_stat(paths[i], &st)) {
      printf("%s\n", paths[i]);

      // test if is a file //
      if(atl_is_file(st.mode)) {
        if(!atl_is_link(st.mode)) {
          atl_cat(paths[i], &data);
          printf("clue: there is %d pathway(s) between me and Atlacan\n", data);
        }
      }

      // Test if is a directory //
      else if(atl_is_dir(st.mode)) {
        atl_cd(paths[i]);
        Found_Atlacan_brute_force2();
        atl_cd("..");
      }
    }
    printf("\n");
  }
  atl_free_ls(paths, num_entry);
}

void Found_Atlacan_test() {
  // Variables //
  int clue = 0, num_entry = 0;
  char **paths;
  atl_ls(&paths, &num_entry);
  struct atl_stat st;

  // Read all file //
  for(int i=0; i<num_entry; i++) {


      // show information of the file i //
      if(!atl_stat(paths[i], &st)) {

        // test if is a file //
        if(atl_is_file(st.mode)) {
          if(!atl_is_link(st.mode)) {
              int is_pertontan = (st.gid  ==  GID_PERTONTAN);
              if(!is_pertontan)
                atl_cat(paths[i], &clue);
            }
          }

        // Test if is a directory //
        else if(atl_is_dir(st.mode)) {
          int is_clayton = (st.uid  ==  UID_CLAYTON);
          if(atl_is_ormode(st.mode)  &&  !is_clayton) {
            atl_cd(paths[i]);
            printf("Go to: %s\n", paths[i]);
            Found_Atlacan_test();
            atl_cd("..");
          }
        }
      }
    atl_free_ls(paths, num_entry);
  }
}

int main(int argc, char const *argv[])
{
  // Beginning //
  atl_init();

  printf("You start here: '%s'.\n\n", atl_getpwd());
  Found_Atlacan_test();
  // Found_Atlacan_brute_force();

  // End //
  atl_fini();
  return 0;
}
