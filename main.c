// main.c
// 耀鈞說做這個可以讓電腦壞掉數分鐘，請勿模仿

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Factor {
  int prime;
  int power;
};

struct Factor *realloc_result(struct Factor **pResult, int *current_size);

// @brief Add a factor to result.
// This will increase current_length by 1 if success.
int add_to_result(struct Factor **pResult,
    int *current_size,
    int *current_length,
    const struct Factor factor);

int main(int argc, char **argv) {
  int number = 120960;

  // If number is less than 0.
  if (number <= 0) {
    printf("Error. number <= 0\n");
    return -1;
  }

  int result_size = 2;  // Start size of pResult.
  int result_length = 0;  // Length of pResult.
  struct Factor *pResult = malloc(sizeof(struct Factor) * result_size);  // Result.
  if (pResult == NULL) {
    perror("malloc");
    return -1;  // Can't malloc.
  }

  bool is_prime = true;
  // Find prime numbers and check if the number is one of "number"'s factor.
  for (int i = 2; i <= number; i++) {
    if (number == 1) {
      // Exit this for loop.
      break;
    }

    // Check if i is a prime number.
    is_prime = true;  // If is false, next number (i);
    for (int j = 2; j < i; j++) {
      if (i % j == 0) {
        is_prime = false;
        break;
      }
    }
    if (!is_prime) {
      // "i" isn't a prime number, next i.
      continue;
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
  }

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
