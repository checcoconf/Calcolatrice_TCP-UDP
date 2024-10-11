/**
 * @file Client.h
 * @brief Header file for a simple client implementation.
 * @date December 12, 2023
 * @author Francesco Conforti
 */

#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#define PROTOPORT "56700"        /**< Default Server Port */
#define PROTO_ADDR "127.0.0.1"    /**< Default Server Address */
#define BUFFERSIZE 256            /**< Default Buffer Size */

char msg[BUFFERSIZE];    /**< Message Array */
char msgLog[BUFFERSIZE]; /**< Message Log */

/**
 * @struct sockaddr_in
 * @brief Structure representing the socket address.
 */

/**
 * @brief Binds the socket to the specified server IP and port number.
 *
 * @param sad The socket address structure.
 * @param server_ip The IP address of the server.
 * @param port_number The port number for the server.
 * @return The updated socket address structure.
 */
struct sockaddr_in bindSocket(struct sockaddr_in sad, char* server_ip, int port_number);

/**
 * @brief Initializes the WSA library if on a Windows platform.
 */
void checkWindowDevice();

/**
 * @brief Cleans up Windows socket resources if on a Windows platform.
 */
void clearwinsock();

/**
 * @brief Closes the connection for the given socket.
 *
 * @param c_socket The socket to be closed.
 */
void closeConnection(int c_socket);

/**
 * @brief Creates a socket and returns the socket descriptor.
 *
 * @param c_socket The socket descriptor.
 * @return The created socket descriptor.
 */
int createSocket(int c_socket);

/**
 * @brief Displays an error message to the console.
 *
 * @param errorMessage The error message to display.
 */
void errorhandler(char *errorMessage);

/**
 * @brief Reads and validates user input as a command string to send to the server.
 * If the input is invalid, it prompts the user to enter a valid input.
 *
 * @param msg The command string entered by the user.
 */
void inputString(char *msg);

/**
 * @brief Receives data from the server.
 *
 * @param sock The socket descriptor.
 * @param msg The message buffer to store the received data.
 * @param fromAddr The server's socket address structure.
 * @param echoServAddr The client's socket address structure.
 * @return The number of bytes received.
 */
int receiveData(int sock, char *msg, struct sockaddr_in *fromAddr, struct sockaddr_in *echoServAddr);

/**
 * @brief Sends data to the server.
 *
 * @param c_socket The socket descriptor.
 * @param msg The message to be sent.
 * @param echoServAddr The server's socket address structure.
 * @return The number of bytes sent.
 */
int sendData(int c_socket, char *msg, struct sockaddr_in *echoServAddr);

/**
 * @brief Writes a log message to the log file.
 *
 * @param message The log message to be written.
 */
void writeLog(const char* message);

/**
 * @brief Display a welcome message to the console.
 *
 * This function prints a welcome message to the console, providing information
 * about the program, the author, and the supported operations.
 */
void writeWelcomeMsg(void);

#endif /* CLIENT_CLIENT_H_ */
