#ifndef __FEATURE_DETECTOR_H__
#define __FEATURE_DETECTOR_H__
/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
#include "math_func.h"

#define GLOBAL 0
#define TURN 1
#define WALK 2
#define STAIR 3
#define JUMP 4
#define RUN 5



typedef struct{
	double max;
	double min;
	double mean;
	double var;
	double rms;
	double mad;

} Seg_feature;

typedef struct{
	// segmentaion 0
	Seg_feature seg0;
	// segmentaion 1
	Seg_feature seg1;
	// segmentaion 2
	Seg_feature seg2;
	// segmentaion 3
	Seg_feature seg3;
} Feature;

Feature* extract_feature(double* data, double* time, int* S_i, int n_S, 
	int feature_case);

void global_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, const char* ofile_feature_name);

void walk_feature(double* accel_x,
	double* time, int* S_i, int n_S, const char* ofile_walk_feature_name);

/*
void turn_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_turn_feature_name);

void stair_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_stair_feature_name);

void jump_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_jump_feature_name);

void run_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_run_feature_name);
*/

#endif