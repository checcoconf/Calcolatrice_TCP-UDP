#include "Headers.h"
#include "Client.h"

/**
 * @file Client.c
 * @brief Implementation file for a simple client.
 * @date December 12, 2023
 * @author Francesco Conforti
 */

/**
 * @brief Main function for the UDP client program.
 *
 * This function represents the main entry point for the UDP client program.
 * It performs the following steps:
 *
 *   1. Writes a welcome message to the console.
 *   2. Initializes the Windows Socket API on Windows systems.
 *   3. Creates a UDP socket for communication with the server.
 *   4. Sets the server address and port based on command-line arguments or default values.
 *   5. Translates the server name into its IP address.
 *   6. Binds the socket to the specified server address and port.
 *   7. Enters a loop to send and receive data to/from the server.
 *   8. Closes the socket connection and performs cleanup after exiting the loop.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return Returns 0 on successful execution, otherwise returns an error code.
 */
int main(int argc, char *argv[]) {
    writeWelcomeMsg();

    // 0) Initialize the WSA library in case we are on Windows
    checkWindowDevice();

    // 1) Create a socket
    int c_socket = -1;
    c_socket = createSocket(c_socket);
    sprintf(msgLog,"Server socket created successfully!");
    printf("%s\n",msgLog);
    writeLog(msgLog);

    // 2) Sets address and port converting from name to address if name is passed as boot parameter
    char *host_input[2];
    struct hostent *host;
    struct in_addr *ina;
    unsigned portC;
    char *token;

    if(argc > 1){
        token = strtok(argv[1], ":");
        host_input[0] = token;
        host_input[1] = strtok(NULL, "\0");
    } else {
        host_input[0] = PROTO_ADDR; // default address (localhost)
        host_input[1] = PROTOPORT;  // default port
        sprintf(msgLog,"Address and port not entered, standard value applied!");
        printf("%s\n",msgLog);
        writeLog(msgLog);
    }

    // translate name into address
    host = gethostbyname((char*) host_input[0]);
    if(host){
        ina = (struct in_addr*) host->h_addr_list[0];
        portC = strtol(host_input[1],NULL,10);
        sprintf(msgLog,"Address resolved: %s:%d", inet_ntoa(*ina), (unsigned) portC);
        printf("%s\n",msgLog);
        writeLog(msgLog);
    }else{
        sprintf(msgLog,"Address not resolved.");
        printf("%s\n",msgLog);
        writeLog(msgLog);
        closeConnection(c_socket);
    }

    // 3) Bind the socket
    struct sockaddr_in echoServAddr = bindSocket(echoServAddr, inet_ntoa(*ina), (int) portC);
    struct sockaddr_in fromAddr;
    sprintf(msgLog,"Server socket binded successfully!");
    printf("%s\n",msgLog);
    writeLog(msgLog);

    // Client loop operations
    while (1) {
        // Request data from the console
        inputString(msg);

        // 4) send data to server
        sendData(c_socket, msg, &echoServAddr);

        // 5) Receive data from the server
        receiveData(c_socket, msg, &fromAddr, &echoServAddr);

        // Check if the received message is "Bye"
        char *byeString = "Bye";
        if (strcmp(msg, byeString) == 0) {
            break; // Exit the loop if the server sends "Bye"
        }
    }

    // 6) Close the connection
    closeConnection(c_socket);

    return 0;
}

/**
 * @brief Display an error message to the console.
 *
 * This function prints an error message to the console.
 *
 * @param error_message The error message to be displayed.
 */
