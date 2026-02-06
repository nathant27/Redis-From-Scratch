# Redis Notes

In memory key value store.
Mostly used for caching bc memory is faster than retrieving from disk

Helps get rid of some slow DB queries

Cache server is map<string, string> over network
BUT Redis value can be any data structure. Hash, list, or sorted set.
    - Allows for more complex use cases: ranking, list pagination, etc.

Redis is called a data structure server

---

Protocols have layers. Builds top down, so lower layer can contain higher layer as payload.
Higher layer adds new functions

Ex: Ethernet contains IP contains UDP/TCP, and then those ocntain application protocls

---

## IP Packets

Large file split into smaller units, called IP packets.

Lowest layer is packet based. 

Ability to make packets into application data is provided by higher layer, usually TCP

---

## Demultiplexing

Demultiplexing allows computer to know which packet belongs to which app.

16 bit port number to distinguish between diff apps. Each needs to claim a port number before it can send or receive data.

4 tuple IP
(src\_ip, src\_port, dst\_ip, dst\_port)

---

## TCP

File transfer case = large data.

TCP provides reliable, ordered bytes on top of IP packets. It handles retransmission, and reordering automatically.
Connection based.

---

## TCP/IP MODEL

Network Protocl layered by function:
     Subject Function
High TCP     Reliable & ordered bytes
 |   Port    Multiplex to programs
Low  IP      Small, discrete messages

TCP/IP Model represents protocol header structs. Puts TCP and UDP on same level, but TCP has a higher function. USP is just IP + port(so lacks the reliable and ordered part of TCP, but faster)

---

## Whats needed for Redis

Do not need most of IP layer, but we do need soure and destination address

Do not need Ethernet, bc we do not care about hardware address (MAC).

We DO care about layers above IP, aka Application layer, whihc use TCP or UDP.
    - Either use own protocol, or indirectly use implemenetation of well know n protocol.
    - This book makes its own

**EVERYTHING IS BUILT ON TOP OF TCP OR UDP**

Need IP(source addr, dest addr), port, TCP/UDP

---

## Request Response protocols.

Redis, HTTP/1.1, etc are request-response protocols. Each request has a response, which means TCP(bc ordered and reliable makes it nice to pair request with response)
    - DNS is an exception

---

## Packet v Stream

TCP provides byte stream, BUT typical apps expect messages. Most apps don't use byte stream unless they interpret it.

TWO OPTIONS:
    - Add message layer to TCP
    - Add reliability and order to UDP (package based)

WHICH IS EASIER: FORMER. Adding message layer to TCP is lots easier, and most apps use TCP

**TCP AND UDP ARE FUNCTIONALLY DIFFERENT AND SEMANTICALLY INCOMPATIBLE**

Therefore, choice between TCP and UDP is the first choice to decide for networked applications.

---

## Sockets

Socket = handle to refer to connection. Just has to do with the fact that APi for netoworking is called socket API.

handle is the int (file descriptor on linux).
    - local to process
    - its a file descriptor (bc everything is a file in linux), but has nothing to do with describing the file.

FUNCTION:

socket() allocates and returns socket fd (handle), which is used LATER to create connections

**HANDLE MUST BE CLOSED WHEN YOU'RE DONE**
In order to free associated resources on OS side. 
    - Common between different handles

## Listening Socket
Listening = tells app to accept TCP connections from a given port.

OS then returns a socket handle for apps to refer to that port(on the client side)

FROM listening socket, on OS side, apps can retrieve (accept) incoming TCP connections. Accept is blocking.


Steps to Create Listening socket (>= 3 API CALLS)
    1. Obtain socket handle with socket()
    2. Set listening IP:port via bind()
    3. Create listening socket with listen()

Get handle, set info, make it listen on a port, and then make it listen


## Connection Socket. AKA connecting from a client

Steps to create from client side (>= 2 API Calls)
    1. Obtain socket with socket()
    2. Create connection socket with connect()


Important Note:
socket() creates typeless socket. listen() or connect() determines the socket type.

bind() just sets parameter to be used for listen. Because you need to listen on a port.
For the client, you are just connecting to an address on a port. But the socket itself is just used for connecting, so don't need bind
setsockopt() API can set other socket params that will be used later. idk for what yet.

## Reading and Writing

TCP and UDP share  same socket API.
    - Includes send() and recv() methods (safer than read and write system calls)
    - send/recv append and consume from byte stream(TCP is byte stream, remember)
    - send/recv are variants of generic read/write syscalls. read/write syscalls can be used for sockets, disk files, pipes, etc. Everything is just a "file" to linux system, so you're always reading and writing to it. But UDP and TCP are functionally different, so don't expect code that works for both TCP and UDP
