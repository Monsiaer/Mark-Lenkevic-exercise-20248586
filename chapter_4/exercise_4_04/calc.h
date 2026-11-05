/*
 * Exercise 4-4 - Calculator declarations.
 */
#ifndef EXERCISE_4_04_CALC_H
#define EXERCISE_4_04_CALC_H

#define MAXOP 100
#define NUMBER '0'
#define INVALID '?'
#define BUFSIZE 100
#define MAXVAL 100

int getch(void);
void ungetch(int c);
int getop(char s[]);
int push(double value);
double pop(void);
int stack_size(void);
void clear_stack(void);
double peek(void);
int duplicate(void);
int swap_top(void);

#endif
