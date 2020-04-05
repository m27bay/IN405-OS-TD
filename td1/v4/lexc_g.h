#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define TRUE 1
#define FALSE 0

#define EXIT_PTR_NULL 2
#define EXIT_FAILED_ADD_OR_DELL 3
#define EXIT_STR_EMPTY 4
#define EXIT_FULL 5

#define SIZENAME 256

struct word{
  char *word;

  struct word *next;
};

struct lexc{
  char *name;
  int mutable;

  struct word *first;
};
// Ou lexc_check
int is_here(struct lexc *lexc, const char *word);

struct lexc *lexc_init(const char *name, const int mutable);
void lexc_fini(struct lexc *lexc);

int word_add(struct lexc *lexc, const char *word);
int word_remove(struct lexc *lexc, const char *dell_word);

void lexc_aff(struct lexc *lexc);
void word_aff(struct lexc *lexc);

void execute();
struct lexc *cmd_interpret(struct lexc *lexc, const char *cmd);
