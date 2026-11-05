/* Chapter 4: a guarded header with a macro and a function declaration. */
#ifndef CHAPTER4_PREPROCESSOR_H
#define CHAPTER4_PREPROCESSOR_H

#define MAXIMUM(A, B) ((A) > (B) ? (A) : (B))

double average(int x, int y);

#endif
