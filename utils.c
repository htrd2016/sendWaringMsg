#include <stdio.h>
#include <sys/types.h>
//#include <unistd.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "utils.h"

MYSQL *mysql = 0;
int sock = 0;

int connect_sock(const char *server_ip, int server_port)
{
  struct sockaddr_in addr;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock<=0)
  {
    printf("error:%s\n", strerror(errno));
    return -1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(server_port);
  addr.sin_addr.s_addr = inet_addr(server_ip);
  printf("socket try to connect....\n");
  if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) <0)
  {
    printf("%s\n", strerror(errno));
    close(sock);
    return -1;
  }
  return 0;
}

int send_sock(const char *buffer, size_t length)
{
  return send(sock, buffer, length, 0);
}

void disconnect_sock()
{
  if(sock != 0)
  {
    close(sock);
    sock = 0;
    printf("socket closed\n");
  }
}

int connect_db(const char* host, int nPort, const char* szDatabase, const char* user, const char* passwd)
{
    printf("connect_db ..... \n");

    mysql = mysql_init(NULL);

    if (mysql == 0)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        return -1;
    }

    if (mysql_real_connect(mysql, host, user, passwd, szDatabase, nPort, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }

    mysql_query(mysql, "SET NAMES utf8");

    printf("connect mysql sucess!\n");
    return (0);
}

void disconnect_db(void)
{
    if (mysql)
    {
        mysql_close(mysql);
        mysql = 0;
        mysql_server_end();
        printf("disconnected_db\n");
    }
}

int get_current_day_str(char **out)
{
  time_t timep;
  struct tm *p;
  time(&timep); /*获得time_t结构的时间，UTC时间*/
  p = gmtime(&timep); /*转换为struct tm结构的UTC时间*/
  //sprintf((char*)(*out), "%d-%02d-%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
  sprintf((char*)(*out), "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,
         p->tm_hour, p->tm_min, p->tm_sec);
  printf("%s", *out);
  return 0;  
}
