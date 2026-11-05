/*
 * Exercise 4-10 - Calculator declarations.
 */
#ifndef EXERCISE_4_10_CALC_H
#define EXERCISE_4_10_CALC_H

#define MAXOP 100
#define NUMBER '0'
#define INVALID '?'
#define BUFSIZE 100
#define MAXVAL 100



int getop(char s[]);
int push(double value);
double pop(void);
int stack_size(void);
void clear_stack(void);
double peek(void);
int duplicate(void);
int swap_top(void);

#endif
