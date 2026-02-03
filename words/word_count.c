/*
 * Implementation of the word_count interface.
 *
 * Copyright © 2019 University of California, Berkeley
 */

#include "word_count.h"

/* Basic utilities */

char *new_string(char *str) {
  return strcpy((char *)malloc(strlen(str) + 1), str);
}

void init_words(WordCount **wclist) {
  /* Initialize word count. */
  *wclist = NULL;
}

size_t len_words(WordCount *wchead) {
  size_t len = 0;
  WordCount *curr = wchead;
  while (curr != NULL) {
    len++;
    curr = curr->next;
  }
  return len;
}

WordCount *find_word(WordCount *wchead, char *word) {
  /* Return count for word, if it exists */
  WordCount *curr = wchead;
  while (curr != NULL) {
    if (strcmp(curr->word, word) == 0) {
      return curr;
    }
    curr = curr->next;
  }
  return NULL;
}

void add_word(WordCount **wclist, char *word) {
  /* If word is present in word_counts list, increment the count, otw insert with count 1. */
  WordCount *wc = find_word(*wclist, word);
  if (wc != NULL) {
    wc->count++;
  } else {
    WordCount *new_node = (WordCount *) malloc(sizeof(WordCount));
    new_node->word = new_string(word);
    new_node->count = 1;
    new_node->next = *wclist;
    *wclist = new_node;
  }
}

void fprint_words(WordCount *wchead, FILE *ofile) {
  /* print word counts to a file */
  WordCount *wc;
  for (wc = wchead; wc; wc = wc->next) {
    fprintf(ofile, "%i\t%s\n", wc->count, wc->word);
  }
}
