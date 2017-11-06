#include <dft.h>
#include <stdbool.h>

bool dsp_dft(signal_t * input, signal_t * output, float frequency, float sampling_frequency) {
	signal_t * output_re = dsp_signal(output->lower_bound, output->upper_bound, output->resolution);
	signal_t * output_im = dsp_signal(output->lower_bound, output->upper_bound, output->resolution);

	/* Perform DFT */
	foreach(output_re, k, y_re) {
		var(output_im, y_im);
		var(output, y_spectrum);

		y_re[kn] = 0;
		y_im[kn] = 0;

		foreach(input, x, y) {
			y_re[kn] += y[xn] * cos((pi / input->sample_count) * xn * (kn - ((output->sample_count / 2))) * 2) / input->sample_count;
			y_im[kn] += y[xn] * sin((pi / input->sample_count) * xn * (kn - ((output->sample_count / 2))) * 2) / input->sample_count;
		}

		y_spectrum[kn] = sqrt(pow(y_re[kn], 2) + pow(y_im[kn], 2));
	}

	return true;
}
