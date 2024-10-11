#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

/**
 * @file Client.h
 * @brief Header file for a simple client implementation.
 * @date November 13, 2023
 * @author Francesco Conforti
 */

#define PROTOPORT 53199         // Default Server Port
#define PROTO_ADDR "127.0.0.1"  // Default Server Address
#define BUFFERSIZE 512          // Default Buffer Size

char msg[BUFFERSIZE];    // Message Array
char msgLog[BUFFERSIZE]; // Message Log

/**
 * @brief Binds the socket to the specified address and port.
 *
 * @param sad The sockaddr_in structure containing address and port information.
 * @return The sockaddr_in structure after binding.
 */
struct sockaddr_in bindSocket(struct sockaddr_in sad);

/**
 * @brief Initializes the WSA library if on a Windows platform.
 */
void checkWindowDevice();

/**
 * @brief Cleans up Windows socket resources if on a Windows platform.
 */
void clearwinsock();

/**
 * @brief Closes the connection and performs cleanup.
 *
 * @param c_socket The socket to close.
 */
void closeConnection(int c_socket);

/**
 * @brief Connects to the server using the provided socket and address.
 *
 * @param c_socket The socket to connect.
 * @param sad The sockaddr_in structure containing server address information.
 * @return 1 if the connection is successful, -1 if there is an error.
 */
int connectToServer(int c_socket, struct sockaddr_in sad);

/**
 * @brief Creates a TCP socket and initializes it.
 *
 * @param c_socket The socket to be created.
 * @return The created socket if successful, -1 if there is an error.
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
 * @brief Receives data from the server through the given socket and stores it in msg.
 *
 * @param c_socket The socket for receiving data.
 * @param string_len The length of the string to receive.
 * @param msg The received message will be stored in this buffer.
 * @return 0 if data is received successfully, -1 if there is an error.
 */
int receiveData(int c_socket, int string_len, char *msg);

/**
 * @brief Sends data to the server through the given socket.
 *
 * @param c_socket The socket for sending data.
 * @param msg The message to be sent.
 * @return 1 if the send operation is successful, -1 if there is an error.
 */
int sendData(int c_socket, char *msg);

/**
 * @brief Writes a log message to the log file.
 *
 * @param message The log message to be written.
 */
void writeLog(const char* message);


#endif /* CLIENT_CLIENT_H_ */
