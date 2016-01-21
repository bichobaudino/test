#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void print_frame(unsigned int length, FILE *file, unsigned int log_file);
void print_header(char format[][100], unsigned int frame_length,
		char title[][100], unsigned int columns, FILE *file,
		unsigned int log_file);
void print_columns(char format[][100], char title[][100], unsigned int columns,
		FILE *file, unsigned int log_file);
void logger(const char* msg, FILE *file, unsigned int log_file);
void get_format(char format[][100], int columns, int column_size[]);
int get_frame_length(int column_size[], int columns);
void compute_column_size(int column_size[], int columns, char data[][100]);

void get_time(struct tm *ts_system_time);
void get_string_from_time(struct tm *tm, char* buffer_system_time);
void get_time_difference(struct tm *ts_time1, struct tm *ts_time2, struct tm *diff);

int columns = 4;
int column_size[4];

int main(void) {

	char title[4][100] = { { "0-0-0 1:15:35" }, { "RX Units" }, {
			"TX Units" }, { "Error Units" } };
//	char slice_0[4][100] = { { "slice0" }, { "123456789123" },
//			{ "3395130360465" }, { "43471" } };
//	char slice_1[4][100] = { { "slice1" }, { "454355373702" },
//			{ "4534556343365" }, { "41" } };
//	char slice_2[4][100] = { { "slice2" }, { "768687867702" },
//			{ "6775675677765" }, { "12441" } };
//	char slice_3[4][100] = { { "slice3" }, { "213344233244" },
//			{ "2342344342225" }, { "0" } };




	char slice_0[4][100] = { { "slice0" }, { "" }, { "" }, { "" } };

	unsigned long long tx = 123456789012;
	unsigned long long rx = 123456789012;
	unsigned long long err = 1213431;
	char txU[100];
	char rxU[100];
	char errU[100];

	sprintf(txU, "%llu", tx);
	sprintf(rxU, "%llu", rx);
	sprintf(errU, "%llu", err);

	int y;
	for (y = 1; y < 4; y++) {

		switch (y) {
		case 1:
			strcpy(slice_0[y], rxU);
			break;
		case 2:
			strcpy(slice_0[y], txU);
			break;
		case 3:
			strcpy(slice_0[y], errU);
			break;
		default:
			break;
		}
	}

	FILE *file = NULL;

	memset(column_size, 0, sizeof(column_size));

	compute_column_size(column_size, columns, title);
	compute_column_size(column_size, columns, slice_0);
//	compute_column_size(column_size, columns, slice_1);
//	compute_column_size(column_size, columns, slice_2);
//	compute_column_size(column_size, columns, slice_3);

	char format[columns][100];

	get_format(format, columns, column_size);

	int frame_length = get_frame_length(column_size, columns);

	//printf("LENGTH: %i\n", frame_length);

	print_header(format, frame_length, title, columns, file, 0);
	print_columns(format, slice_0, columns, file, 0);
//	print_columns(format, slice_1, columns, file, 0);
//	print_columns(format, slice_2, columns, file, 0);
//	print_columns(format, slice_3, columns, file, 0);

//	struct tm ts_time1;
//	struct tm ts_time2;
//	struct tm diff;
//	char buffer_system_time[100];
//
//    get_time(&ts_time2);
//    sleep(10);
//    get_time(&ts_time1);
//
//	get_string_from_time(&ts_time1, buffer_system_time);
//	printf("time1: %s\n", buffer_system_time);
//
//
//	get_string_from_time(&ts_time2, buffer_system_time);
//	printf("time2: %s\n", buffer_system_time);
//
//	get_time_difference(&ts_time1, &ts_time2, &diff);
//
//	get_string_from_time(&diff, buffer_system_time);
//
//	printf("diff: %s\n", buffer_system_time);

	printf("\nEND\n");
	return (0);
}

void print_frame(unsigned int length, FILE *file, unsigned int log_file) {
	unsigned int i;

	for (i = 0; i < length; i++) {
		logger("-", file, log_file);
	}

	logger("\n", file, log_file);
}

void print_header(char format[][100], unsigned int frame_length,
		char title[][100], unsigned int columns, FILE *file,
		unsigned int log_file) {

	print_frame(frame_length, file, log_file);

	print_columns(format, title, columns, file, log_file);

	print_frame(frame_length, file, log_file);

}

void print_columns(char format[][100], char title[][100], unsigned int columns,
		FILE *file, unsigned int log_file) {
	unsigned int i;
	char msg[200] = { 0 };

	logger("|", file, log_file);

	for (i = 0; i < columns; i++) {
		sprintf(msg, format[i], title[i], '|');

		logger(msg, file, log_file);
	}

	logger("\n", file, log_file);
}

void logger(const char* msg, FILE *file, unsigned int log_file) {

	printf("%s", msg);

	if ((log_file) && (file != NULL)) {
		fprintf(file,"%s", msg);
	}
}

void get_format(char format[][100], int columns, int column_size[]) {

	char first[10];
	char second[10];

	int g;
	for (g = 0; g < columns; g++) {

		sprintf(first, "%d", column_size[g]+2);
		sprintf(second, "%d", column_size[g] / 2);

		char format_str[10] = { "%" };
		strcat(format_str, first);
		strcat(format_str, "s%");
		strcat(format_str, second);
		strcat(format_str, "c");

		strcpy(format[g], format_str);
	}
}

int get_frame_length(int column_size[], int columns) {
	int frame_length = 0;

	int i;
	for (i = 0; i < columns; i++) {
		frame_length += ((column_size[i]+2) + column_size[i] / 2);
	}

	return frame_length;
}

void compute_column_size(int column_size[], int columns, char data[][100]) {

	int i;
	for (i = 0; i < columns; i++) {

		if (strlen(data[i]) > column_size[i]) {
			column_size[i] = strlen(data[i]) + 5;
		}
	}
}

void get_time(struct tm *ts_system_time)
{
	time_t t = time(0);
	*ts_system_time = *localtime(&t);
	ts_system_time->tm_year += 1900;
	ts_system_time->tm_mon += 1;
}

void get_string_from_time(struct tm *tm, char* buffer_system_time)
{
	sprintf(buffer_system_time, "%d-%d-%d %d:%d:%d", tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void get_time_difference(struct tm *ts_time1, struct tm *ts_time2, struct tm *diff)
{

   int seconds_t1 = (ts_time1->tm_mday*24*3600) + (ts_time1->tm_hour*3600) + (ts_time1->tm_min*60) + ts_time1->tm_sec;
   int seconds_t2 = (ts_time2->tm_mday*24*3600) + (ts_time2->tm_hour*3600) + (ts_time2->tm_min*60) + ts_time2->tm_sec;

   int seconds = seconds_t1 - seconds_t2;

   printf("seconds: %d\n", seconds);

   diff->tm_year = ts_time1->tm_year - ts_time2->tm_year;
   diff->tm_mon = ts_time1->tm_mon - ts_time2->tm_mon;
   diff->tm_mday = seconds / 60 / 60 / 24;
   diff->tm_hour = (seconds / 60 / 60) % 24;
   diff->tm_min = (seconds / 60) % 60;
   diff->tm_sec = seconds % 60;
}

