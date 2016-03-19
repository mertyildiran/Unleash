/* This file was automatically generated.  Do not edit! */
utmp_close();
utmp_open(char *filename);
struct utmp *utmp_next();
int Who();
void showtime(time_t);
void showtime(time_t timeval);
void show_info(struct utmp *);
void show_info(struct utmp *utbufp);
