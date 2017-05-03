#!/bin/bash

DATA_DIR='new_data'
OUTPUT_DIR='new_data_results'
THRESHOLD=50
INPUT_NUM=13
OUTPUT_NUM=3

echo "Stride detection and feature selection on walking data files..."
./main $DATA_DIR/walk_slow.csv $OUTPUT_DIR/walk_slow_pt.csv $OUTPUT_DIR/walk_slow_st.csv $OUTPUT_DIR/walk_slow_feature.csv $THRESHOLD
./main $DATA_DIR/walk_med.csv $OUTPUT_DIR/walk_med_pt.csv $OUTPUT_DIR/walk_med_st.csv $OUTPUT_DIR/walk_med_feature.csv $THRESHOLD
./main $DATA_DIR/walk_fast.csv $OUTPUT_DIR/walk_fast_pt.csv $OUTPUT_DIR/walk_fast_st.csv $OUTPUT_DIR/walk_fast_feature.csv $THRESHOLD

printf "\n"

echo "Stride detection and feature selection on walking data files..."
./main $DATA_DIR/test_walk_slow.csv $OUTPUT_DIR/test_walk_slow_pt.csv $OUTPUT_DIR/test_walk_slow_st.csv $OUTPUT_DIR/test_walk_slow_feature.csv $THRESHOLD
./main $DATA_DIR/test_walk_med.csv $OUTPUT_DIR/test_walk_med_pt.csv $OUTPUT_DIR/test_walk_med_st.csv $OUTPUT_DIR/test_walk_med_feature.csv $THRESHOLD
./main $DATA_DIR/test_walk_fast.csv $OUTPUT_DIR/test_walk_fast_pt.csv $OUTPUT_DIR/test_walk_fast_st.csv $OUTPUT_DIR/test_walk_fast_feature.csv $THRESHOLD

printf "\n"

printf "\nGenerate train files...\n"
./train_file_generator $OUTPUT_DIR/walk_slow_feature.csv $OUTPUT_DIR/walk_med_feature.csv $OUTPUT_DIR/walk_fast_feature.csv $OUTPUT_DIR/train.txt $INPUT_NUM $OUTPUT_NUM

printf "\nGenerate test files...\n"
./train_file_generator $OUTPUT_DIR/test_walk_slow_feature.csv $OUTPUT_DIR/test_walk_med_feature.csv $OUTPUT_DIR/test_walk_fast_feature.csv $OUTPUT_DIR/test.txt $INPUT_NUM $OUTPUT_NUM

printf "\nTrain neural network...\n"
./train_neural_net $OUTPUT_DIR/train.txt $INPUT_NUM $OUTPUT_NUM

printf "\nTest neural network...\n"
./test_neural_network $OUTPUT_DIR/test.txt
