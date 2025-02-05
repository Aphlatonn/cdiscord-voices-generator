#ifndef DATA_H
#define DATA_H

struct config {
	struct {
		char prefix[128];
	} bot;
};

typedef struct {
	struct config config;
} data_t;

#endif // DATA_H
