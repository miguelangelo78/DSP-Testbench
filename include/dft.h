#ifndef _DFT_H_
#define _DFT_H_

#include <stdbool.h>
#include <dsp.h>

extern bool dsp_dft(signal_t * input, signal_t * output, float frequency, float sampling_frequency);

#endif
