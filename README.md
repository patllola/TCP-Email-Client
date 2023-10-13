# TCP-Email-Client
This program is a basic TCP client that interacts with an SMTP server to send an email.
The program uses the basic commands of SMTP (Simple Mail Transfer Protocol) to initiate a connection, set the sender and receiver addresses, set the subject, and send the email content.

## Prerequisites
GCC compiler for compiling the c-program.

## Compilation:
To compile the program, use the following command:
gcc -o tcpClient tcpClient.c


## After starting, the program will follow the below steps:

1) Connect to the specified SMTP server.
2) Exchange initial greetings (HELO).
3) Prompt the user to input the sender's email address.
4) Prompt the user to input the recipient's email address.
5) Prompt the user to input the email subject.
6) Allow the user to type the body of the email. The user can finish the email input by typing a line containing only a dot (.).
7) Terminate the connection with the SMTP server.

# Notes:
The program connects to the SMTP server on the default port 25.
The client will send a HELO command to the SMTP server.
The program uses basic error handling for socket operations.
Currently, the program does not handle multi-line email bodies very well, as it reads input line by line. For better handling of multi-line inputs, some modifications will be needed.
Always ensure that the SMTP server you are connecting to allows and accepts requests from your client.
Warning
Sending emails using this program requires knowledge about the SMTP server you are connecting to. Make sure you have permission to send emails via the server, or you might get blacklisted.

Feel free to modify the README and ping me if there are any changes.






