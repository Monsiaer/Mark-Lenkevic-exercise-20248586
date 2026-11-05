/* Chapter 4.4: share an external variable across translation units. */
#ifndef CHAPTER4_SCOPE_H
#define CHAPTER4_SCOPE_H

extern int shared_count;
void increment_count(void);
int persistent_count(void);

#endif
