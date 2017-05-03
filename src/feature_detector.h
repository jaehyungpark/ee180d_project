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
#define WALK 1

typedef struct{
	double max;
	double min;
	double mean;
	double var;
	double rms;
	double std;
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
	//absolute mean
	double abs_integral;
} Feature;

Feature* extract_feature(double* data, double* time, int* S_i, int n_S, int feature_case);

void global_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_feature_name);

void walk_feature(double* accel_x,
	double* time, int* S_i, int n_S, char* ofile_walk_feature_name);


#endif
