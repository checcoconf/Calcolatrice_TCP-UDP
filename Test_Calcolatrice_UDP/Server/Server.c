#include "Headers.h"
#include "Server.h"
#include "Calculator.h"

/**
 * @file Server.c
 * @brief Implementation file for a basic server application.
 * @date December 12, 2023
 * @author Francesco Conforti
 */

/**
 * @brief Main function for the server application.
 *
 * This function initializes the Windows Sockets API (WSA) on Windows platforms,
 * creates a socket, binds it to the specified address and port, and then enters
 * a loop to receive and process data from clients. It processes each client request,
 * logs the operations, and sends back the processed data to the client.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return The exit status of the program.
 */
int main(int argc, char *argv[]) {
    printf("Look at the log file!\n\n");
    // 0) Initialize the WSA library in case we are on Windows
    checkWindowDevice();

    // 1) Create a socket
    int my_socket = -1;
    my_socket = createSocket(my_socket);
    sprintf(msgLog,"Server socket created successfully!");
    writeLog(msgLog);

    // 2) Bind the socket
    struct sockaddr_in sad = bindSocket(my_socket, sad, PROTO_ADDR, PROTOPORT);
    sprintf(msgLog,"Server socket binded successfully!");
    writeLog(msgLog);
    struct sockaddr_in cad;	// Structure for the client's address

    sprintf(msgLog,"Searching for a client...");
    writeLog(msgLog);

    while(1) {
        // Receive and process data from the client until the client sends "="
        socklen_t client_len = sizeof(cad); // Set the client's size
        memset(msg, 0, BUFFERSIZE); // clean msg
        // 3) receive data
        int bytes_received = recvfrom(my_socket, msg, BUFFERSIZE, 0, (struct sockaddr*) &cad, &client_len);

        // check the received data length
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                sprintf(msgLog,"Client has closed the connection.");
                writeLog(msgLog);
            } else {
                errorhandler("recvfrom() failed or connection closed prematurely");
            }
        } else {
            // 4) Convert the address to the associated DNS
            struct hostent *he;
            he = gethostbyaddr((char *)&(cad.sin_addr), sizeof(struct in_addr), AF_INET);
            if (he == NULL) {
                errorhandler("gethostbyaddr() failed");
                closesocket(my_socket);
                clearwinsock();
                return -1;
            }

            sprintf(msgLog,"Request operation '%s' from client %s, IP %s", msg, he->h_name, inet_ntoa(cad.sin_addr));
            writeLog(msgLog);
            printf("%s\n",msgLog);

            // 5) Process data according to the logic defined in the function
            processData(msg);

            // 6) Send processed data back to the client
            if (sendto(my_socket, msg, sizeof(msg), 0, (struct sockaddr*) &cad, client_len) != sizeof(msg)) {
                errorhandler("sendto() sent a different number of bytes than expected");
                closesocket(my_socket);
                clearwinsock();
                return 0;
            }
        }
    }
}

/**
 * @brief Initializes the Windows Sockets API (WSA) if on a Windows platform.
 *
 * This function is specific to Windows platforms. It initializes the WSA library,
 * enabling the use of sockets on Windows. If the initialization fails, it prints
 * an error message using the errorhandler function.
 */
void checkWindowDevice() {
#if defined WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        errorhandler("Error during WSAStartup");
        return;
    }
#endif
}

/**
 * @brief Closes the connection for the given socket and performs system-specific actions.
 *
 * This function is responsible for closing the socket connection and executing
 * system-specific commands based on the platform. On Windows, it clears the console
 * screen and waits for a key press. On Unix-like systems, it prints a message and
 * waits for a key press. After system-specific actions, it closes the socket and
 * cleans up Windows socket resources.
 *
 * @param c_socket The socket to be closed.
 */
void closeConnection(int c_socket) {
#ifdef _WIN32
    // Windows specific command
    system("cls");
    system("pause");
#else
    // Unix-like systems command
    system("clear")
    printf("Press any key to close the process...");
    getchar();
#endif
    closesocket(c_socket);
    clearwinsock();
}

/**
 * @brief Creates a UDP socket.
 *
 * This function creates a UDP socket using the specified protocol family,
 * socket type, and protocol. If the socket creation fails, an error message
 * is displayed, and the necessary cleanup is performed before returning -1.
 *
 * @param my_socket A socket descriptor, which will be updated upon success.
 * @return If successful, returns the updated socket descriptor; otherwise, returns -1.
 */
