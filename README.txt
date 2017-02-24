RUNNING PROCEDURE

server side:
gcc -o httpServer httpServer.c -lpthread
./httpServer <port_number>

client side:
gcc -o httpClient httpClient.c 
./httpClient <host_name> <port_num> <GET/POST/HEAD> <'pagename.html'>
