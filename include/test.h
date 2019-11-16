#pragma once

#ifndef TEST_H_
#define TEST_H_

#include <stdio.h> /* fprintf */

/* Author: Shmuel Hanoch (C) */

#define RED     "\033[1m\033[31m"
#define GREEN   "\033[1m\033[32m"
#define RESET   "\033[0m"

typedef enum { TEST_FAIL = 0, TEST_PASS = 1 } TestResult;

#define REQUIRE(cond) \
do { \
	if (!(cond)) { \
		fprintf(stderr, \
				"\nAssertion failed at" RED " %s:%d" RESET " %s ", \
				__FILE__,__LINE__,#cond); \
				return TEST_FAIL; \
	} \
} while (0)

#define RUN_TEST(test) \
do { \
	fprintf(stderr, "Running "#test"... "); \
	if (test() == TEST_PASS) { \
		fprintf(stderr, GREEN "[OK]" RESET "\n"); \
	} else { \
		fprintf(stderr, RED "[Failed]" RESET "\n"); \
	} \
} while(0)

#endif /* TEST_H_ */
