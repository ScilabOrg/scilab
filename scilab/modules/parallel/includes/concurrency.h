#ifndef CONCURRENCY_H
#define CONCURRENCY_H
/* can return 0: no -> concurrency, 1 -> threads, 2 -> processes, 3 -> both */
int concurrency(void);
int forbidden(char const* fname);
#endif
