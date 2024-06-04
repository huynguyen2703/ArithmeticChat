// Huy Quoc Nguyen
// CPSC 3500 : Client-Server Arithmetic Chat

//----------------------------------------------LIBRARIES NEEDED--------------------------------------------------------

/*
 * sys/socket.h : to create socket used for client-server connections (socket API)
 * netinet/in.h : used for IPV4 structure (socketaddr_in)
 * sstream : used to parse desired pieces of data in an entire string
 * unistd.h : used to close the sockets
 * csignal : used to handle SIGINT when the server terminates itself
 */
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>
#include <csignal>

// maximum buffer size to store data
#define BUFFER_SIZE 512

using namespace std;
//----------------------------------------METHODS AND DATA DECLARATIONS-------------------------------------------------

/*
 * MAX_INCOMING_REQUEST : intention to store at most 5 clients in the waiting queue if they get connected but not
 * yet accepted by the server.
 * server_address : a struct used to include information about the server. This includes the address family,
 * IP addresses in binary format and port number
 * client_address : a struct used to include information about the client. This includes the address family,
 * IP addresses in binary format and port number (this is required for accept() to accept a client and start serving the
 * client).
 * server_socket_fd : stores server file descriptor returned from socket()
 * client_socket_fd : stores client file descriptor returned from socket()
 * portnum : used to store any port number provided by the user.
 * connected : used to control accept(), if a client is being served by the server, it will be set to true, if it is
 * false, it means another client's turn to be served and accept() will be proceeded with that next client.
 * client_id : used to unique identifies each client connected to the server.
 * bytes_receive : used to store the number of bytes received from the client
 * buff_in : buffer used to store the data/messages the client sends.
 */
const int MAX_INCOMING_REQUEST = 5;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
int server_socket_fd;
int client_socket_fd;
int portnum;
bool connected;
int client_id = 0;
ssize_t bytes_receive;
char buff_in[BUFFER_SIZE + 1];

// use template to help determine type mixing in arithmetic expressions
template<typename T1, typename T2>
auto calculateExpression(T1 num1, T2 num2, char operation) -> typename common_type<T1, T2>::type;

/*
 * signalHandler() is used to handle signal when the server terminates itself using keyboard interrupts (CTRL + C).
 * This will send a SIGINT signal to the operating system, this signalHandler() is then used to handle the signal
 * gracefully and print out termination message. As the connection is terminated then the client socket will be closed
 * and server socket will be closed and program is stopped.
 * Parameter : sigNum (signature of signalHandler), therefore it is noted unused
 * Return : None
 */
void signalHandler([[maybe_unused]] int signum) {
    cout << endl;
    cout << "Server is closing..server closed" << endl;
    close(client_socket_fd);
    close(server_socket_fd);
    exit(0);
}


//-----------------------------------------------EXECUTION SECTION------------------------------------------------------

/*
 * main() will starts the server program, since the server program also is very simple and follows a path with
 * create socket() -> bind() -> listen() -> accept() -> recv() -> send() -> decision was not to decompose it into methods.
 */
