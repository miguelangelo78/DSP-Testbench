#include <dsp.h>

int main(int argc, char ** argv)
{
	signal_t * signal = dsp_signal(0, 360, 1);

	foreach(signal, x, y)
		y[xn] = sin(rad(x));

	dsp_export(signal);
	dsp_exportend_to_csv(argv[1]);

	return 0;
}
