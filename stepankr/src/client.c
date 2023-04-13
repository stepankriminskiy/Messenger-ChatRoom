/**
* @client
* @author  Swetank Kumar Saha <swetankk@buffalo.edu>, Shivang Aggarwal <shivanga@buffalo.edu>
* @version 1.0
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details at
* http://www.gnu.org/copyleft/gpl.hml
*
* @section DESCRIPTION
*
* This file contains the client.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include "../include/client.h"
#include "../include/global.h"
#include <ctype.h>


#define TRUE 1
#define MSG_SIZE 256
#define BUFFER_SIZE 256


int listening_port;
char client_ip[16];
int connected = 0;
void get_ip_client();
void print_clients2();
struct client clients[100];

int connect_to_host(char *server_ip, char *server_port);

/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/

int is_valid_port(const char* str) {
    char* port = str;
	while (*str) {
        if (isdigit(*str) == 0)
            return 0;
		str++;
	return atoi(port);
	}
}
int start_client(int argc, char **argv)
{

	if(argc != 2) {
		printf("Usage:%s [port]\n", argv[0]);
		return -1;
	}
	get_ip_client();
	char* ip;
    char* port;
	char* token;
	listening_port = atoi(argv[1]);
	while(TRUE){
		char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
		memset(msg, '\0', MSG_SIZE);
		if(fgets(msg, MSG_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to msg
			return -1;
		if(strcmp(msg, "AUTHOR\n") == 0){
			msg = "AUTHOR";
			cse4589_print_and_log("[%s:SUCCESS]\n", msg);
			cse4589_print_and_log("I, stepankr, have read and understood the course academic integrity policy.\n");
			cse4589_print_and_log("[%s:END]\n", msg);
		}
		if (strcmp(msg, "IP\n") == 0) {
			msg = "IP";
			cse4589_print_and_log("[%s:SUCCESS]\n", msg);
			cse4589_print_and_log("IP:%s\n", client_ip);
			cse4589_print_and_log("[%s:END]\n", msg);
			
		}
		if (strcmp(msg, "LIST\n") == 0) {
			char *command = "LIST";
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			print_clients2();
			cse4589_print_and_log("[%s:END]\n", command);
		}
		if (strcmp(msg, "EXIT\n") == 0) {		
			return 0;
			
		}
		if (strcmp(msg, "PORT\n") == 0) {
			int port = atoi(argv[1]);
			msg = "PORT";
			cse4589_print_and_log("[%s:SUCCESS]\n", msg);
			cse4589_print_and_log("PORT:%d\n", port);
			cse4589_print_and_log("[%s:END]\n", msg);
			
		}
		if (strstr(msg, "LOGIN") != NULL){
			token = strtok(msg, " ");
			ip = strtok(NULL, " ");
        	port = strtok(NULL, " ");
			port[strcspn(port, "\n")] = '\0';
			struct in_addr addr;
    		int result = inet_pton(AF_INET, ip, &addr);
			int Validport = is_valid_port(port);
			
			
			if(Validport != 0 && result == 1){
				printf("ip and port valid");
				break;
				
			}
			else{
			cse4589_print_and_log("[%s:ERROR]\n", msg);
			cse4589_print_and_log("[%s:END]\n", msg);

			}	
		}
	}
	int server;

	server = connect_to_host(ip, port);
	memset(clients, 0, sizeof(clients));
	send(server, &listening_port, sizeof(listening_port), 0);
	recv(server, clients, sizeof(clients), 0);
	
	while(TRUE){
		printf("\n[PA1-Client@CSE489/589]$ ");
		fflush(stdout);
		
		char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
		memset(msg, '\0', MSG_SIZE);
		if(fgets(msg, MSG_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to msg
			return -1;
		
		
		
		printf("I got: %s(size:%d chars)", msg, strlen(msg));
		
		printf("\nSENDing it to the remote server ... ");
		if(send(server, msg, strlen(msg), 0) == strlen(msg))
			printf("Done!\n");
			if(strcmp(msg, "REFRESH\n") == 0){
				msg = "REFRESH";
				cse4589_print_and_log("[%s:SUCCESS]\n", msg);
				memset(clients, 0, sizeof(clients));
				recv(server, clients, sizeof(clients), 0);
				cse4589_print_and_log("[%s:END]\n", msg);
				
				
			}
			else if(strcmp(msg, "LIST\n") == 0){
				char *command = "LIST";
				cse4589_print_and_log("[%s:SUCCESS]\n", command);
				print_clients2();
				cse4589_print_and_log("[%s:END]\n", command);
				
			}
			else if(strcmp(msg, "EXIT\n") == 0){
				msg = "EXIT";
				
				send(server, &listening_port, sizeof(listening_port), 0);
				close(server);
				
				
				return 0;
			}
			else if(strcmp(msg, "AUTHOR\n") == 0){
				msg = "AUTHOR";
				
				cse4589_print_and_log("[%s:SUCCESS]\n", msg);
				cse4589_print_and_log("I, stepankr, have read and understood the course academic integrity policy.\n");
				cse4589_print_and_log("[%s:END]\n", msg);
			}
			else if (strcmp(msg, "IP\n") == 0) {
				msg = "IP";
				cse4589_print_and_log("[%s:SUCCESS]\n", msg);
				cse4589_print_and_log("IP:%s\n", client_ip);
				cse4589_print_and_log("[%s:END]\n", msg);
			
			}
			else if (strcmp(msg, "PORT\n") == 0) {
				int port = atoi(argv[1]);
				msg = "PORT";
				cse4589_print_and_log("[%s:SUCCESS]\n", msg);
				cse4589_print_and_log("PORT:%d\n", port);
				cse4589_print_and_log("[%s:END]\n", msg);
			}
			
			/* Initialize buffer to receieve response */
			char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
			memset(buffer, '\0', BUFFER_SIZE);
			if(recv(server, buffer, BUFFER_SIZE, 0) >= 0){
			//printf("Server responded: %s", buffer);
			fflush(stdout);
			}
			
			
		

	}
}

