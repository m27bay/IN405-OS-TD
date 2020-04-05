#include "lexc_g.h"

/**
  * Other tools
  *
  */

int is_here(struct lexc *lexc, const char *word)
{
  // Function which return if word is in the lexicon.

  // Found if word is in the lexicon.
  struct word *run=lexc->first;
  while(run)
  {
    if(!strcmp(run->word, word)) return TRUE;
    run=run->next;
  }

  return FALSE;
}


/**
  * Functions for handling (manipulation) lexicon.
  *
  */

struct lexc *lexc_init(const char *name, const int mutable)
{
  // Function which init a new empty lexicon without words inside.

  // Test if haven't 'name'.
  if(!name || name[0]=='\0')
    exit(EXIT_STR_EMPTY);

  // Creat new lexicon.
  struct lexc* NewLexicon=malloc(sizeof(struct lexc));
  if(!NewLexicon)
    exit(EXIT_PTR_NULL);

  // Give parameters.
  if(strlen(name)>SIZENAME)
  {
    puts("Name too big");
    exit(EXIT_FULL);
  }

  NewLexicon->name=calloc(SIZENAME, sizeof(char));
  strcpy(NewLexicon->name, name);
  NewLexicon->mutable=mutable;

  // First is the fistr word in the lexicon.
  NewLexicon->first=NULL;

  return NewLexicon;
}


void lexc_fini(struct lexc *lexc)
{
  // Function which releases lexicon and all words inside.

  // Release all words.
  struct word *run=lexc->first;
  while(run)
  {
    struct word *tmp = run;
    free(tmp->word);
    free(tmp);
    run=run->next;
  }

  // Release lexicon.
  free(lexc->name);
  free(lexc);
}

/**
  * Functions for handling words.
  *
  */

int word_add(struct lexc *lexc, const char *word)
{
  // Function which adds a new word in the lexicon.

  // Test if mutable == FALSE.
  if(!lexc->mutable)
  {
    puts("Error: Mutuable = FALSE");
    return 1;
  }

  // Test if word is already in the lexicon.
  if(is_here(lexc, word))
  {
    printf("\n'%s' is already in the lexicon.\n", word);
    return EXIT_FAILED_ADD_OR_DELL;
  }

  // Creation of the new word.
  struct word *new=malloc(sizeof(struct word));
  if(!new) exit(EXIT_PTR_NULL);

  // Give parameters.
  new->word=malloc(SIZENAME*sizeof(char));
  strcpy(new->word, word);
  new->next=NULL;

  // If the lexicon is empty.
  if(!lexc->first)
  {
    lexc->first=new;
    return 0;
  }

  // Add at the beginning.
  // new->next=lexc->first;
  // lexc->first=new;

  // Add at the end.
  struct word *run=lexc->first;
  if(!run) exit(EXIT_PTR_NULL);
  while(run->next) run=run->next;
  run->next=new;

  return 0;
}

int word_remove(struct lexc *lexc, const char *dell_word)
{
  // Function which removes dell_word in the lexicon.

  // Test if mutable == FALSE.
  if(!lexc->mutable)
  {
    puts("Error: Mutuable = FALSE");
    return 1;
  }

  // Test if word is not in the lexicon.
  if(!is_here(lexc, dell_word))
  {
    printf("\n'%s' is not in the lexicon, dell failed.\n", dell_word);
    return EXIT_FAILED_ADD_OR_DELL;
  }

  // Found where is dell_word.
  struct word *run=lexc->first;
  while((strcmp(run->next->word, dell_word)) && run) run=run->next;

  struct word *prev = run;
  struct word *follow = run->next->next;
  prev->next=follow;

  // Release dell_word.
  printf("Deletion of %s in the lexicon", dell_word);
  free(run->next);

  return 0;
}

/**
  *
  * Functions of displays (affichage).
  *
  */

void word_aff(struct lexc *lexc)
{
  // Function which displays all words in the lexicon.

  struct word *run=lexc->first;
  printf("\nBeginning of word:\n\n");
  if(!lexc->first || (!lexc->name[0]))
    puts("No words");

  else
  {
    printf("{");
    while(run)
    {
      printf(" %s ", run->word);
      run=run->next;
    }
    puts("}");
  }
}

void lexc_aff(struct lexc *lexc)
{
  // Function which displays the lexicon.

  // test lexicon empty
  if(!lexc)
    puts("Lexicon empty");

  else
  {
    printf("Name lexc: %s\nMutable? %d\n", lexc->name, lexc->mutable);

    // Display all words.
    word_aff(lexc);
  }
}

/**
  *
  * Functions for user intervention.
  *
  */

void execute()
{
  // Functon which deals user intervention.

  puts("Initialisation of a new lexicon...");
  puts("Command 'man': Print user manual. ");
  struct lexc *L=NULL;

  char action[SIZENAME];

  do
  {
    puts("\nGive me an action:");
    scanf("%s", action); puts("");
    L=cmd_interpret(L, action);
  } while(L);

}

struct lexc *cmd_interpret(struct lexc *lexc, const char *cmd)
{
  if(!strcmp("quit", cmd))
    return NULL;

  if(!strcmp("man", cmd))
  {
    puts("Command 'creat': Create a new lexicon.");
    puts("Command 'quit': Quit the menu.");
    puts("Command 'add': Add a new word in the lexicon.");
    puts("Command 'dell': Dell a word in the lexicon.");
    puts("Command 'print': Print the lexicon and all word inside.");
    puts("Command 'end': Release the lexicon and all word inside.");

    return lexc;
  }

  if(!strcmp("creat", cmd) && (!lexc))
  {
    puts("Give me a name for your lexicon:");
    char name[SIZENAME]; scanf("%s", name);

    puts("Your lexicon is mutable? TRUE:1 or FALSE:0.");
    int mutable;
    do{
      scanf("%d", &mutable);
    }while((mutable!=0) && (mutable!=1));

    lexc=lexc_init(name, mutable);

    return lexc;
  }

  else if(!strcmp("creat", cmd) && (lexc))
  {
    puts("The lexicon is already created");
    return lexc;
  }

  else if(strcmp("creat", cmd) && (!lexc))
  {
    puts("You need to creat your lexicon before");
    return lexc;
  }

  else if(lexc)
  {
    if(!strcmp("add", cmd))
    {
      puts("What word you want to add?");
      char new_word[SIZENAME]; scanf("%s", new_word);
      word_add(lexc, new_word);

      return lexc;
    }

    else if(!strcmp("dell", cmd))
    {
      puts("What word you want to remove?");
      char word_dell[SIZENAME]; scanf("%s", word_dell);
      word_remove(lexc, word_dell);

      return lexc;
    }

    else if(!strcmp("print", cmd))
    {
      lexc_aff(lexc);
      return lexc;
    }

    else if(!strcmp("end", cmd))
    {
      lexc_fini(lexc);
      return lexc;
    }

    else
    {
      printf("Command '%s': dosen't exist.", cmd);
      return lexc;
    }
  }
  else
  {
    printf("Command '%s': dosen't exist.", cmd);
    return lexc;
  }
}




