export LD_LIBRARY_PATH=/usr/lib/:LD_LIBRARY_PATH
nohup ./sendMsg 192.168.103.158 60000 127.0.0.1 username password 3306 dbname > send.log &
