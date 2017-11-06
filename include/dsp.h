#ifndef _DSP_H_
#define _DSP_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define STRSTR(x) #x
#define STR(x) STRSTR(x)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_SIGNAL_COUNT 16

#define foreach(s, x, y) for(float * y = s->samples, x ## r = s->resolution, x = s->lower_bound / s->resolution; y <= s->samples; y++) for(int x ## n = 0; x ## n < s->sample_count; x ## n++, x += x ## r)
#define forrange(s, x, y, start, end) for(float * y = s->samples, x ## r = s->resolution, x = (start) / s->resolution; y <= s->samples; y++) for(int x ## n = (int)(start); x ## n < s->sample_count && x ## n < (end); x ## n++, x += x ## r)
#define var(s, y) float * y = s->samples;

typedef struct {
	int sample_count;
	float lower_bound;
	float upper_bound;
	float resolution;
	float * samples;
} signal_t;

#define dsp_exportend(csv_filename, s) dsp_export(s); dsp_export_to_csv(csv_filename); dsp_signalgroup_end();
#define dsp_exportend_all(csv_filename) dsp_export_all(); dsp_export_to_csv(csv_filename); dsp_signalgroup_end();
#define dsp_exportend_to_csv(csv_filename) dsp_export_to_csv(csv_filename); dsp_signalgroup_end();

extern char dsp_export(signal_t * s);
extern void dsp_export_all(void);
extern void dsp_export_to_csv(char * csv_filename);
extern void dsp_signalgroup_end(void);
extern signal_t * dsp_signal(float lower_bound, float upper_bound, float resolution);

/* Constants/Transformations */
#define pi M_PI
#define rad(degrees) ((degrees) * M_PI / 180)
#define deg(radians) ((radians) * 180 / M_PI)
#define sign(y) (((y) > 0) ? 1 : 0)
#define set(signal, nth) signal.samples[nth]
#define fill(signal, ...) do { float vals[] = { __VA_ARGS__ }; memcpy(signal->samples, vals, sizeof(float) * signal->sample_count); } while(0);

#endif
