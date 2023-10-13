/* tcpClient.c */
/* Compile: gcc -o tcpClient tcpClient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* close */
#define SERVER_PORT 25
#define MAX_MSG 100
int main(int argc, char *argv[])
{
    int sd, rc;
    struct sockaddr_in localAddr, servAddr;
    struct hostent *h;
    char line[MAX_MSG];
    char msg[MAX_MSG];
    char fromID[MAX_MSG], toID[MAX_MSG], subject[MAX_MSG];
    if (argc < 3)
    {
        printf("usage: %s <server> <data>\n", argv[0]);
        exit(1);
    }
    h = gethostbyname(argv[1]);
    if (h == NULL)
    {
        printf("%s: unknown host '%s'\n", argv[0], argv[1]);
        exit(1);
    }
    servAddr.sin_family = h->h_addrtype;
    memcpy((char *)&servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servAddr.sin_port = htons(SERVER_PORT);
    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("cannot open socket ");
        exit(1);
    }
    /* bind any port number */
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(0);
    rc = bind(sd, (struct sockaddr *)&localAddr, sizeof(localAddr));
    if (rc < 0)
    {
        printf("%s: cannot bind port TCP %u\n", argv[0], SERVER_PORT);
        perror("error ");
        exit(1);
    }
    /* connect to server */
    rc = connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rc < 0)
    {
        perror("cannot connect ");
        exit(1);
    }
    rc = recv(sd, line, MAX_MSG, 0);
    if (rc < 0)
    {
        perror("cannot receive data ");
        close(sd);
        exit(1);
    }
    printf("Received %s:", line);
// *****************************************************************************************
    /* Sending HELO to the server */
    strcpy(msg, "HELO ");
    strcat(msg, argv[1]);
    strcat(msg, "\r\n");
    printf("\nPrinting msg = %s", msg);
    fflush(stdout);

    rc = send(sd, msg, strlen(msg), 0);
    if (rc < 0)
    {
        perror("cannot send data ");
        close(sd);
        exit(1);
    }

    memset(line, 0, MAX_MSG);
    rc = recv(sd, line, MAX_MSG, 0);
    if (rc < 0)
    {
        perror("cannot receive data ");
        close(sd);
        exit(1);
    }
    printf("\nReceived %s:", line);
// *********************************************************************************************
    /* Sender's mail id */
    printf("Enter Sender's email address: ");
    scanf("%s", fromID);
    strcpy(msg, "MAIL FROM: ");
    strcat(msg, fromID);
    strcat(msg, "\r\n");
    rc = send(sd, msg, strlen(msg), 0);
    if (rc < 0)
    {
        perror("cannot send data ");
        close(sd);
        exit(1);
    }

    memset(line, 0, MAX_MSG);
    rc = recv(sd, line, MAX_MSG, 0);
    if (rc < 0)
    {
        perror("cannot receive data ");
        close(sd);
        exit(1);
    }
    printf("\nReceived %s:", line);

    /* Receivers's mail id */
    printf("Enter Receiver's email address: ");
    scanf("%s", toID);
    strcpy(msg, "RCPT TO: ");
    strcat(msg, toID);
    strcat(msg, "\r\n");
    rc = send(sd, msg, strlen(msg), 0);
    if (rc < 0)
    {
        perror("cannot send data ");
        close(sd);
        exit(1);
    }

    memset(line, 0, MAX_MSG);
    rc = recv(sd, line, MAX_MSG, 0);
    if (rc < 0)
    {
        perror("cannot receive data ");
        close(sd);
        exit(1);
    }
    printf("\nReceived %s:", line);

    /* Subject of the mail */
    printf("Enter the subject of the message: ");
    scanf("%s", subject);
    strcpy(msg, "DATA");
    strcat(msg, "\r\n");
    rc = send(sd, msg, strlen(msg), 0);
    if (rc < 0)
    {
        perror("cannot send data ");
        close(sd);
        exit(1);
    }

    memset(line, 0, MAX_MSG);
    rc = recv(sd, line, MAX_MSG, 0);
    if (rc < 0)
    {
        perror("cannot receive data ");
        close(sd);
        exit(1);
    }
    printf("\nReceived %s:", line);
    // Now, send the email headers and body
    // char* emailHeaders = "From: sender@example.com\r\n"
    //                        "To: recipient@example.com\r\n"
    //                        "Subject: Your Subject Here\r\n"
    //                        "\r\n";  // Don't forget an empty line to separate headers and body
    char emailHeaders[256];
    strcpy(emailHeaders, "From: ");
    strcat(emailHeaders, fromID);
    strcat(emailHeaders, "\r\n");
    strcat(emailHeaders, "To: ");
    strcat(emailHeaders, toID);
    strcat(emailHeaders, "\r\n");
    strcat(emailHeaders, "Subject: ");
    strcat(emailHeaders, subject);
    strcat(emailHeaders, "\r\n");
    strcat(emailHeaders, "\r\n");
    printf("Generated Email headers = %s",emailHeaders );
    rc = send(sd, msg, strlen(msg), 0);
    if (rc < 0)
    {
        perror("cannot send data ");
        close(sd);
        exit(1);
    }

    printf("Enter your message now.\n");
    while(strcmp(msg,".\r\n") != 0)
    {
        scanf("%s", msg);
        strcat(msg, "\r\n");
        rc = send(sd, msg, strlen(msg), 0);
        if (rc < 0)
        {
            perror("cannot send data ");
            close(sd);
            exit(1);
        }
    }
    printf("Outside while");

    rc = recv(sd, line, MAX_MSG, 0);
    if (rc < 0)
    {
        perror("cannot receive data ");
        close(sd);
        exit(1);
    }
    printf("\nReceived %s:", line);

    strcpy(msg,"QUIT\r\n");
    rc = send(sd, msg, strlen(msg), 0);
    if (rc < 0)
    {
        perror("cannot send data ");
        close(sd);
        exit(1);
    }
    memset(line, 0, MAX_MSG);
    rc = recv(sd, line, MAX_MSG, 0);
    if (rc < 0)
    {
        perror("cannot receive data ");
        close(sd);
        exit(1);
    }
    printf("\nReceived %s:", line);

    return 0;
}
