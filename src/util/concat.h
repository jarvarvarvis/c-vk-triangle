#ifndef VKT_UTIL_CONCAT_H
#define VKT_UTIL_CONCAT_H

// Use extra level of indirection to properly paste the tokens
// Source: https://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr
#define VKT_CONCAT_(x, y) x ## y
#define VKT_CONCAT(x, y) VKT_CONCAT_(x, y)

#endif /* VKT_UTIL_CONCAT_H */
