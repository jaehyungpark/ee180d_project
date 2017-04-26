CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SRC=consumer
SOURCES=${SRC}/math_func.c ${SRC}/consumer.c ${SRC}/test_neural_network.c ${SRC}/train_neural_net.c ${SRC}/imu_data.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c
EXECUTABLES=$(SOURCES:.c=)

all: consumer

consumer: consumer.c stride_detector.c feature_detector.c math_func.c exe_neural_network.c
	$(CC) $(CFLAGS) -o consumer consumer.c feature_detector.c stride_detector.c math_func.c exe_neural_network.c -lm $(LDFLAGS)

clean:
	rm -f consumer
	rm -f *~
	rm -f GLOBAL.net
	rm -f WALK.net
	rm -f test_data.txt