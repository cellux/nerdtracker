#ifndef NT_DECODE_UTF8_H
#define NT_DECODE_UTF8_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

uint32_t decode_utf8(uint32_t* state, uint32_t* codep, uint32_t byte);

#ifdef __cplusplus
}
#endif

#endif
