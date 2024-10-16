#include "Headers.h"
#include "Server.h"
#include "Calculator.h"

/**
 * @file Server.c
 * @brief Implementation file for a basic server application.
 * @date November 13, 2023
 * @author Francesco Conforti
 */

/**
 * @brief Main function for the server application.
 *
 * This function initializes the WSA library (if on Windows), creates a socket,
 * binds the socket, sets the socket to listen mode, and accepts incoming connections.
 * It then communicates with the connected clients, processing their requests until
 * the client sends "=" to close the connection.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return 0 upon successful execution.
 */
int main(int argc, char *argv[]) {
    printf("Look at the log file!");
    // 0) Initialize the WSA library in case we are on Windows
    checkWindowDevice();

    // 1) Create a socket
    int my_socket = -1;
    my_socket = createSocket(my_socket);

    // 2) Bind the socket
    struct sockaddr_in sad = bindSocket(my_socket, sad);

    // 3) Set the socket to listen mode
    setSocketOnListen(my_socket);

    struct sockaddr_in cad;    // Structure for the client's address
    int client_socket;     // Socket descriptor for the client
    int client_len;     // Size of the client's address


    while (1) {
        sprintf(msgLog,"Searching for a client...");
        writeLog(msgLog);
        client_len = sizeof(cad); // Set the client's size

        // 4) Accept a connection
        if ((client_socket = accept(my_socket, (struct sockaddr*) &cad,&client_len)) < 0) {
            errorhandler("accept() failed.");
            closesocket(client_socket);
            clearwinsock();
            return -1;
        }

        // 5) Server sends a connection string

        sprintf(msgLog,"Connection established with %s:%d", inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
        writeLog(msgLog);

        // Send Welcome Message
        sendWelcomeMsg(client_socket);

        // Receive and process data from the client until the client sends "="
        while (1) {
            int bytes_received = recv(client_socket, msg,sizeof(char) * BUFFERSIZE, 0);

            if (bytes_received <= 0) {
                if (bytes_received == 0) {
                    sprintf(msgLog,"Client has closed the connection.");
                    writeLog(msgLog);
                } else {
                    errorhandler("recv() failed or connection closed prematurely");
                }
                closesocket(client_socket);
                clearwinsock();
                break; // Exit the loop
            }

            snprintf(msgLog, sizeof(msgLog),"Client: %s:%d send: %s", inet_ntoa(cad.sin_addr), ntohs(cad.sin_port), msg);
            writeLog(msgLog);

            // Process data according to the logic defined in the function
            processData(msg);

            snprintf(msgLog, sizeof(msgLog),"Server: %s:%d said: %s", inet_ntoa(cad.sin_addr), ntohs(cad.sin_port), msg);
            writeLog(msgLog);

            // Send processed data back to the client
            if (send(client_socket, msg, sizeof(char) * BUFFERSIZE,0) != sizeof(char) * BUFFERSIZE) {
                errorhandler("send() sent a different number of bytes than expected");
                closesocket(client_socket);
                clearwinsock();
                break; // Exit the loop
            }

            char *byeString = "Bye";
            if (strcmp(msg, byeString) == 0) {
                break; // Exit the loop if the server sends "Bye"
            }
        }
        sprintf(msgLog,"Closing connection with %s:%d", inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
        // Close the client socket and wait for the next connection
        closesocket(client_socket);
        writeLog(msgLog);
    }
}

/**
 * @brief Sends a welcome message to the client upon connection.
 *
 * @param client_socket The socket descriptor for the connected client.
 */
void sendWelcomeMsg(int client_socket) {
    char welcomeString[] =
            "\n* * * * * * * * * * * * * * * * * * * * * * * *\n"
            "*   Francesco Conforti - Matricola: 776628    *\n"
            "*             Basic Calculator                *\n"
            "*      Supported operations: +, -, *, /       *\n"
            "*      Enter = to close the connection        *\n"
            "* * * * * * * * * * * * * * * * * * * * * * * *";
    send(client_socket, welcomeString, sizeof(char) * BUFFERSIZE, 0);
}

/**
 * @brief Checks and initializes the Windows Socket API (WSA) for Windows systems.
 * This function is used for cross-platform compatibility.
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
 * @brief Creates a socket for communication with the server.
 *
 * @param my_socket The socket descriptor to be created.
 * @return The created socket descriptor on success, -1 on failure.
 */
int createSocket(int my_socket) {
    my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (my_socket < 0) {
        errorhandler("Socket creation failed.");
        closesocket(my_socket);
        clearwinsock();
        return -1;
    } else {
        sprintf(msgLog,"Socket created successfully!");
        writeLog(msgLog);
    }
    return my_socket;
}

/**
 * @brief Binds the socket to a specific address and port.
 *
 * @param my_socket The socket descriptor to bind.
 * @param sad A sockaddr_in structure containing address and port information.
 * @return The sockaddr_in structure with updated information after binding.
 */
struct sockaddr_in bindSocket(int my_socket, struct sockaddr_in sad) {
    // Assign an address to the newly created socket
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr(PROTO_ADDR);
    sad.sin_port = htons(PROTOPORT);

    if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
        errorhandler("bind() failed.");
        closesocket(my_socket);
        clearwinsock();
    }

    return sad;
}

/**
 * @brief Sets the socket to listen mode to accept incoming connections.
 *
 * @param my_socket The socket descriptor to set on listen.
 */
void setSocketOnListen(int my_socket) {
    if (listen(my_socket, QUEUE < 0)) {
        errorhandler("listen() failed.");
        closesocket(my_socket);
        clearwinsock();
        return;
    }
}

/**
 * @brief Cleans up resources related to the Windows Socket API (WSA).
 * This function is used for cross-platform compatibility.
 */
void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

/**
 * @brief Handles errors by printing the error message to the console.
 *
 * @param errorMessage The error message to display.
 */
void errorhandler(char *errorMessage) {
    printf("\n%s", errorMessage);
    writeLog(errorMessage);
}

/**
 * @brief Processes the input message, performs calculations, and updates the input string.
 *
 * @param msg The input message containing operator and operands.
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
    int operands[MAXOPERANDS];

    // Tokenize the input string and collect operands
    char *token = strtok(msg + 2, " ");
    while (token != NULL && numOperands < MAXOPERANDS) {
        if (sscanf(token, "%d", &operands[numOperands]) != 1) {
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
    snprintf(msg, BUFFERSIZE, "%.2f", result);
}

/**
 * @brief Writes a log message to the log file.
 *
 * @param message The log message to be written.
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
