#include <stdio.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"
#include <math.h>

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/file.h>
#include <stdlib.h>
#include <string.h>

#define MILLION 1000000.0

sig_atomic_t volatile run_flag = 1;

void do_when_interrupted(int sig) {
	if (sig == SIGINT)
		run_flag = 0;
}

void collect_data() {
	//Variables used for data collection
	struct timeval start, end;
	double start_epoch, end_epoch;
	mraa_i2c_context accel, gyro;
	float a_res, g_res;
	accel_scale_t a_scale;
	gyro_scale_t g_scale;
	data_t ad, gd;
	//Variables for file
	int fd;
	char* file_name;
	FILE *fp;

	file_name = malloc(sizeof(char) * 1024);
	memset(file_name, 0, 1024);
	sprintf(file_name, "file_%ld.csv", time(NULL));

	//timer variable
	int sec = 0, trigger = 10; 

	//Set up sensor configuration & read data
	a_scale = A_SCALE_6G;
	g_scale = G_SCALE_500DPS;

	accel = accel_init();
	set_accel_ODR(accel, A_ODR_100);
	set_accel_scale(accel, a_scale);
	a_res = calc_accel_res(a_scale);

	gyro = gyro_init();
	set_gyro_ODR(gyro, G_ODR_190_BW_70);
	set_gyro_scale(gyro, g_scale);
	g_res = calc_gyro_res(g_scale);


	fp = fopen(file_name, "w");
	if(fp==NULL) {
		fprintf(stderr, "Cannot create file %s/n",  file_name);
		exit(EXIT_FAILURE);
	}
	//Lock the file
	fd = fileno(fp);
	flock(fd, LOCK_EX);
	clock_t start_pt = clock();
	fprintf(fp, "timestamp_before, timestamp_after, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z\n");
	do {
		gettimeofday(&start, NULL);
		ad = read_accel(accel, a_res);
		gd = read_gyro(gyro, g_res);
		gettimeofday(&end, NULL);

		start_epoch = start.tv_sec + start.tv_usec / MILLION;
		end_epoch = end.tv_sec + end.tv_usec / MILLION;
		fprintf(fp, "%f, %f, %+f, %+f, %+f, %+f, %+f, %+f\n", start_epoch, end_epoch, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);
		usleep(100);
		//Check timer
		clock_t diff = clock() - start_pt;
		sec = diff / CLOCKS_PER_SEC;
	} while (sec < trigger);

	//Clean up memory
	free(file_name);
	fclose(fp);
}

int main() {
	signal(SIGINT, &do_when_interrupted);

	while(run_flag) {
		collect_data();
		sleep(2);
	}
	//Clean up memory
	exit(EXIT_SUCCESS);
}
