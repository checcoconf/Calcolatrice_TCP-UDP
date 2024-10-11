#include "Headers.h"
#include "Client.h"

/**
 * @file Client.c
 * @brief Implementation file for a simple client.
 * @date November 13, 2023
 * @author Francesco Conforti
 */

/**
 * @brief Main function for the client application.
 *
 * This function initializes the WSA library (if on Windows), creates a socket,
 * binds the socket, connects to the server, receives a welcome message, sends
 * and receives data to and from the server until the server sends "Bye," and then
 * closes the connection.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return 0 upon successful execution.
 */
int main(int argc, char *argv[]) {
    // 0) Initialize the WSA library in case we are on Windows
    checkWindowDevice();

    // 1) Create a socket
    int c_socket = -1;
    c_socket = createSocket(c_socket);

    // 2) Bind the socket
    struct sockaddr_in sad = bindSocket(sad);

    // 3) Connect to the server
    connectToServer(c_socket, sad);

    // Receive Welcome Message
    receiveData(c_socket, sizeof(char) * BUFFERSIZE, msg);

    // 4) Send data
    while (1) {
        // Request data from the console
        inputString(msg);

        sendData(c_socket, msg);

        // 5) Receive data from the server
        receiveData(c_socket, sizeof(char) * BUFFERSIZE, msg);

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
 * @brief Displays an error message to the console.
 *
 * @param errorMessage The error message to display.
 */
void errorhandler(char *errorMessage) {
    printf("\n%s", errorMessage);
    writeLog(errorMessage);
}

/**
 * @brief Cleans up Windows socket resources if on a Windows platform.
 */
void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

/**
 * @brief Sends data to the server through the given socket.
 *
 * @param c_socket The socket for sending data.
 * @param msg The message to be sent.
 * @return 1 if the send operation is successful, -1 if there is an error.
 */
int sendData(int c_socket, char *msg) {
    // Send data to the server
    if (send(c_socket, msg, sizeof(sizeof(char) * BUFFERSIZE), 0) != sizeof(sizeof(char) * BUFFERSIZE)) {
        errorhandler("send() sent a different number of bytes than expected.");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    }
    return 1;
}

/**
 * @brief Receives data from the server through the given socket and stores it in msg.
 *
 * @param c_socket The socket for receiving data.
 * @param string_len The length of the string to receive.
 * @param msg The received message will be stored in this buffer.
 * @return 0 if data is received successfully, -1 if there is an error.
 */
int receiveData(int c_socket, int string_len, char *msg) {
    int bytes_received;
    int total_bytes_received = 0;

    memset(msg, 0, string_len); // Ensure any extra bytes are set to 0
    printf("Received: "); // Set up to print the received string

    while (total_bytes_received < string_len) {
        if ((bytes_received = recv(c_socket, msg + total_bytes_received,string_len - total_bytes_received, 0)) <= 0) {
            errorhandler("recv() failed or the connection closed prematurely.");
            closesocket(c_socket);
            clearwinsock();
            return -1;
        }
        total_bytes_received += bytes_received; // Keep track of the total received bytes
    }

    msg[total_bytes_received] = '\0'; // Null-terminate the received message

    printf("%s\n", msg);

    return 0;
}

/**
 * @brief Closes the connection and performs cleanup.
 *
 * @param c_socket The socket to close.
 */
void closeConnection(int c_socket) {
    closesocket(c_socket);
    clearwinsock();

    system("pause");
    getchar();
}


/**
 * @brief Creates a TCP socket and initializes it.
 *
 * @param c_socket The socket to be created.
 * @return The created socket if successful, -1 if there is an error.
 */
int createSocket(int c_socket) {
    // Creation of a TCP socket
    c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c_socket < 0) {
        errorhandler("Socket creation failed.");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    } else {
        printf("Socket created successfully!\n");
    }
    return c_socket;
}

/**
 * @brief Binds the socket to the specified address and port.
 *
 * @param sad The sockaddr_in structure containing address and port information.
 * @return The sockaddr_in structure after binding.
 */
struct sockaddr_in bindSocket(struct sockaddr_in sad) {
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;  // Set IPv4 socket
    sad.sin_addr.s_addr = inet_addr(PROTO_ADDR); // Server's IP
    sad.sin_port = htons(PROTOPORT); // Server's port

    return sad;
}

/**
 * @brief Connects to the server using the provided socket and address.
 *
 * @param c_socket The socket to connect.
 * @param sad The sockaddr_in structure containing server address information.
 * @return 1 if the connection is successful, -1 if there is an error.
 */
int connectToServer(int c_socket, struct sockaddr_in sad) {
    if (connect(c_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
        errorhandler("Connection failed.");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    } else {
        printf("Connection Established!\n");
        return 1;
    }
}

/**
 * @brief Initializes the WSA library if on a Windows platform.
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
 * @brief Reads and validates user input as a command string to send to the server.
 * If the input is invalid, it prompts the user to enter a valid input.
 *
 * @param msg The command string entered by the user.
 */
void inputString(char *msg) {
    printf("\nEnter the commands to send to the server: ");
    // Use fgets to read an entire line, including spaces
    if (fgets(msg, BUFFERSIZE, stdin) != NULL) {
        // Check if the input exceeds BUFFERSIZE
        if (!strchr(msg, '\n')) {
            // Input exceeds BUFFERSIZE, clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            sprintf(msgLog,"Input too long. Please enter commands less than %d characters.", BUFFERSIZE);
            printf("%s\n", msgLog);
            writeLog(msgLog);
        } else {
            // Remove the trailing newline character if present
            size_t length = strlen(msg);
            if (length > 0 && msg[length - 1] == '\n') {
                msg[length - 1] = '\0'; // Truncate the string if it's too long
            }
        }
    } else {
        // fgets failed, handle the error
        printf(msgLog,"Error reading input.");
        writeLog(msgLog);
    }

    // Check if the input string format is valid (operator value value)
    // Regex for all devices
    char operator;
    int num1, num2;
    if (sscanf(msg, "%c", &operator) == 1 && strchr("=", operator) != NULL) {
        printf("\nClosing the connection...\n");
    } else {
        if (sscanf(msg, "%c %d %d", &operator, &num1, &num2) != 3 || strchr("+-*/", operator) == NULL) {
            sprintf(msgLog,"Invalid input format. Please use the format: operator [+-*/] value value or = to close the connection");
            printf("%s\n", msgLog);
            writeLog(msg);
            writeLog(msgLog);
            inputString(msg); // Prompt the user to enter a valid input
        }
    }
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
        fprintf(file, "CLIENT - [%s] - %s\n", dateAndTime, message);

        fclose(file);
    } else {
        printf("Error opening the log file.\n");
    }
}
