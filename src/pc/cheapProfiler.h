#ifndef __CHEAP_PROFILER_H__
#define __CHEAP_PROFILER_H__

#ifdef USE_PROFILER
extern void ProfEmitEventStart(char *label);
extern void ProfEmitEventEnd(char *label);
extern void ProfSampleFrame();
#else
#define ProfEmitEventStart(...) ;
#define ProfEmitEventEnd(...) ;
#define ProfSampleFrame(...) ;
#endif

#endif /* __CHEAP_PROFILER_H__ */