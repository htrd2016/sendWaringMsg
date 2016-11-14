#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "utils.h"

#define BUFFER_HEAD_LEN 20

extern MYSQL *mysql;
int timestamp = -1;

int getCurrentTimestamp()
{
  time_t t;
  return time(&t);
}

int query_alerts_and_send()
{
  MYSQL_RES *res_ptr;
  MYSQL_ROW mysql_row;
  int nRet = 0;
  int nCount = 0;
  int nClock = 0;

  char row_data[1024] = {0};
  char buffer_to_send[1024] = {0};

  char sql[1024] = {0};
  sprintf(sql, "select clock,message,alerttype from alerts where clock>%d order by eventid asc", timestamp);
  
  printf("%s\n", sql);
  int res = mysql_query(mysql, sql);
  if (res)
  {
     fprintf(stderr, "SELECT error: %s\n", mysql_error(mysql));
     return -1;
  }
  else
  {
     res_ptr = mysql_store_result(mysql);
     if (res_ptr)
     {
       nCount = mysql_num_rows(res_ptr);
       if(nCount <= 0)
       {
         mysql_free_result(res_ptr);
         return 0;
       }

       while((mysql_row = mysql_fetch_row(res_ptr)))
       {
         strcpy(row_data, mysql_row[1]);
         nClock = atoi(mysql_row[0]);
         timestamp = nClock>timestamp?nClock:timestamp;
         //printf("[0:%s\n1:%s\n2:%s]\n", mysql_row[0], row_data, mysql_row[2]);
         sprintf(buffer_to_send, "%20u%s", (unsigned int)strlen(row_data), row_data);
         printf("%s\n", buffer_to_send);
         nRet = send_sock(buffer_to_send, strlen(row_data)+BUFFER_HEAD_LEN);
         if(nRet <=0 )
         {
            break;
            nRet = -1;
         }
       }

       if(mysql_errno(mysql))
       {
         fprintf(stderr, "Retrive error: %s\n", mysql_error(mysql));
         nRet = -1;
       }
       mysql_free_result(res_ptr);
     }
  }
  return nRet;
}

int main(int argc, char ** argv)
{
  char server_ip[64];
  int server_port = -1;

  char mysql_ip[64]={0};
  int mysql_port = -1;
  char mysql_username[512] = {0};
  char mysql_pass[512] = {0};
  char mysql_db[512] = {0};
  int time_interval = 5;
  //timestamp = 1447131104;//getCurrentTimestamp();
  timestamp = getCurrentTimestamp();
 
  printf("timestamp %d\n", timestamp);

  if(argc<8)
  {
    printf("use age<path><server ip><server port><mysql ip><mysql user name><mysql password><mysql port><mysql database>\n");
    return -1;
  }

  strcpy(server_ip, argv[1]);
  server_port = atoi(argv[2]);
  strcpy(mysql_ip, argv[3]);
  strcpy(mysql_username, argv[4]);
  strcpy(mysql_pass, argv[5]);
  mysql_port = atoi(argv[6]);
  strcpy(mysql_db, argv[7]);

  printf("server_ip:%s, port:%d, mysql ip:%s, mysql user name:%s, mysql port:%d, mysql db:%s\n", 
        server_ip, server_port, mysql_ip, mysql_username, mysql_port, mysql_db);

  for (;;)
  {
    if (0!=connect_db(mysql_ip, mysql_port, mysql_db, mysql_username, mysql_pass))
    {
        printf("connect mysql faild: server=%s,port=%d,username=%s,database=%s failed(%s)\n",
            mysql_ip, mysql_port, mysql_username, mysql_db, strerror(errno));
        sleep(time_interval);
        continue;
    }

    if (0!=connect_sock(server_ip, server_port))
    {
       printf("connect socket failed: server:%s, port:%d\n", server_ip, server_port);
       disconnect_db();
       sleep(time_interval);
       continue;
    }
    printf("---------------\n");
    for(;;)
    {
       if(query_alerts_and_send()<0)
       {
          break;
       } 
       sleep(1);
    }
    disconnect_sock();
    disconnect_db();
    sleep(1);    
  }
  return 0;
}
