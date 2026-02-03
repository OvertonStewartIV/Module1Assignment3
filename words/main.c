/*

  Word Count using dedicated lists

 * Main file for the word count program.
 *
 * Copyright © 2019 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "word_count.h"

/* Global data structure tracking the words encountered */
WordCount *word_counts = NULL;

/* The maximum length of each word in a file */
#define MAX_WORD_LEN 64

/*
 * 3.1.1 Total Word Count
 *
 * Returns the total amount of words found in infile.
 * Useful functions: fgetc(), isalpha().
 */
int num_words(FILE* infile) {
  int num_words = 0;
  int c;
  bool in_word = false;

  while ((c = fgetc(infile)) != EOF) {
    if (isalpha(c)) {
      if (!in_word) {
        num_words++;
        in_word = true;
      }
    } else {
      in_word = false;
    }
  }

  return num_words;
}

/*
 * 3.1.2 Word Frequency Count
 *
 * Given infile, extracts and adds each word in the FILE to `wclist`.
 * Useful functions: fgetc(), isalpha(), tolower(), add_word().
 */
void count_words(WordCount **wclist, FILE *infile) {
  char word[MAX_WORD_LEN + 1];
  int len = 0;
  int c;

  while ((c = fgetc(infile)) != EOF) {
    if (isalpha(c)) {
      if (len < MAX_WORD_LEN) {
        word[len++] = tolower(c);
      }
    } else if (len > 0) {
      word[len] = '\0';
      add_word(wclist, word);
      len = 0;
    }
  }
  if (len > 0) {
    word[len] = '\0';
    add_word(wclist, word);
  }
}

/*
 * Comparator to sort list by frequency.
 * Useful function: strcmp().
 */
static bool wordcount_less(const WordCount *a, const WordCount *b) {
  if (a->count != b->count) {
    return a->count < b->count;
  }
  return strcmp(a->word, b->word) < 0;
}

int main(int argc, char *argv[]) {
  int count_mode = 0;
  int total_words = 0;
  int c;
  static struct option long_options[] = {
      {"count", no_argument, 0, 'c'},
      {"frequency", no_argument, 0, 'f'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0}
  };

  while ((c = getopt_long(argc, argv, "cfh", long_options, NULL)) != -1) {
    switch (c) {
      case 'c':
        count_mode = 1;
        break;
      case 'f':
        count_mode = 2;
        break;
      case 'h':
        printf("Usage: %s [-c | -f] [FILES...]\n", argv[0]);
        return 0;
    }
  }

  if (count_mode == 0) {
    printf("Please specify -c (count) or -f (frequency)\n");
    return 1;
  }

  if (optind == argc) {
    if (count_mode == 1) {
      total_words += num_words(stdin);
    } else {
      count_words(&word_counts, stdin);
    }
  } else {
    for (int i = optind; i < argc; i++) {
      FILE *infile = fopen(argv[i], "r");
      if (!infile) {
        perror("fopen");
        continue;
      }
      if (count_mode == 1) {
        total_words += num_words(infile);
      } else {
        count_words(&word_counts, infile);
      }
      fclose(infile);
    }
  }

  if (count_mode == 1) {
    printf("The total number of words is: %i\n", total_words);
  } else {
    wordcount_sort(&word_counts, wordcount_less);
    printf("The frequencies of each word are:\n");
    fprint_words(word_counts, stdout);
  }

  return 0;
}
