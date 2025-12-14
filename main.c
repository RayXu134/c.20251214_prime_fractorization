// main.c
// 耀鈞說做這個可以讓電腦壞掉數分鐘，請勿模仿

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Factor {
  int prime;
  int power;
};

int result_size;     // Start size of pResult is 2, assign later.
int result_length;   // Length of pResult.
struct Factor *pResult;  // Result.

struct Factor *realloc_result(struct Factor **pResult, int *current_size);

// @brief Add a factor to result.
// This will increase current_length by 1 if success.
int add_to_result(struct Factor **pResult,
    int *current_size,
    int *current_length,
    const struct Factor factor);

int main(int argc, char **argv) {
  int number;

  if (argc <= 1) {
    printf("Needs an argument\n");
    printf("Try: %s 210\n", argv[0]);
    return -1;  // User didn't give any argument.
  }
  {
    char *endptr;
    number = (int) strtol(argv[1], &endptr, 10);  // Base = 10.
    if (*endptr != '\0') {
      printf("The argument is not a number!\n");
      return -1;  // Argument is not a number.
    }
  }

  // If number is less than 0.
  if (number <= 0) {
    printf("Error. number <= 0\n");
    return -1;
  }

  result_size = 2;
  result_length = 0;
  pResult = malloc(sizeof(struct Factor) * result_size);
  if (pResult == NULL) {
    perror("malloc");
    return -1;  // Can't malloc.
  }

  const int kProgressMaxLength = 30;
  int progress_length;
  int original_number = number;  // Backup the number because the code
                                 // below will modify it.
  // Find prime numbers and check if the number is one of "number"'s factor.
  for (int i = 2; i <= number; i++) {

    if (number == 1) {
      // Exit this for loop.
      break;
    }

    // "i" is a prime number.
    int power_count = 0;
    while (number % i == 0) {
      number /= i;
      power_count++;
    }
    if (power_count == 0) {
      continue;
    }

    // Initialize a struct Factor and add it to result.
    struct Factor factor;
    factor.prime = i;
    factor.power = power_count;
    // Add it to result.
    if (add_to_result(&pResult, &result_size, &result_length, factor) == -1) {
      perror("add to result");
      return -1;
    }

    // Update the progress bar.
    printf("\r|->");  // Start of progress bar.
    progress_length = (int) (((double) (original_number - number)) / original_number * kProgressMaxLength);
    for (int j = 0; j < progress_length; j++) {
      printf("#");
    }
    for (int j = 0; j < kProgressMaxLength-progress_length; j++) {
      printf(" ");
    }
    printf("<-|");  // End of progress bar.
    fflush(stdout);
  }

  // Print a finish progress bar.
  printf("\r|->");
  for (int i = 0; i < kProgressMaxLength; i++) {
    printf("#");
  }
  printf("<-|");

  printf("\n");  // Go out of progress bar.

  // Print the result.
  for (int i = 0; i < result_length; i++) {
    printf("%d^%d * ", pResult[i].prime, pResult[i].power);
  }
  printf("\b\b\b   \n");

  free(pResult);

  return 0;
}

struct Factor *realloc_result(struct Factor **pResult, int *current_size) {
  if (*pResult == NULL) {
    return NULL;  // pResult isn't available.
  }
  *current_size *= 2;  // Double the result_size.
  struct Factor *new_pResult = realloc(*pResult, *current_size * sizeof(struct Factor));
  if (new_pResult == NULL) {
    free(*pResult);
    return NULL;
  }
  *pResult = new_pResult;
  return *pResult;
}

int add_to_result(struct Factor **pResult,
    int *current_size,
    int *current_length,
    const struct Factor factor) {
  if (pResult == NULL) {
    return -1;  // pResult isn't available.
  }
  if (*current_length >= *current_size) {
    // Reallocate pResult when it's too small.
    realloc_result(pResult, current_size);
    if (pResult == NULL) {
      return -1;  // Reallocation error.
    }
  }
  // Append factor to the end of pResult.
  (*pResult)[*current_length] = factor;
  (*current_length)++;  // Increase the result_size of pResult.
  return 0;
}
