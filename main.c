// main.c
// 耀鈞說做這個可以讓電腦壞掉數分鐘，請勿模仿

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef long long num;

struct Factor {
  num prime;
  int power;
};

// ----------------------
// FUNCTIONS DECLARATIONS
// ----------------------

struct Factor *realloc_result(struct Factor *pResult, int *current_size);

// @brief Add a factor to result.
// This will increase current_length by 1 if success.
int add_to_result(struct Factor *pResult,
    int *current_size,
    int *current_length,
    const struct Factor factor);

// @brief Factorize the number and store it to pResult.
// @param pResult_return Just give a pointer, you don't need to malloc it.
// @param result_length The length of pResult ,you don't need to set it.
// @param kProgressMaxLength The progress bar's max length.
// @return Return status code. 0 for success, -1 for error.
int factorize(
    struct Factor **pResult_return,
    int *result_length,
    num number,
    const int kProgressMaxLength);

// ----------
// MAIN
// ----------

int main(int argc, char **argv) {
  num number;

  if (argc <= 1) {
    printf("Needs an argument\n");
    printf("Try: %s 210\n", argv[0]);
    return -1;  // User didn't give any argument.
  }
  {
    char *endptr;
    number = (num) strtoll(argv[1], &endptr, 10);  // Base = 10.
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


  const int kProgressMaxLength = 30;
  struct Factor *pResult = NULL;
  int result_length;
  int status = factorize(&pResult, &result_length, number, kProgressMaxLength);
  if (status == -1) {
    printf("Got an error in factorize function\n");
    return -1;
  }

  // Print the result.
  for (int i = 0; i < result_length; i++) {
    printf("%lld^%d * ", pResult[i].prime, pResult[i].power);
  }
  printf("\b\b\b   \n");

  free(pResult);

  return 0;
}

// ------------------------
// FUNCTION IMPLEMENTATIONS
// ------------------------

struct Factor *realloc_result(struct Factor *pResult, int *current_size) {
  if (pResult == NULL) {
    return NULL;  // pResult isn't available.
  }
  *current_size *= 2;  // Double the result_size.
  // Reallocated result, bigger and can store more results.
  struct Factor *new_pResult = realloc(pResult, *current_size * sizeof(struct Factor));
  if (new_pResult == NULL) {
    free(pResult);
    return NULL;
  }
  *pResult = *new_pResult;
  return pResult;
}

int add_to_result(struct Factor *pResult,
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
  pResult[*current_length] = factor;
  (*current_length)++;  // Increase the result_size of pResult.
  return 0;
}

int factorize(
    struct Factor **pResult_return,
    int *result_length,
    num number,
    const int kProgressMaxLength) {
  // Initialize result_length.
  *result_length = 0;

  // The size of pResult.
  // This will be bigger when there are more results inside.
  int result_size = 2;

  // Malloc pResult.
  struct Factor *pResult;
  pResult = (struct Factor *) malloc(sizeof(struct Factor) * result_size);
  if (pResult == NULL) {
    perror("malloc");
    return -1;  // Can't malloc.
  }

  // Backup the number because the code below will modify it.
  const int original_number = number;

  // Handle factor 2.
  int power_count = 0;
  while (number % 2 == 0) {
    number /= 2;
    power_count++;
  }
  if (power_count > 0) {
    struct Factor factor = {2, power_count};
    if (add_to_result(pResult, &result_size, result_length, factor) == -1) {
      // Add to result error.
      perror("add to result");
      return -1;
    }
  }

  // Handle factors after 2.
  for (num i = 3; i * i <= number; i += 2) {

    if (number == 1) {
      // Exit this for loop.
      break;
    }

    power_count = 0;
    while (number % i == 0) {
      number /= i;
      power_count++;
    }
    if (power_count == 0) {
      continue;  // Check next number (i).
    }

    // Initialize a struct Factor and add it to result.
    struct Factor factor;
    factor.prime = i;
    factor.power = power_count;
    // Add it to result.
    if (add_to_result(pResult, &result_size, result_length, factor) == -1) {
      perror("add to result");
      return -1;
    }

    // Update the progress bar.
    printf("\r|->");  // Start of progress bar.
    const int progress_length = (int) (((double) (original_number - number)) / original_number * kProgressMaxLength);
    for (int j = 0; j < progress_length; j++) {
      printf("#");
    }
    for (int j = 0; j < kProgressMaxLength-progress_length; j++) {
      printf(" ");
    }
    printf("<-|");  // End of progress bar.
    fflush(stdout);
  }

  // If number is still greater than 2, then it is a prime factor.
  // The number is a prime number.
  if (number > 2) {
    struct Factor factor = {number, 1};
    add_to_result(pResult, &result_size, result_length, factor);
  }

  // Print a finish progress bar.
  printf("\r|->");
  for (int i = 0; i < kProgressMaxLength; i++) {
    printf("#");
  }
  printf("<-|");
  printf("\n");  // Go out of progress bar.

  // Assign the result to pResult_return.
  // Because pResult is only available in this function.
  // And pResult_return is a double pointer.
  *pResult_return = pResult;

  return 0;
}
