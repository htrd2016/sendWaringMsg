export LD_LIBRARY_PATH=/usr/lib/:LD_LIBRARY_PATH

sudo cp -f ./libmysqlclient.a /usr/lib/
sudo cp -f ./libmysql.so.16.0.0 /usr/lib/
sudo ln -sf /usr/lib/libmysql.so.16.0.0 /usr/lib/libmysql.so
sudo ln -sf /usr/lib/libmysql.so.16.0.0 /usr/lib/libmysql.so.16
sudo ln -sf /usr/lib/libmysql.so.16.0.0 /usr/lib/libmysqlclient.so
sudo ln -sf /usr/lib/libmysql.so.16.0.0 /usr/lib/libmysqlclient_r.so

sudo echo /usr/lib > /etc/ld.so.conf.d/local.conf
