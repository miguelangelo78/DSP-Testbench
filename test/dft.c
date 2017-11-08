#include <dsp.h>

int main(int argc, char ** argv)
{
	signal_t * s1 = dsp_signal(0, 256, 1);
	signal_t * s2_re = dsp_signal(0, 256, 1);
	signal_t * s2_im = dsp_signal(0, 256, 1);
	signal_t * s2 = dsp_signal(0, 256, 1);

	float f = 2000;
	float fs = 15000;

	/* Create sinewave */
	foreach(s1, x, y)	
		y[xn] = 10 * sin(2 * pi * (f / fs) * x);

	/* Perform DFT */
	foreach(s2_re, k, y_re) {
		var(s2_im, y_im);
		var(s2, y_spectrum);

		y_re[kn] = 0;
		y_im[kn] = 0;

		foreach(s1, x, y) {
			y_re[kn] += y[xn] * cos((pi / s1->sample_count) * xn * (kn - ((s2->sample_count / 2))) * 2) / s1->sample_count;
			y_im[kn] += y[xn] * sin((pi / s1->sample_count) * xn * (kn - ((s2->sample_count / 2))) * 2) / s1->sample_count;
		}

		y_spectrum[kn] = sqrt(pow(y_re[kn], 2) + pow(y_im[kn], 2));
	}
	
	dsp_export(s1);
	dsp_export(s2);

	dsp_exportend_to_csv(argv[1]);

	return 0;
}