void errorhandler(char *error_message) {
    printf("%s", error_message);
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
 * @brief Send data to a UDP server using the specified socket and server address.
 *
 * This function sends data to a UDP server using the provided socket descriptor
 * and server address. It verifies that the number of bytes sent matches the expected
 * length of the message. If there are errors during the process, an appropriate
 * error message is displayed, and the function returns -1.
 *
 * @param c_socket The UDP socket descriptor.
 * @param msg The message to be sent to the server.
 * @param echoServAddr The sockaddr_in structure representing the server address.
 * @return If successful, returns 1; otherwise, returns -1.
 */
int sendData(int c_socket, char *msg, struct sockaddr_in *echoServAddr) {
    // Send data to the server
    int echoStringLen = strlen(msg);
    if (sendto(c_socket, msg, echoStringLen, 0, (struct sockaddr*) echoServAddr, sizeof(*echoServAddr)) != echoStringLen) {
        errorhandler("sendto() sent a different number of bytes than expected.");
        return -1;
    }
    return 1;
}

/**
 * @brief Receive data from a UDP socket and validate the source address.
 *
 * This function receives data from a UDP socket, validates the source address,
 * and logs the received information. If the source address is unknown or if
 * there are errors during the process, appropriate error messages are displayed,
 * and the program may exit.
 *
 * @param sock The UDP socket descriptor.
 * @param msg The buffer to store the received data.
 * @param fromAddr The sockaddr_in structure for the source address.
 * @param echoServAddr The sockaddr_in structure for the expected server address.
 * @return If successful, returns 0; otherwise, returns -1.
 */
int receiveData(int sock, char *msg, struct sockaddr_in *fromAddr, struct sockaddr_in *echoServAddr) {
    unsigned int fromSize = sizeof(*fromAddr);
    int respStringLen = recvfrom(sock, msg, BUFFERSIZE, 0, (struct sockaddr*) fromAddr, &fromSize);

    if (echoServAddr->sin_addr.s_addr != fromAddr->sin_addr.s_addr){
        errorhandler("Error: received a packet from unknown source.");
        exit(EXIT_FAILURE);
    }
    msg[respStringLen] = '\0';

    // Convert the address to the associated DNS
    struct hostent *he;
    he = gethostbyaddr((char *)&fromAddr->sin_addr, sizeof(struct in_addr), AF_INET);
    if (he == NULL) {
        errorhandler("gethostbyaddr() failed");
        closesocket(sock);
        clearwinsock();
        return -1;
    }

    sprintf(msgLog,"Received result from server %s, ip %s: %s", he->h_name, inet_ntoa(fromAddr->sin_addr), msg);
    printf("%s\n",msgLog);
    writeLog(msgLog);
    return 0;
}

/**
 * @brief Close the socket connection and perform system-specific cleanup.
 *
 * This function closes the socket connection and performs system-specific cleanup.
 * On Windows, it clears the console screen and pauses the process. On Unix-like
 * systems, it clears the console screen, prints a message, and waits for user input.
 *
 * @param c_socket The socket descriptor to be closed.
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
 * @brief Create a UDP socket for communication.
 *
 * This function creates a UDP socket using the specified protocol family,
 * socket type, and protocol. If the socket creation fails, an error message
 * is displayed, and the necessary cleanup is performed before returning -1.
 *
 * @param c_socket The socket descriptor to be updated upon success.
 * @return If successful, returns the updated socket descriptor; otherwise, returns -1.
 */
int createSocket(int c_socket) {
    // Creation of a TCP socket
    if ((c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        errorhandler("Socket creation failed.");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    }
    return c_socket;
}

/**
 * @brief Configure and initialize a sockaddr_in structure for binding a socket.
 *
 * This function configures and initializes a sockaddr_in structure with the provided
 * server IP address and port number for binding a socket. The resulting structure
 * can be used as an argument for the bind() function to associate the socket with
 * a specific IP address and port.
 *
 * @param sad The sockaddr_in structure to be configured and initialized.
 * @param server_ip The IP address to bind the socket to.
 * @param port_number The port number to bind the socket to.
 * @return The configured sockaddr_in structure.
 */
struct sockaddr_in bindSocket(struct sockaddr_in sad, char* server_ip, int port_number) {
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;  // Set IPv4 socket
    sad.sin_addr.s_addr = inet_addr(server_ip); // Server's IP
    sad.sin_port = htons(port_number); // Server's port
    return sad;
}

/**
 * @brief Initialize the Windows Socket API (WSA) on Windows systems.
 *
 * This function checks if the program is running on a Windows system and,
 * if so, initializes the Windows Socket API (WSA). It uses the WSAStartup
 * function to set up the necessary resources for socket programming on Windows.
 *
 * @note This function is specific to Windows systems and should be called
 *       before any socket-related operations.
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
 * @brief Read a command string from the user.
 *
 * This function prompts the user to enter a command string to send to the server.
 * It uses fgets to read the entire line, checks for input length limits, and validates
 * the input string format. If the input is invalid or too long, appropriate messages
 * are printed to the console and logged. If the user inputs an "=" character, the
 * function prepares to close the connection.
 *
 * @param msg The buffer to store the input string.
 */
void inputString(char *msg) {
    memset(msg, 0, BUFFERSIZE);

    printf("\nEnter the commands to send to the server: ");

    // Use fgets to read an entire line, including spaces
    if (fgets(msg, BUFFERSIZE, stdin) != NULL) {
        // Check if the input exceeds BUFFERSIZE
        if (!strchr(msg, '\n')) {
            // Input exceeds BUFFERSIZE, clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            sprintf(msgLog,"Input too long. Please enter commands less than %d characters.", BUFFERSIZE);
            printf("%s\n",msgLog);
            writeLog(msgLog);
        } else {
            // Remove the trailing newline character if present
            size_t length = strlen(msg);
            if (length > 0 && msg[length - 1] == '\n') {
                msg[length - 1] = '\0';
            }
        }
    } else {
        // fgets failed, handle the error
        sprintf(msgLog,"Error reading input.");
        printf("%s\n",msgLog);
        writeLog(msgLog);
    }

    // Check if the input string length is within limits
    if (strlen(msg) > (BUFFERSIZE - 1)) {
        sprintf(msgLog,"Input string is too long, a string truncated according to the limits will be sent to the server.");
        printf("%s\n",msgLog);
        writeLog(msgLog);
        msg[BUFFERSIZE - 1] = '\0'; // Truncate the string if it's too long
    }

    // Check if the input string format is valid (operator value value)
    // Regex for all devices
    char operator;
    double num1, num2;
    if (sscanf(msg, "%c", &operator) == 1 && strchr("=", operator) != NULL) {
        sprintf(msgLog,"Closing the connection...");
        printf("%s\n",msgLog);
        writeLog(msgLog);
    } else {
        if (sscanf(msg, "%c %lf %lf", &operator, &num1, &num2) != 3|| strchr("+-*/", operator) == NULL) {
            sprintf(msgLog,"Invalid input format. Please use the format: operator [+-*/] value value or = to close the connection");
            printf("%s\n",msgLog);
            writeLog(msgLog);
            inputString(msg); // Prompt the user to enter a valid input
        }
    }
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
void writeLog(const char *message) {
    char logFilePath[256];
    FILE *file;

    // Get the path of the running program
    char basePath[256];

#ifdef _WIN32
    GetModuleFileName(NULL, basePath, sizeof(basePath));
#else
    readlink("/proc/self/exe", basePath, sizeof(basePath));
#endif

    // Remove the executable name from the path
    char *lastSlash = strrchr(basePath, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0';
    }

    // Construct the full path to the log file
    snprintf(logFilePath, sizeof(logFilePath), "%s\\Server_UDP\\Debug\\Log.txt", basePath);
    file = fopen(logFilePath, "a");

    if (file != NULL) {
        time_t timestamp = time(NULL);
        struct tm *timeInfo = localtime(&timestamp);

        // Get current date and time
        char dateAndTime[20];
        strftime(dateAndTime, sizeof(dateAndTime), "%H:%M:%S %d/%m/%Y", timeInfo);

        // Write the log message to the file
        fprintf(file, "CLIENT - [%s] - %s\n", dateAndTime, message);

        fclose(file);
    } else {
        printf("Error opening the log file.\n");
    }
}

/**
 * @brief Display a welcome message in the console.
 *
 * This function prints a welcome message to the console, providing information
 * about the author, the basic calculator, and the supported operations. It also
 * includes instructions on how to close the connection. The message is displayed
 * using printf, character by character.
 */
void writeWelcomeMsg(void){
    char welcomeString[] =
            "* * * * * * * * * * * * * * * * * * * * * * * *\n"
            "*   Francesco Conforti - Matricola: 776628    *\n"
            "*             Basic Calculator                *\n"
            "*      Supported operations: +, -, *, /       *\n"
            "*      Enter = to close the connection        *\n"
            "* * * * * * * * * * * * * * * * * * * * * * * *\n";

    for (int i = 0; i < strlen(welcomeString); i++){
        printf("%c", welcomeString[i]);
    }
    printf("\n");
}
