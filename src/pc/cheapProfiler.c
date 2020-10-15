#ifdef USE_PROFILER
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <errno.h>

#include "cheapProfiler.h"

// Simple convertion constants
#define S_IN_NS (1e+9)
#define NS_IN_MS (1.0f / 1e+6)

// Limits
#define MAX_PROFILER_SLOTS 128
#define MAX_LABEL_SIZE 128

typedef struct EventSlot {
    int needs_sampling;
    double total;
    struct timespec start;
    char label[MAX_LABEL_SIZE];
} EventSlot;

static int cur_event_frame = 0;
static int events_allocated = 0;
static EventSlot event_slots[MAX_PROFILER_SLOTS] = {};
static FILE *f = NULL;


// Returns difference between two timespec structures in nanoseconds
double diff_timespec(struct timespec *t1, struct timespec *t2) 
{
    struct timespec td;
    td.tv_nsec = t2->tv_nsec - t1->tv_nsec;
    td.tv_sec  = t2->tv_sec - t1->tv_sec;

    if (td.tv_sec > 0 && td.tv_nsec < 0) {         
        td.tv_nsec += S_IN_NS;         
        td.tv_sec--;
    }     
    else if (td.tv_sec < 0 && td.tv_nsec > 0) {         
        td.tv_nsec -= S_IN_NS;         
        td.tv_sec++;     
    }

    return td.tv_nsec + td.tv_sec * S_IN_NS;
}

//getProfilerSlot: Returns slot on success, otherwise -1
static int getProfilerSlot(char *label)
{
    int i;
    for (i = 0; i < events_allocated; i++) {
        if (strcmp(label, event_slots[i].label) == 0)
            return i;
    }

    return -1;
}

void ProfEmitEventStart(char *label)
{
    EventSlot *ev;

    int slot;
    if ((slot = getProfilerSlot(label)) == -1) {
        // Create new event if we don't have one
        ev = &event_slots[events_allocated++];
        strncpy(ev->label, label, MAX_LABEL_SIZE);
        ev->total = 0;
        ev->needs_sampling = 0;
    } else {
        ev = &event_slots[slot];
    }

    if (ev->needs_sampling == 1)
        printf("Warning: Event %s has been started without being ended.\n", label);

    clock_gettime(CLOCK_MONOTONIC, &ev->start);
    ev->needs_sampling = 1;
}

void ProfEmitEventEnd(char *label)
{
    EventSlot *ev;

    int slot;
    if ((slot = getProfilerSlot(label)) == -1)
        return;

    ev = &event_slots[slot];
    if (ev->needs_sampling == 0)
        printf("Warning: Event %s has been ended before a start.\n", label);

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    double ms_diff = diff_timespec(&ev->start, &end) * NS_IN_MS;
    ev->total += ms_diff;

    ev->needs_sampling = 0;
}

void ProfSampleFrame()
{
    if (!f) {
        f = fopen("profiler_samples.json", "w+");
        if (!cur_event_frame && !f)
            fprintf(stderr, "Profiler failed to start, %s.\n", strerror(errno));
        return;
    }

    //Begin a data-point
    #define START_DP ((i == 0) ? "{": "")
    //End a data-point
    #define END_DP ((i == events_allocated-1) ? "}\n": ", ") 

    for (int i = 0; i < events_allocated; i++) {
        EventSlot *ev = &event_slots[i];
        if (ev->needs_sampling)
            fprintf(stderr, "Frame ended with event %s end still pending.\n", ev->label);

        fprintf(f, "%s\"%s\": %.03f%s", START_DP, ev->label, ev->total, END_DP);
        ev->total = 0;
    }
    #undef HAS_LF_OR_COMMA

    fflush(f);
    cur_event_frame++;
}
#endif /* USE_PROFILER */