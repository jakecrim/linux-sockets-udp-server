// Home test lab5server
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MSG_SIZE 40			// message size

// Used so if our vote loses, it stops checking itself against the other board's votes
int greatestCurrentVote;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void generateVote(char * voteMsg)
{
    char addr[MSG_SIZE];
    strcpy(addr, "#128.206.19.125 ");
    // 1 -> 10
    int randomInt = (rand() % 10) + 1;
    char randomChar[3];

    sprintf(randomChar, "%d", randomInt);
    strcat(addr, randomChar); 
    strcpy(voteMsg, addr);

    printf("Vote Message: %s \n", voteMsg);
}

int decideMaster(char * myVote, char * otherVote)
{
    char * myVoteParsed[6];
    char * otherVoteParsed[6];
    char myVoteTemp[MSG_SIZE]; 
    int myVoteInt = 0;
    int otherVoteInt = 0;
    char * tempParse;

    strcpy(myVoteTemp, myVote);

    int i = 0;
    printf("My Vote: %s  | Other Vote: %s \n", myVoteTemp, otherVote);

    // Parse myVote
    tempParse = strtok(myVoteTemp, "#. ");
    while(tempParse != NULL)
    {
        myVoteParsed[i] = tempParse;
        tempParse = strtok(NULL, "#. ");
        i++;
    }
    // Parse otherVote
    i = 0;
    tempParse = strtok(otherVote, "#. ");
    while(tempParse != NULL)
    {
        otherVoteParsed[i] = tempParse;
        tempParse = strtok(NULL, "#. ");
        i++;
    }
    printf("My Vote Parsed: %s \n", myVoteParsed[4]);
    printf("Other Vote Parsed: %s \n", otherVoteParsed[4]);

    // turn the vote random num into an int
    sscanf(myVoteParsed[4], "%d", &myVoteInt);
    sscanf(otherVoteParsed[4], "%d", &otherVoteInt);

    // if our vote has not yet lost, compare votes to decide winner
    if(greatestCurrentVote == 0)
    {
        if(myVoteInt > otherVoteInt)
            return 1;
        else if(myVoteInt < otherVoteInt)
        {
            greatestCurrentVote = otherVoteInt;
            return 0;
        }
        // compare ip addrs if true
        else if(myVoteInt == otherVoteInt)
        {
            sscanf(myVoteParsed[3], "%d", &myVoteInt);
            sscanf(otherVoteParsed[3], "%d", &otherVoteInt);

            if(myVoteInt > otherVoteInt)
            {
                return 1;
            }
            greatestCurrentVote = otherVoteInt;
        }
    }


    return 0;
}

int main(void)
{
   int sock, length, n;
   int boolval = 1;			// for a socket option
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in addr;
   char buffer[MSG_SIZE];	// to store received messages or messages to be sent.
   char role[MSG_SIZE];


   srand(time(NULL));

    // initialize role as slave
    strcpy(role, "slave");
    // init buffer for testing
    strcpy(buffer, "VOTE");

   while (1)
   {
       printf("Received Msg: %s \n", buffer);

        /* Handle Possible Request */
        // status of board requested
        if(strcmp(buffer, "WHOIS") == 0)
        {
            printf("Role: %s \n", role);
        }

        // if vote for new master requested
        if(strcmp(buffer, "VOTE") == 0)
        {
            char voteMsg[MSG_SIZE];
            char otherVote[MSG_SIZE];

            // currently don't have a greatest vote
            greatestCurrentVote = 0;

            generateVote(voteMsg);

            // receive first other board vote
            strcpy(otherVote, "#128.206.19.126 4");
            printf("%c \n", otherVote[0]);

            // keep comparing votes until the newest message is no longer another board's vote message
            while(otherVote[0] == '#')
            {
                // decide winner
                if(decideMaster(voteMsg, otherVote))
                    strcpy(role, "master");
                else
                    strcpy(role, "slave"); 

                // receive next vote
                strcpy(otherVote, "#128.206.19.126 4");
                printf("Current Role: %s \n", role);
                printf("Greatest Vote: %d \n", greatestCurrentVote);
                printf("************************************\n");
                usleep(1000000);

            }
        }

        /* end of handling request */

        printf("Current Role: %s \n", role);
        usleep(1000000);

   }

   return 0;
 }



//  OLD WHILE LOOP BEFORE EDITING IT 11/1


//    while (1)
//    {
// 	   // bzero: to "clean up" the buffer. The messages aren't always the same length...
// 	   bzero(buffer,MSG_SIZE);		// sets all values to zero. memset() could be used

// 	   // receive from a client
// 	   n = recvfrom(sock, buffer, MSG_SIZE, 0, (struct sockaddr *)&addr, &fromlen);
//        if (n < 0)
//     	   error("recvfrom"); 

//        printf("Received a datagram. It says: %s", buffer);

//        // To send a broadcast message, we need to change IP address to broadcast address
//        // If we don't change it (with the following line of code), the message
//        // would be transmitted to the address from which the message was received.
// 	   // You may need to change the address below (check ifconfig)
//     //    addr.sin_addr.s_addr = inet_addr("128.206.19.17");		// broadcast address

//        n = sendto(sock, "Got a message. Was it from you?\n", 32, 0,
//     		      (struct sockaddr *)&addr, fromlen);
//        if (n  < 0)
//     	   error("sendto");
//    }