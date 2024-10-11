/**
 * @file Headers.h
 * @brief Header file containing common includes for testing purposes.
 * @date November 13, 2023
 * @author Francesco Conforti
 *
 * This header file includes common standard libraries and platform-specific headers
 * for testing purposes. It provides a consistent set of includes for cross-platform
 * development and testing.
 */

#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>      /**< Standard input/output functions */
#include <stdlib.h>     /**< Standard library functions */
#include <string.h>     /**< String manipulation functions */
#include <time.h>       /**< Time functions */

#if defined WIN32
#include <winsock.h>    /**< Windows Sockets API */
#else
#include <unistd.h>     /**< Symbolic constants and types for POSIX */
#include <sys/socket.h> /**< Socket functions */
#include <arpa/inet.h>  /**< Definitions for internet operations */
#include <netinet/in.h> /**< Internet address family */
#define closesocket close
#endif

#endif /* HEADERS_H_ */
