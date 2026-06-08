#pragma once

#define DEBUG_OUT

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG_OUT

void debug_init();
void debug_log(const char *fmt, ...);
void debug_ring_log(const char *fmt, ...);
void debug_ring_flush();

#else

#define debug_init()                     ((void)0)
#define debug_log(...)                   ((void)0)
#define debug_ring_log(...)              ((void)0)
#define debug_ring_flush()               ((void)0)

#endif

#ifdef __cplusplus
}
#endif
