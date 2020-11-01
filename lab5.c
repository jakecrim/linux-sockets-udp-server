// Server
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MSG_SIZE 40

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(void)
{
    struct sockaddr_in servAddr, cliAddr;
    bool boolVar = 1;
    char buffer[MSG_SIZE];

    int addrLength = sizeof(struct sockaddr_in);
    /*  SOCKET */
    //               UDP connection
    int sock = socket(AF_INET , SOCK_DGRAM , 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2000);    // CL
    servAddr.sin_addr.s_addr = INADDR_ANY;

    /* BIND */
    if(bind( sock , (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        error("Binding");

    /* SOCKET OPTIONS */
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolVar, sizeof(boolvar)))
    {
   		printf("error setting socket options\n");
   		exit(-1);
    }

    while(1)
    {
        // cleaning up buffer...
        bzero(buffer, MSG_SIZE);

        // receive from client
        if(recvfrom(sock, buffer, (struct sockaddr *)cliAddr, &addrLength) < 0)
            error("Receive Error");

        printf("Received: %s \n", buffer);
        // return if slave ***(MESSAGES ALWAYS CHAR ARRAYS SIZE 40)***

            // return student name and IP addr if this device is the master


        // if VOTE requested (if WHOIS received)

            cliAddr.sin_addr.s_addr = inet_addr("128.206.19.255"); // for example
            // broadcast random number generated between 1 and 10 and this device's IP addr to all boards

            // receive votes

            // decide which IP will become the master

                // compare all VOTES highest random number received

                // if tie, higher IP addr win

        if(sendto(sock, buffer, sizeof(buffer), (struct sockaddr*)&cliAddr, addrLength) < 0)
            error("Send To Error");
        
    }

    // start with slave status


    return 0;        

}