#CISC 611-90-O-2019/Late Spring - Network Operating Systems 

###### Homework-2

###### Author: Youwei Lu

## Compilation
Under the same path of `.java` files, use command `javac` to generate `.class` files.
Specifically, execute the following commands:

```
javac UDPClient.java
```

```
javac UDPServer.java
```

```
javac TCPClient.java
```

```
javac TCPServer.java
```

## UDP Connection Test
1. Open one terminal at the path of `UDPServer.class`, run the server with a chosen port.
 
    Usage: `java UDPServer <server port>`. 
    
    For example, run the following command:
    ```
    java UDPServer 6789
    ```
    Keep this terminal open.
2. Open another terminal at the path of `UDPClient.class`, run the client with the same port number used by the UDP
server and the host name. If on the same machine, host name can be "localhost".
One can choose to run the client with automatically generation of messages (Exercise 4.3) or repeatedly reading a line 
of client from the user (Exercise 4.4) by giving different numbers of commands arguments.
    1. #### Client with automatically generation of messages
        Specify the number of message to be sent and length of messages in the arguments.
        Usage: `java UDPClient <hostname> <server port> <number of message to be sent> <length of messages>`.
        For example, run the following command:
        ```
        java UDPClient localhost 6789 1000 10
        ```
        Responses will be shown in the client terminal, and an error will be shown indicating the order of message if 
        a timeout has been reached.
    2. #### Client with repeatedly reading a line from user
        Usage: `java UDPClient <hostname> <server port>`.
        For example, run the following command:
        ```
        java UDPClient localhost 6789
        ```
        Responses will be shown in the client terminal, and an error will be shown if a timeout has been reached.
        
        In the client terminal, user can input `quit` to quit.

## TCP Connection Test
1. Open one terminal at the path of `TCPServer.class`, run the server with a chosen port.
 
    Usage: `java TCPServer <server port>`. 
    
    For example, run the following command:
    ```
    java TCPServer 7896
    ```
    Keep this terminal open.
2. Open another terminal at the path of `TCPClient.class`, run the client with the same port number used by the TCP
server and the host name. If on the same machine, host name can be "localhost".
   Usage: `java UDPClient <hostname> <server port>`.
   For example, run the following command:
   ```
   java TCPClient localhost 7896
   ```
   Responses will be shown in the server terminal. In the server terminal, press the <kbd>Enter</kbd> key to shown 
   the next line of responses display.
          
   In the client terminal, user can input `quit` to quit.
