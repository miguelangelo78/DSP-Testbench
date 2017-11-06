#include <dsp.h>

static signal_t signal_pool[MAX_SIGNAL_COUNT];
static unsigned int signal_pool_ctr = 0;

struct {
	int signal_count;
	float max_lower_bound;
	float max_upper_bound;
	float max_resolution;
	signal_t * signals[MAX_SIGNAL_COUNT];
} signal_export_fmt = {
	0, 0.0f, 0.0f, 0.0f
};

char dsp_export(signal_t * s) {
	if(signal_export_fmt.signal_count >= MAX_SIGNAL_COUNT)
		return 1;

	signal_export_fmt.signals[signal_export_fmt.signal_count] = s;
	signal_export_fmt.signal_count++;

	if(signal_export_fmt.max_lower_bound > s->lower_bound)
		signal_export_fmt.max_lower_bound = s->lower_bound;

	if(signal_export_fmt.max_upper_bound < s->upper_bound)
		signal_export_fmt.max_upper_bound = s->upper_bound;

	if(signal_export_fmt.max_resolution < s->resolution)
		signal_export_fmt.max_resolution = s->resolution;

	return 0;
}

void dsp_export_all(void) {
	for(int i = 0; i < signal_pool_ctr; i++)
		dsp_export(&signal_pool[i]);
}

void dsp_export_to_csv(char * csv_filename) {
	FILE * f = fopen(csv_filename, "w");
	char buff[(MAX_SIGNAL_COUNT * 128) + 3];

	for(float i = signal_export_fmt.max_lower_bound, i2 = 0; i <= signal_export_fmt.max_upper_bound; i += signal_export_fmt.max_resolution, i2++) {
		sprintf(buff, "%.10f", i);
		for(int j = 0; j < signal_export_fmt.signal_count; j++) {
			float sample;
			
			if(i2 < signal_export_fmt.signals[j]->sample_count)
				sample = signal_export_fmt.signals[j]->samples[(int)i2];
			else
				sample = 0.0f;

			sprintf(buff, "%s,%.4f", buff, sample);
		}
		fprintf(f, "%s\n", buff);
	}

	fclose(f);
}

void dsp_signalgroup_end(void) {
	for(int i = 0; i < signal_export_fmt.signal_count; i++)
		free(signal_export_fmt.signals[i]->samples);
	signal_pool_ctr = 0;
	signal_export_fmt.signal_count = 0;
	signal_export_fmt.max_lower_bound = 0.0f;
	signal_export_fmt.max_upper_bound = 0.0f;
}

signal_t * dsp_signal(float lower_bound, float upper_bound, float resolution) {
	if(signal_pool_ctr >= MAX_SIGNAL_COUNT)
		return NULL;

	signal_t * ret = &signal_pool[signal_pool_ctr++];
	ret->sample_count = ((abs(lower_bound) + abs(upper_bound)) + 1) / resolution;
	ret->lower_bound = lower_bound;
	ret->upper_bound = upper_bound;
	ret->resolution = resolution;
	ret->samples = (float*)malloc(sizeof(float) * ret->sample_count);
	memset(ret->samples, 0, sizeof(float) * ret->sample_count);
	return ret;
}