int createSocket(int my_socket) {
    if ((my_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        errorhandler("socket() failed.");
        clearwinsock();
        return -1;
    }
    return my_socket;
}

/**
 * @brief Binds a socket to a specified address and port.
 *
 * This function binds the specified socket to the given IP address and port number.
 * If the binding fails, an error message is displayed, and the socket is closed,
 * followed by necessary cleanup procedures.
 *
 * @param my_socket The socket descriptor to be bound.
 * @param sad The sockaddr_in structure to be filled with address and port information.
 * @param server_addr The IP address to bind the socket to.
 * @param port_number The port number to bind the socket to.
 * @return The sockaddr_in structure containing the bound address and port information.
 */
struct sockaddr_in bindSocket(int my_socket, struct sockaddr_in sad, const char* server_addr, const int port_number) {
    // Assign an address to the newly created socket
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr(server_addr);
    sad.sin_port = htons(port_number);

    if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
        errorhandler("bind() failed.");
        closesocket(my_socket);
        clearwinsock();
    }

    return sad;
}

/**
 * @brief Cleanup the Windows Socket API (WSA) resources on Windows systems.
 *
 * This function cleans up the Windows Socket API (WSA) resources on Windows systems,
 * if applicable. It is designed to be used after socket operations on Windows to release
 * resources acquired by the WSAStartup function.
 *
 * @note This function is specific to Windows systems and should be called after socket
 *       operations are completed.
 */
void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

/**
 * @brief Handles and reports errors by printing an error message to the console and writing it to the log.
 *
 * This function takes an error message as input, prints it to the console with a newline character,
 * and writes the error message to the log file using the writeLog() function.
 *
 * @param errorMessage The error message to be handled.
 */
void errorhandler(char *errorMessage) {
    printf("\n%s", errorMessage);
    writeLog(errorMessage);
}

/**
 * @brief Process the input string containing operator and operands.
 *
 * This function extracts the operator and operands from the input string,
 * performs the corresponding arithmetic operation, and updates the input
 * string with the result. If there are errors in the input format or if
 * an arithmetic operation encounters an error (e.g., division by zero),
 * appropriate error messages are returned.
 *
 * @param msg The input string containing the operator and operands.
 *            On success, it is updated with the result of the operation.
 */
void processData(char *msg) {
    // Extract the operator and operands from the input string
    char operator = msg[0];

    // Check if the operator is '=' to terminate communication
    if (operator == '=') {
        char *byeString = "Bye";
        snprintf(msg, strlen(byeString) + 1, "%s", byeString);
        return;
    }

    int numOperands = 0;
    double operands[MAXOPERANDS];

    // Tokenize the input string and collect operands
    char *token = strtok(msg + 2, " ");
    while (token != NULL && numOperands < MAXOPERANDS) {
        if (sscanf(token, "%lf", &operands[numOperands]) != 1) {
            // Error handling: Invalid operand format
            snprintf(msg, strlen(msg), "Invalid operand format: %s", token);
            return; // Or handle the error as needed
        }
        numOperands++;
        token = strtok(NULL, " ");
    }

    if (numOperands < 2) {
        char *insufficientNumberError = "Insufficient number of operands";
        writeLog(insufficientNumberError);
        strcpy(msg, insufficientNumberError);
        return;
    }

    double result = operands[0];
    for (int i = 1; i < numOperands; i++) {
        switch (operator) {
            case '+':
                result = add(result, operands[i]);
                break;
            case '-':
                result = sub(result, operands[i]);
                break;
            case '*':
                result = mult(result, operands[i]);
                break;
            case '/':
                if (operands[i] != 0) {
                    result = division(result, operands[i]);
                } else {
                    // Error handling: Division by zero
                    char *divisionError = "|Error| -  Division by Zero";
                    strcpy(msg, divisionError);
                    return;
                }
                break;
            default:
                // Error handling: Unknown operator
                snprintf(msg, BUFFERSIZE, "Unknown operator: %c", operator);
                return;
        }
    }

    // Convert the result to a string and update the input string
    sprintf(msg, "%.2f %c %.2f = %.2f", operands[0], operator, operands[1], result);
}

/**
 * @brief Write a log message to a file.
 *
 * This function opens a log file ("Log.txt") and appends a log message
 * along with a timestamp to the file. If the file cannot be opened,
 * an error message is printed to the console.
 *
 * @param message The log message to be written to the file.
 */
void writeLog(const char* message) {
    FILE* file = fopen("Log.txt", "a");

    if (file != NULL) {
        time_t timestamp = time(NULL);
        struct tm* timeInfo = localtime(&timestamp);

        // Get current date and time
        char dateAndTime[20];
        strftime(dateAndTime, sizeof(dateAndTime), "%H:%M:%S %d/%m/%Y", timeInfo);

        // Write the log message to the file
        fprintf(file, "SERVER - [%s] - %s\n", dateAndTime, message);

        fclose(file);
    } else {
        printf("Error opening the log file.\n");
    }
}
