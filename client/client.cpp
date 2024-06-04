// Huy Quoc Nguyen
// CPSC 3500 : Client-Server Arithmetic Chat

//------------------------------------------------LIBRARIES NEEDED------------------------------------------------------

/*
 * sys/socket.h : to create socket used for client-server connections (socket API)
 * arpa/inet.h : used to specify internet protocols (IPV4)
 * unistd.h : used to close the sockets
 * cstring : to use string methods and functionalities when processing and sending results between client-server
 * csignal : used to handle SIGINT when client terminates a connection
 * regex : used to create regular expression that can surely matches the expression format and its variations
 */
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <regex>

// maximum buffer size to store data
#define BUFFER_SIZE 512

using namespace std;

//-----------------------------------------------DATA DECLARATIONS------------------------------------------------------

/*
 * server_address : a struct used to include information about the server. This includes the address family,
 * IP addresses in binary format and port number
 * client_socket_fd : stores client file descriptor returned from socket()
 * portnum : port number used to connect to a server
 */
struct sockaddr_in server_address;
int client_socket_fd;
const char *ip_address;
int portnum;

//-----------------------------------------------EXECUTION SECTION------------------------------------------------------

/*
 * signalHandler() is used to handle signal when a client terminates the connection between the client program
 * and the server using keyboard interrupts (CTRL + C). This will send a SIGINT signal to the operating system, this
 * signalHandler() is then used to handle the signal gracefully and print out termination message. As the connection is
 * terminated then the client socket will be closed and program is stopped.
 * Parameter : sigNum (signature of signalHandler), therefore it is noted unused
 * Return : None
 */
void signalHandler([[maybe_unused]] int sigNum) {
    cout << endl;
    cout << "Client terminates connection" << endl;
    close(client_socket_fd);
    exit(0);
}

/*
 * main() will starts the client program, since the client program is very simple and follows a path with
 * create socket() -> connect() -> send() -> receive() -> decision was not to decompose it into methods.
 */
int main(int argc, char *argv[]) {
    // terminates if not correct number of arguments
    if (argc != 3) {
        cout << "Usage client.cpp <client ip address> <port number>" << endl;
        return 1;
    }
    ip_address = argv[1];  // get IP address
    portnum = stoi(argv[2]);  // get port number

    // create a socket
    // after successful socket() -> any failed request/syscall will terminate the program
    client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_fd == -1) {  // failed on -1
        cerr << "Failed to create client socket" << endl;
        return -1;
    }

    // specify server address and port number
    server_address.sin_family = AF_INET; // use IPV4
    // get portnum and convert using htons to ensure appropriate representation when transmitted over a network
    server_address.sin_port = htons(portnum);

    if (inet_pton(AF_INET, ip_address, &server_address.sin_addr.s_addr) <= 0) {
        cerr << "Invalid Address/Not Supported" << endl;
        close(client_socket_fd);
        return -1;
    }

    // get expression from command line arg using regular expression to match pattern and variations
    string expression;
    regex expressionPattern(R"(\s*([-+]?[1-9]\d*((\.\d+)?)|0)\s*([\+\-\*\/])\s*([-+]?[1-9]\d*(\.\d+)?|0)\s*)");

    // connect to the server
    if (connect(client_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        cerr << "Failed to connect to server" << endl;
        close(client_socket_fd);
        return -1;
    }
    // if connected successfully print message for user
    cout << "Connected to server" << endl;

    // keep prompting client for input
    while (true) {
        cout << "Please enter the message to the server : ";
        // signal is put here to catch CTRL + C (case client terminates without any input)
        signal(SIGINT, signalHandler);
        // get the expression from user input
        getline(cin, expression);
        if (!regex_match(expression, expressionPattern)) {
            cerr << "invalid expression" << endl;
            continue; // if invalid -> go back to loop and re-prompt -> invalid expression will not go to server
        }
        cout << endl;
        if (expression.empty()) {  // same as above -> re-prompt
            cout << endl;
            continue;
        }
        // convert to const char* to use with send()
        const char *converted_expression = expression.c_str();


        // send data to server
        ssize_t bytes_send = send(client_socket_fd, converted_expression, strlen(converted_expression), 0);
        if (bytes_send == -1) { // success return the number of bytes send, else -1
            cerr << "Data is not sent thoroughly" << endl;
            close(client_socket_fd);
            return -1;
        }

        // receive data from the server and print
        char buff[BUFFER_SIZE] = {0}; // uses a buffer size 512 to store data sent from the server
        // recv() uses to receive the bytes from server
        ssize_t message_read = recv(client_socket_fd, buff, BUFFER_SIZE, 0);
        if (message_read == -1) {
            close(client_socket_fd);
            cerr << "Failed to read message from server" << endl;
            return -1;
        }
        cout << buff << endl; // print the content received from server
    }
}