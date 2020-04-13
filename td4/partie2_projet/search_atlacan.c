#include "atlacan.h"

#include "stdio.h"

#include "unistd.h"
#include "string.h"
#include "time.h"

/**
  * @autor LE DENMAT Mickael
  * @autor CAMBRESY Florian
  *
  * @version 2.0
  */

void Found_Atlacan_brute_force(int num_tab) {
  // Found Atlacan with "Brute-force" //

  // Variable
  int data = 0, num_entry = 0;
  char **name_file;

  // For print tree
  num_tab+=1;

  // Save file information
  atl_ls(&name_file, &num_entry);
  struct atl_stat st;

  // Read all file
  for(int i=0; i<num_entry; i++) {

    // show information of the file i
    if(!atl_stat(name_file[i], &st)) {

      if(atl_is_file(st.mode)) {
        if(!atl_is_link(st.mode)  && // test if isn't a trap
           atl_is_ormode(st.mode)) {


          // print tree
          for(int i=0; i<num_tab; i++)
            printf(" | ");

          atl_cat(name_file[i], &data);
          printf("%s, clue: %d\n", name_file[i], data);
        }
      }

      else if(atl_is_dir(st.mode)) {
        atl_cd(name_file[i]);

        // print tree
        for(int i=0; i<num_tab; i++)
          printf(" | ");

        printf("dir:%s\n", name_file[i]);
        Found_Atlacan_brute_force(num_tab);
        atl_cd("..");
      }
    }
  }
  atl_free_ls(name_file, num_entry);
}

void Found_Atlacan(int num_tab, int init_clue){
  // Function found Atlacan with clues //

  // Use for print tree
  num_tab+=1;

  // Variables
  int clue =0, num_file =0;
  char ** name_file;

  // Save informations of files
  atl_ls(&name_file, &num_file);
  struct atl_stat atl_stat_buf;

  // show all file in the directory
  for(int i=0; i<num_file; i++){

    // View informations of file
    if(!atl_stat(name_file[i],&atl_stat_buf)){

      if(atl_is_file(atl_stat_buf.mode)       &&
        !atl_is_link(atl_stat_buf.mode)       &&
        (atl_stat_buf.uid == UID_OTHER)       && // test if uid is other,
        (atl_stat_buf.gid != GID_PERTONTAN)   && // test if isn't a trap by pertontan
        (atl_stat_buf.gid == GID_ABITHAN))       // test is gid is abithan

      {
        // print tree
        for(int i=0; i<num_tab; i++)
          printf(" | ");

        atl_cat(name_file[i],&clue);

        // init_clue  =  -1    haven't read a clue in the directory yet
        if(init_clue  ==  -1)
          init_clue = clue;

        // we know Atlacan isn't is this directory
        else if(init_clue  >  0)
          return;

        printf("%s, clue: %d\n", name_file[i], clue);
      }

      else if(atl_is_dir(atl_stat_buf.mode)       &&
             (atl_stat_buf.uid != UID_CLAYTON)    && // test if isn't a wrong pathway by Clayton
             atl_is_ormode(atl_stat_buf.mode))

      {
        atl_cd(name_file[i]);

        // print tree
        for(int i=0; i<num_tab; i++)
          printf(" | ");

        printf("dir:%s\n", name_file[i]);
        Found_Atlacan(num_tab, init_clue);
        atl_cd("..");
      }

      else if(!atl_is_file(atl_stat_buf.mode)   &&
              !atl_is_dir(atl_stat_buf.mode)    &&
              !atl_is_link(atl_stat_buf.mode))

      {
        return;
      }
    }
  }
  // free
  atl_free_ls(name_file, num_file);
}

int main(int argc, char const *argv[])
{
  // Beginning //
  atl_init();

  /*
  printf("You start here: '%s'.\n\n", atl_getpwd());
  Found_Atlacan_brute_force(-1);
  */


  printf("With clues:\n\n");
  Found_Atlacan(-1, -1);


  // End //
  atl_fini();
  return 0;
}
