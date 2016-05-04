# Simple server-client app on Winsock
========================

ABOUT
----------- 

Sockets (eng. socket - recess, socket, connector) - the name of the
program interface for providing data exchange between processes. 
The processes in such an exchange may be executed on one computer 
or on different computers connected by a network. Socket - abstract 
object representing a connection endpoint.

There are two types of network applications using sockets: 
the server and the client. The clients and servers of different 
behavior, respectively, and the process of creating them is also 
different.

The model for the creation of the server:

- Initialize WinSock Application

- Socket creation

- Binding socket
	 
- Listening socket

- A request for a connection

- Sending and receiving data

- Shutdown

A model for creating client:

- Initialize WinSock Application

- Socket creation

- The connection to the server

- Sending and receiving data

- Shutdown

Task of programs
-----------------

When connecting to the server, the client must enter the password. 
Allowed to make three attempts. If the password is incorrect, the 
server should block the IP address of the client for 2 minutes.

The server on each client allocates a new thread. 
Threads are synchronized on the mutex. The server for 
every command the client sends a response depending on command.

The server only knows about 2 clients: user, root.

At the moment users the following commands are available:

User: 

 - "exit" - disconnect from server

Root:

- "getbanlist" - get list of banned IPS

- "unban %i%" - unban IP, when %i% is number of ip adress in banlist

- "exit" - disconnect from server


Passwords
-----------

User: PaSs4332

Root: root 