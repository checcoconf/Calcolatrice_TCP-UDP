#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

/**
 * @file Server.h
 * @brief Header file for a basic server application.
 * @date November 13, 2023
 * @author Francesco Conforti
 */

#define PROTOPORT 56700         // Default Server Port
#define PROTO_ADDR "127.0.0.1"  // Default Server Address
#define BUFFERSIZE 256          // Default Buffer Size

#define MAXOPERANDS 2           // Maximum number of operands

char msg[BUFFERSIZE];    // Message Array
char msgLog[BUFFERSIZE]; // Message Log

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
struct sockaddr_in bindSocket(int my_socket, struct sockaddr_in sad, const char* server_addr, const int port_number);

/**
 * @brief Checks and initializes the Windows Socket API (WSA) for Windows systems.
 * This function is used for cross-platform compatibility.
 */
void checkWindowDevice();

/**
 * @brief Cleans up resources related to the Windows Socket API (WSA).
 * This function is used for cross-platform compatibility.
 */
void clearwinsock();

/**
 * @brief Creates a socket for communication with the server.
 *
 * @param my_socket The socket descriptor to be created.
 * @return The created socket descriptor on success, -1 on failure.
 */
int createSocket(int my_socket);

/**
 * @brief Handles errors by printing the error message to the console.
 *
 * @param errorMessage The error message to display.
 */
void errorhandler(char *errorMessage);

/**
 * @brief Processes the input message, performs calculations, and updates the input string.
 *
 * @param msg The input message containing operator and operands.
 */
void processData(char *msg);

/**
 * @brief Closes the connection for the given socket.
 *
 * @param c_socket The socket to be closed.
 */
void closeConnection(int c_socket);

/**
 * @brief Writes a log message to the log file.
 *
 * @param message The log message to be written.
 */
void writeLog(const char* message);

#endif /* SERVER_SERVER_H_ */