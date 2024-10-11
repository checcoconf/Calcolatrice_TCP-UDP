/**
 * @file Headers.h
 * @brief Header file containing common includes for testing purposes.
 * @date November 13, 2023
 * @author Francesco Conforti
 */

#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>      /**< Standard input/output functions */
#include <stdlib.h>     /**< Standard library functions */
#include <string.h>     /**< String manipulation functions */
#include <time.h>       /**< Time functions */

#if defined WIN32
#include <winsock.h>    /**< Windows Sockets API */
#include <ws2tcpip.h>   /**< Windows Sockets 2 API */
#else
#include <unistd.h>     /**< Symbolic constants and types for POSIX */
#include <sys/socket.h> /**< Socket functions */
#include <arpa/inet.h>  /**< Definitions for internet operations */
#include <netinet/in.h> /**< Internet address family */
#define closesocket close
#endif

/**
 * @def HEADERS_H_
 * @brief Definition to avoid double inclusion of the header file.
 */

#endif /* HEADERS_H_ */
