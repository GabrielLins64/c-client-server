/**
 * @file server.c
 * @brief Tutorial from https://www.linuxhowtos.org/C_C++/socket.htm
 * @date 2021-09-13
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /*
    In Unix and Unix-like computer operating systems, a file descriptor (FD, 
    less frequently fildes) is a unique identifier (handle) for a file or 
    other input/output resource, such as a pipe or network socket.
    - sockfd and newsockfd are file descriptors, i.e. array subscripts into the 
    file descriptor table. These two variables store the values returned by 
    the socket system call and the accept system call.
    - portno stores the port number on which the server accepts connections.
    - clilen stores the size of the address of the client. This is needed for 
    the accept system call.
    - n is the return value for the read() and write() calls; i.e. it contains 
    the number of characters read or written.
    */
    int sockfd, newsockfd, portno, clilen, n;
    const char *res = "From server: I got your message!";
    size_t res_size = 0;
    int option = 1;

    /* The server reads characters from the socket connection into this buffer. */
    char buffer[256];

    /* A sockaddr_in is a structure containing an internet address. This structure is defined in netinet/in.h. */
    struct sockaddr_in serv_addr, cli_addr;

    /* The user needs to pass in the port number on which the server will accept connections as an argument. This code displays an error message if the user fails to do this. */
    if (argc < 2)
        error("ERROR, no port provided");

    /*
    The socket() system call creates a new socket. It takes three arguments. The first is the address domain of the socket.
    Recall that there are two possible address domains, the unix domain for two processes which share a common file system, and the Internet domain for any two hosts on the Internet. The symbol constant AF_UNIX is used for the former, and AF_INET for the latter (there are actually many other options which can be used here for specialized purposes).
    The second argument is the type of socket. Recall that there are two choices here, a stream socket in which characters are read in a continuous stream as if from a file or pipe, and a datagram socket, in which messages are read in chunks. The two symbolic constants are SOCK_STREAM and SOCK_DGRAM.
    The third argument is the protocol. If this argument is zero (and it always should be except for unusual circumstances), the operating system will choose the most appropriate protocol. It will choose TCP for stream sockets and UDP for datagram sockets.
    The socket system call returns an entry into the file descriptor table (i.e. a small integer). This value is used for all subsequent references to this socket. If the socket call fails, it returns -1.
    In this case the program displays and error message and exits. However, this system call is unlikely to fail.
    This is a simplified description of the socket call; there are numerous other choices for domains and types, but these are the most common. The socket() man page (https://www.linuxhowtos.org/data/6/socket.txt) has more information.
    */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    /* The function bzero() sets all values in a buffer to zero. It takes two arguments, the first is a pointer to the buffer and the second is the size of the buffer. Thus, this line initializes serv_addr to zeros. */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    /* The port number on which the server will listen for connections is passed in as an argument, and this statement uses the atoi() function to convert this from a string of digits to an integer. */
    portno = atoi(argv[1]);

    /* The variable serv_addr is a structure of type struct sockaddr_in. This structure has four fields. The first field is short sin_family, which contains a code for the address family. It should always be set to the symbolic constant AF_INET. */
    serv_addr.sin_family = AF_INET;

    /* The second field of serv_addr is unsigned short sin_port, which contain the port number. However, instead of simply copying the port number to this field, it is necessary to convert this to *network byte order using the function htons() which converts a port number in host byte order to a port number in network byte order. */
    /*
    Virtually all computer architectures are byte addressable. If an int is four bytes, there are two different ways to store this. Suppose the address of the int is A. In a so-called big endian computer, the highest order byte is stored at A, and the lowest order byte is stored at address A+3. In a so-called little endian computer, address A stores the least significant byte and the most significant byte is at address A+3.
    Big endian computer architectures include the IBM 370, the Motorola 68000 and Sun Sparc. Little endian computers include the intel series (80486, pentium etc) and VAX.
    Consider the decimal integer 91,329. This is 00 01 64 C1 in hexidecimal. If this were to be stored at address A in a big endian computer, 00 would be at address A, 01 at address A+1 64 at address A+2, and C1 at address A+3. On a little endian computer, C1 would be the value at address A, 64 at address A+1, 01 at address A+2, and 00 at address A+3.
    Computer networks are big endian. This means that when little endian computers are going to pass integers over the network (IP addresses for example), they need to convert them to network byte order. Likewise, when the receive integer values over the network, they need to convert them back to their own native representation.
    There are four functions that do this.
    unsigned long htonl(unsigned long)
        host to network conversion for long ints (4 bytes)
    unsigned short htons(unsigned short)
        host to network conversion for short ints (2 bytes)
    unsigned long ntohl(unsigned long)
        network to host conversion for long ints 
    unsigned short ntohs(short)
        network to host conversion for short ints
    On big endian computers these functions simply return their arguments.
    The terms big endian and little endian are borrowed from Gulliver's Travels.
    */
    serv_addr.sin_port = htons(portno);

    /* The third field of sockaddr_in is a structure of type struct in_addr which contains only a single field unsigned long s_addr. This field contains the IP address of the host. For server code, this will always be the IP address of the machine on which the server is running, and there is a symbolic constant INADDR_ANY which gets this address. */
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* The bind() system call binds a socket to an address, in this case the address of the current host and port number on which the server will run. It takes three arguments, the socket file descriptor, the address to which is bound, and the size of the address to which it is bound. The second argument is a pointer to a structure of type sockaddr, but what is passed in is a structure of type sockaddr_in, and so this must be cast to the correct type. This can fail for a number of reasons, the most obvious being that this socket is already in use on this machine. The bind() manual (https://www.linuxhowtos.org/data/6/bind.txt) has more information. */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }

    /* The listen system call allows the process to listen on the socket for connections. The first argument is the socket file descriptor, and the second is the size of the backlog queue, i.e., the number of connections that can be waiting while the process is handling a particular connection. This should be set to 5, the maximum size permitted by most systems. If the first argument is a valid socket, this call cannot fail, and so the code doesn't check for errors. The listen() man page (https://www.linuxhowtos.org/data/6/listen.txt) has more information. */
    listen(sockfd, 5);

    /* The accept() system call causes the process to block until a client connects to the server. Thus, it wakes up the process when a connection from a client has been successfully established. It returns a new file descriptor, and all communication on this connection should be done using the new file descriptor. The second argument is a reference pointer to the address of the client on the other end of the connection, and the third argument is the size of this structure. The accept() man page has more information. */
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (unsigned int *) &clilen);

    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }

    /* Note that we would only get to this point after a client has successfully connected to our server. This code initializes the buffer using the bzero() function, and then reads from the socket. Note that the read call uses the new file descriptor, the one returned by accept(), not the original file descriptor returned by socket(). Note also that the read() will block until there is something for it to read in the socket, i.e. after the client has executed a write(). It will read either the total number of characters in the socket or 255, whichever is less, and return the number of characters read. The read() man page (https://www.linuxhowtos.org/data/6/read.txt) has more information. */
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    printf("Here is the message: %s", buffer);

    while (res[res_size] != '\0') res_size++;
    n = write(newsockfd, res, res_size);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }

    close(sockfd);
    close(newsockfd);

    return 0;
}
