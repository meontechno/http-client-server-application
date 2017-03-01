RUNNING PROCEDURE

server side:
gcc -o httpServer httpServer.c -lpthread
./httpServer <port_number>

client side:
gcc -o httpClient httpClient.c 
./httpClient <host_name> <port_num> <GET/POST/HEAD> <'pagename.html'>



(OR)
*********************
CS 547 Programming Assignment 1
Emmanuel A. Castillo
Nithesh Singh Sanjay

Description: Develop a simple HTTP server and client to handle
data communication within a network. This is accomplished in C++
mainly driven by the socket library.

*********************
---------------------------------------------------------------
HOW TO COMPILE IN EMPRESS
---------------------------------------------------------------
1) Get files from git repo
git clone https://github.com/3m4nuel/CS537-Webserver.git

2) Compile cpp files
g++ *.cpp -o HttpServer -lpthread

---------------------------------------------------------------
HOW TO RUN
---------------------------------------------------------------
1) With port
./HttpServer 7080

2) Without port (defaults to 8080)
./HttpServer

--------------------------------------------------------------
HOW TO TEST WITH BROWSER (GET request)
--------------------------------------------------------------
1) Get ip address of your linux instance
/sbin/ifconfig
  a) your ip address is within "eth0" at "inet addr"
  b) e.g. inet addr:142.38.2.12 (ip address = 142.38.2.12)

2) On a browser connected on the same network as your linux box,
type in your ip address and port of the running HTTP server
  a) Successful Response Test
	142.38.2.12/getfile.html
	142.38.2.12/ (defaults to index.html)

  b) Unsucessful Response Test
        142.38.2.12/thisfiledoesnotexist.html

-------------------------------------------------------------
HOW TO TEST WITH HTTP CLIENT (GET, POST, HEAD)
-------------------------------------------------------------
