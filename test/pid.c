#include <stdbool.h>
#include <stdlib.h>
#include <dsp.h>

typedef struct {
	bool init;

	float kp, ki, kd;
	float delta_time;

	float integral;
	float derivative;
	float error;
	float error_old;
} pid_t;

pid_t * pid_new_controller(float kp, float ki, float kd, float delta_time)
{
	pid_t * ret = (pid_t*)malloc(sizeof(pid_t));
	ret->kp = kp;
	ret->ki = ki;
	ret->kd = kd;
	ret->delta_time = delta_time;

	ret->integral = 0.0f;
	ret->derivative = 0.0f;
	ret->error = 0.0f;
	ret->error_old = 0.0f;
	ret->init = true;

	return ret;
}

float pid_control(pid_t * pid_handle, float SP, float feedback)
{
	if(!pid_handle)
		return 0.0f;

	pid_handle->error = SP - feedback;
	pid_handle->integral += pid_handle->error * pid_handle->delta_time;
	pid_handle->derivative = (pid_handle->error - pid_handle->error_old) / pid_handle->delta_time;
	pid_handle->error_old = pid_handle->error;

	return (pid_handle->kp * pid_handle->error) + (pid_handle->ki * pid_handle->integral) + (pid_handle->kd * pid_handle->derivative);
}

int main(int argc, char ** argv)
{
	signal_t * signal_position = dsp_signal(0, 1000, 1);
	signal_t * signal_error = dsp_signal(0, 1000, 1);

	float SP = 10;      /* Set Point                              */
	float position = 0; /* Simulate the response of this variable */

	/* Create new PID Controller handle */
	pid_t * pid = pid_new_controller(
		0.15, /* kp (Proportional constant)    */
		0.00, /* ki (Integral     constant)    */
		0.00, /* kd (Derivative   constant)    */
		1.00  /* The rate at which time passes */
	);

	/* Control position until it reaches SP */
	foreach(signal_position, x, y) {
		var(signal_error, y_error);

		/* Store data (position) */
		y[xn] = position;

		/* Update PID and simulate feedback response */
		position += pid_control(pid, SP, position) + (rand() % 2);

		/* Store data (error) */
		y_error[xn] = pid->error;

		continue; /* Keep the terrain static (comment this line otherwise) */

		/* Simulate a dynamic set point varying across time */
		if(xn >= 30 && xn < 50) {
			SP = 5;
		} else if(xn >= 50 && xn < 60) {
			SP = 8;
		} else if(xn >= 60) {
			SP = 12;
		}
	}

	/* Cleanup and output the data */
	free(pid);
	dsp_export(signal_position);
	dsp_export(signal_error);
	dsp_exportend_to_csv(argv[1]);

	return 0;
}
