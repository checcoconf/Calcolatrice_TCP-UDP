#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

/**
 * @file Server.h
 * @brief Header file for a basic server application.
 * @date November 13, 2023
 * @author Francesco Conforti
 */

#define PROTOPORT 53199         // Default Server Port
#define PROTO_ADDR "127.0.0.1"  // Default Server Address
#define BUFFERSIZE 512          // Default Buffer Size

// Define the maximum queue size for pending client connections
#define QUEUE 5

#define MAXOPERANDS 2           // Maximum number of operands

char msg[BUFFERSIZE];    // Message Array
char msgLog[BUFFERSIZE]; // Message Log

/**
 * @brief Binds the socket to a specific address and port.
 *
 * @param my_socket The socket descriptor to bind.
 * @param sad A sockaddr_in structure containing address and port information.
 * @return The sockaddr_in structure with updated information after binding.
 */
struct sockaddr_in bindSocket(int my_socket, struct sockaddr_in sad);

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
 * @brief Sends a welcome message to the client upon connection.
 *
 * @param client_socket The socket descriptor for the connected client.
 */
void sendWelcomeMsg(int client_socket);

/**
 * @brief Sets the socket to listen mode to accept incoming connections.
 *
 * @param my_socket The socket descriptor to set on listen.
 */
void setSocketOnListen(int my_socket);

/**
 * @brief Writes a log message to the log file.
 *
 * @param message The log message to be written.
 */
void writeLog(const char* message);

#endif /* SERVER_SERVER_H_ */