int main(int argc, char *argv[]) {
    // signal is put here to catch CTRL + C (case server terminates without processing any client)
    signal(SIGINT, signalHandler);
    // terminates if not correct number of arguments
    if (argc != 2) {
        cout << "Usage server.cpp <port number>" << endl;
        return 1;
    }

    portnum = stoi(argv[1]); // get port number

    // create server socket
    // after successful socket() -> any failed syscall will terminate the program
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // specify server address and port
    server_address.sin_family = AF_INET; // use IPV4
    server_address.sin_addr.s_addr = INADDR_ANY;  // listen for incoming connections on all available network interfaces.
    // get portnum and convert using htons to ensure appropriate representation when transmitted over a network
    server_address.sin_port = htons(portnum);

    // bind the socket to the server address and port
    struct sockaddr *server_addr_arg = (struct sockaddr *) &server_address;
    bind(server_socket_fd, server_addr_arg, sizeof(server_address));

    // listen for incoming connection can queue up to 5 incoming request
    if (listen(server_socket_fd, MAX_INCOMING_REQUEST) == -1) {
        cerr << "Failed to listen" << endl;
        close(server_socket_fd);
        return -1;
    }

    // first line run if no error has occurred during socket(), bind() or listen()
    // -> starts listening
    cout << "Server started" << endl;
    cout << "Listening on port " << portnum << endl;

    // accept one single incoming connection
    struct sockaddr *client_addr_arg = (struct sockaddr *) &client_address;
    while (true) {
        // can accept as long as connected is false
        socklen_t client_addr_len = sizeof(client_address);
        if (!connected) {
            client_socket_fd = accept(server_socket_fd, client_addr_arg, &client_addr_len);
            client_id += 1; // unique id assigned for that new client
            connected = true; // lock status to prevent accept()
            if (client_socket_fd == -1) {
                cerr << "Failed to accept a connection" << endl;
                return -1;
            }
        }

        cout << "Connected with client socket number " << client_id << endl;
        while (connected) {
            // only receive data if client is accepted
            // receive data from client
            char *buffer_pointer = buff_in;
            int buffer_space = BUFFER_SIZE;
            bytes_receive = recv(client_socket_fd, buffer_pointer, buffer_space, 0);
            if (bytes_receive == -1) {
                cerr << "Failed to receive all data" << endl;
                return -1;
            } else if (bytes_receive == 0) {
                // case when client terminates
                // -> 0 byte is sent -> means can accept new client -> unlock status
                connected = false;
                break;
            }

            cout << "Client socket " << client_id << " sent message : " << buff_in << endl;

            // send reply to client
            stringstream message(buff_in); // uses stringstream to parse data
            // first number and second number in the expression (type may not be double but can be cast to correct type)
            double num1, num2;
            char operation;
            message >> num1 >> operation >> num2;  // start reading and storing appropriate content

            // calculate the result, returned type is determined inside method
            auto result = calculateExpression(num1, num2, operation);
            cout << "Sending reply: " << result << endl;

            ostringstream result_string;
            // server will send a whole message of data (uses ostringstream to string representation of the response)
            result_string << "Server replied: " << result;
            const char *reply_arg;
            reply_arg = result_string.str().c_str();
            send(client_socket_fd, reply_arg, result_string.str().length(), 0);

            for (int i = 0; i < BUFFER_SIZE; ++i) {
                buff_in[i] = '\0'; // clear buffer
            }
        }
    }
}

/*
 * calculateExpression() is used to calculate the result of arithmetic expression sent from the client to the server.
 * Since the type can be challenging to determine, using templates to reduce redundancy and still getting correct result.
 * The method will take in type T1 and T2, which is the first and second number in the expression and the operation used
 * to determine operation type. If two number in the expression are both integers, they will be cast to ensure
 * the client receives appropriate results. Otherwise, the type will be double. The result's type will be determined
 * using common type, which is nested type alies in the common type struct template, it is used to calculates the
 * common type between T1 and T2.
 * Parameter : the first number in the expression follows by the second number and the operation
 * Return : result (type is determined after calculation)
 */
template<typename T1, typename T2>
auto calculateExpression(T1 num1, T2 num2, char operation) -> typename common_type<T1, T2>::type {
    using CommonType = typename common_type<T1, T2>::type;
    CommonType result = 0;  // using CommonType to avoid implicit type conversion

    // cast if both are integers, else will be double
    if (num1 == static_cast<int>(num1) && num2 == static_cast<int>(num2)) {
        num1 = static_cast<int>(num1);
        num2 = static_cast<int>(num2);
    }
    switch (operation) {
        case '+': {
            result = num1 + num2;
            break;
        }
        case '-': {
            result = num1 - num2;
            break;
        }
        case '*': {
            result = num1 * num2;
            break;
        }
        case '/': {
            result = num1 / num2;
            if (num2 == 0) {
                result = 0;
            }
            break;
        }
        default: {
            cerr << "Invalid Arithmetic Operation" << endl;
            return -1;
        }
    }
    return result;
}