#ifndef UTIL_H
#define UTIL_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

extern int sock;
extern MYSQL *mysql;

int connect_sock(const char *server_ip, int server_port);
int send_sock(const char *buffer, size_t length);
void disconnect_sock();

int connect_db(const char* host, int nPort, const char* szDatabase, const char* user, const char* passwd);
void disconnect_db(void);
int get_current_day_str(char **out);

#endif // UTIL_H
