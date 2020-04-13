#include "atlacan.h"
#include <stdio.h>

#define FAILED_PTR_NULL 2

void print_type_file(const int mode) {
  if(atl_is_file(mode))
    printf("It'a place (reg file).\n");
  else if(atl_is_dir(mode))
    printf("It's a pathway (dir).\n");
  else if(atl_is_link(mode))
    printf("Oh no ! It's a trap !\n");
  else
    printf("Mode unknown.");
}

void test_ptr(void *ptr, const char* name) {
  if(!ptr) {
    printf("Error: ptr '%s' is NULL.");
    exit(FAILED_PTR_NULL);
  }
}

void Found_atlacan(int num_tab, int init_clue){
	// printf("Start function\n");
	num_tab+=1;
	int clue =0, num_entry =0;
	char ** paths;
	atl_ls(&paths,&num_entry);
	struct atl_stat atl_stat_buf;

	for(int i=0; i<num_entry; i++){

		if(!atl_stat(paths[i],&atl_stat_buf)){

			if(atl_is_file(atl_stat_buf.mode)     &&
		  	!atl_is_link(atl_stat_buf.mode)     &&
		  	(atl_stat_buf.uid == UID_OTHER)     &&
		  	(atl_stat_buf.gid != GID_PERTONTAN) &&
		  	(atl_stat_buf.gid == GID_ABITHAN))

			{
				for(int i=0; i<num_tab; i++)
					printf(" | ");

				atl_cat(paths[i],&clue);
				if(init_clue  ==  -1)
					init_clue = clue;
				else
					return;

				printf("%s, clue: %d\n", paths[i], clue);
			}

			else if(atl_is_dir(atl_stat_buf.mode)     &&
						 (atl_stat_buf.uid != UID_CLAYTON)  &&
						 atl_is_ormode(atl_stat_buf.mode))

			{
				atl_cd(paths[i]);
				for(int i=0; i<num_tab; i++)
					printf(" | ");

				printf("dir:%s\n", paths[i]);
				Found_atlacan(num_tab, init_clue);
				atl_cd("..");
			}

			else if(!atl_is_file(atl_stat_buf.mode)		&&
							!atl_is_dir(atl_stat_buf.mode) 		&&
							!atl_is_link(atl_stat_buf.mode))

			{
				return;
			}
		}
	}
	atl_free_ls(paths, num_entry);
}

void Found_Atlacan2() {
  // Variables //
  int clue = 0, num_entry = 0;
  char **paths;
  // Read all file
  atl_ls(&paths, &num_entry);
  struct atl_stat atl_stat_buf;

  // Read all file //
  for(int i=0; i<num_entry; i++) {

    // show information of the file i //
    if(!atl_stat(paths[i], &atl_stat_buf)) {

      // test if is a file //
      if(atl_is_file(atl_stat_buf.mode)  &&
        !atl_is_link(atl_stat_buf.mode)  && // Isn't a trap
        (atl_stat_buf.gid  !=  GID_PERTONTAN)  && // isn't pertontan tribe
        (atl_stat_buf.size/1000000)  >=  1) // treasure
      {
        // printf("\nClue in '%s' is: %d\n", paths[i], clue);
        printf("\nSize of treasure: %d million of golds\n", atl_stat_buf.size/1000000);
        atl_cat(paths[i], &clue);
      }

      // Test if is a directory //
      else if(atl_is_dir(atl_stat_buf.mode)
        &&  (atl_stat_buf.uid  !=  UID_CLAYTON) // Isn't a trap by Clayton
        &&  atl_is_ormode(atl_stat_buf.mode)) // We can't access
      {
        atl_cd(paths[i]);
        printf("Go to: %s\n", paths[i]);
        Found_Atlacan();
        printf("Go back\n");
        atl_cd("..");
      }

      else if(!atl_is_file(atl_stat_buf.mode)
          &&  !atl_is_link(atl_stat_buf.mode)
          &&  !atl_is_dir(atl_stat_buf.mode))
      {
        printf("Type of file unknown\n");
        return;
      }
    }
  }
  atl_free_ls(paths, num_entry);
}

int main(int argc, char const *argv[])
{
	atl_init();
	Found_atlacan(-1, -1);
	atl_fini();
	return 0;
}
