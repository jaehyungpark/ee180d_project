#include "feature_detector.h"
#include "math_func.h"
// Divide the stride into 4 segmentations
// Extract max min on each segmentation
Feature* extract_feature(double* data, double* time, int* S_i, int n_S,
	int feature_case)
{
	int i;
	Feature *features = (Feature*) malloc(sizeof(Feature) * n_S-1);
	
	for(i = 0; i< n_S-1; i++){
		// Segmentation
		float step = (float)(S_i[i+1] - S_i[i]);
		int pos0 = S_i[i];
		int pos1 = S_i[i] + (int)(step / 4.0);
		int pos2 = S_i[i] + (int)(step / 2.0);
		int pos3 = S_i[i] + (int)(3.0 * step / 4.0);
		int pos4 = S_i[i+1];

		switch(feature_case) {
			case GLOBAL:
				max(data, pos0, pos1, &features[i].seg0.max);
				min(data, pos0, pos1, &features[i].seg0.min);
				//Add seg max/min + period to array
				max(data, pos1, pos2, &features[i].seg1.max);
				min(data, pos1, pos2, &features[i].seg1.min);
				//Add seg max/min + period to array
				max(data, pos2, pos3, &features[i].seg2.max);
				min(data, pos1, pos2, &features[i].seg2.min);
				//Add seg max/min + period to array
				max(data, pos3, pos4, &features[i].seg3.max);
				min(data, pos3, pos4, &features[i].seg3.min);
				//Add seg max/min + period to array
				break;
			case WALK:
				mean(data, pos0, pos1, &features[i].seg0.mean);
				mean(data, pos1, pos2, &features[i].seg1.mean);
				mean(data, pos2, pos3, &features[i].seg2.mean);
				mean(data, pos3, pos4, &features[i].seg3.mean);
				variance(data, pos0, pos1, &features[i].seg0.var);
				variance(data, pos1, pos2, &features[i].seg1.var);
				variance(data, pos2, pos3, &features[i].seg2.var);
				variance(data, pos3, pos4, &features[i].seg3.var);
				break;
		}
		
	}
	return features;
}




void global_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, const char* ofile_feature_name)
{
	/* Generic variables */
	int i;
	FILE *fp;

	// Feature needed
	Feature* features0;
	Feature* features1;
	double* period;


	/* feature detection section*/
	// compute period
	period = (double*) malloc(sizeof(double)*(n_S - 1));
	for(i = 0; i < n_S-1; i++){
		period[i] = time[S_i[i+1]] - time[S_i[i]];
	}

	//2 indices divided by 4 
	features1 = extract_feature(gyro_y,time,S_i,n_S,GLOBAL);
	features0 = extract_feature(accel_y,time,S_i,n_S,GLOBAL);

	printf("Attempting to write to file \'%s\'.\n", ofile_feature_name);
	fp = fopen(ofile_feature_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_feature_name
		       );
		exit(EXIT_FAILURE); 
	}

	fprintf(fp, "Seg0_Max,Seg0_Min,Seg1_Max,Seg1_Min,Seg2_Max,Seg2_Min,Seg3_Max,Seg3_Min,Period\n");
	for(i = 0; i < n_S-1; i++){
		fprintf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%20.10lf\n",
		 	features0[i].seg0.max, features0[i].seg0.min, 
			features0[i].seg1.max, features0[i].seg1.min,
			features0[i].seg2.max, features0[i].seg2.min,
			features0[i].seg3.max, features0[i].seg3.min,
			features1[i].seg0.max, features1[i].seg0.min, 
			features1[i].seg1.max, features1[i].seg1.min,
			features1[i].seg2.max, features1[i].seg2.min,
			features1[i].seg3.max, features1[i].seg3.min,
			period[i]);
	}
	fclose(fp);
}

void walk_feature(double* accel_x,
	double* time, int* S_i, int n_S, const char* ofile_walk_feature_name)
{
	/* Generic variables */
	int i;
	FILE *fp;

	// Feature needed
	Feature* features0;
	double* period;


	/* feature detection section*/
	// compute period
	period = (double*) malloc(sizeof(double)*(n_S - 1));
	for(i = 0; i < n_S-1; i++){
		period[i] = time[S_i[i+1]] - time[S_i[i]];
	}

	//2 indices divided by 4 
	features0 = extract_feature(accel_x,time,S_i,n_S,WALK);

	printf("Attempting to write to file \'%s\'.\n", ofile_walk_feature_name);
	fp = fopen(ofile_walk_feature_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_walk_feature_name
		       );
		exit(EXIT_FAILURE); 
	}

	fprintf(fp, "Seg0_mean,Seg0_var,Seg1_mean,Seg1_var,Seg2_mean,Seg2_var,Seg3_mean,Seg3_var,Period\n");
	for(i = 0; i < n_S-1; i++){
		fprintf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%20.10lf\n",
		 	features0[i].seg0.mean, features0[i].seg0.var, 
			features0[i].seg1.mean, features0[i].seg1.var,
			features0[i].seg2.mean, features0[i].seg2.var,
			features0[i].seg3.mean, features0[i].seg3.var,
			period[i]);
	}
	fclose(fp);
}