int connect_to_host(char *server_ip, char* server_port)
{
	int fdsocket;
	struct addrinfo hints, *res;

	/* Set up hints structure */	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; 

	/* Fill up address structures */	
	if (getaddrinfo(server_ip, server_port, &hints, &res) != 0)
		perror("getaddrinfo failed");
		
		
	/* Socket */
	fdsocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(fdsocket < 0)
		perror("Failed to create socket");
	
	bind(fdsocket, res->ai_addr, res->ai_addrlen);
	getaddrinfo(server_ip, server_port, &hints, &res);
	/* Connect */
	if(connect(fdsocket, res->ai_addr, res->ai_addrlen) < 0)
		perror("Connect failed");
	
	
	
	freeaddrinfo(res);

	return fdsocket;
}
void get_ip_client(){
	int fdsocket;
	struct addrinfo hints, *res;

	/* Set up hints structure */	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	/* Fill up address structures */	
	getaddrinfo("8.8.8.8", "53", &hints, &res); /*set address info to google ip and port*/
		
	/* Socket */
	fdsocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(fdsocket, res->ai_addr, res->ai_addrlen); /*connects socket*/
		
	
	struct sockaddr_in local_address;
	socklen_t address_length = sizeof(local_address);

	getsockname(fdsocket, (struct sockaddr *)&local_address, &address_length);
    strcpy(client_ip, inet_ntoa(local_address.sin_addr)); /*copies ip to global server_ip variable*/
	
	freeaddrinfo(res);
	close(fdsocket);

}
void print_clients2() {
	
    int list_id = 1;
    for (int i = 1; i <= 50; i++) {
        if (strlen(clients[i].name) > 0) {
            cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", list_id, clients[i].name, clients[i].ip, clients[i].port);
            list_id++;
        }
    }
}
