/**
* @server
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
* http://www.gnu.org/copyleft/gpl.html
*
* @section DESCRIPTION
*
* This file contains the server init and main while loop for tha application.
* Uses the select() API to multiplex between network I/O and STDIN.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include "../include/server.h"
#include <arpa/inet.h>

#define BACKLOG 5
#define STDIN 0
#define TRUE 1
#define CMD_SIZE 100
#define BUFFER_SIZE 256

char server_ip[16];
char *ip;
struct client clients[100];
int num_clients = 0;

void print_clients();
void get_ip();


/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
int start_server(int argc, char **argv)
{
	if(argc != 2) {
		printf("Usage:%s [port]\n", argv[0]);
		exit(-1);
	}
	
	int server_socket, head_socket, selret, sock_index, fdaccept=0, caddr_len;
	struct sockaddr_in client_addr;
	struct addrinfo hints, *res;
	fd_set master_list, watch_list;

	/* Set up hints structure */
	memset(&hints, 0, sizeof(hints));
    	hints.ai_family = AF_INET;
    	hints.ai_socktype = SOCK_STREAM;
    	hints.ai_flags = AI_PASSIVE;

	/* Fill up address structures */
	if (getaddrinfo(NULL, argv[1], &hints, &res) != 0)
		perror("getaddrinfo failed");
	
	/* Socket */
	server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(server_socket < 0)
		perror("Cannot create socket");
	
	/* Bind */
	
	if(bind(server_socket, res->ai_addr, res->ai_addrlen) < 0 )
		perror("Bind failed");

	freeaddrinfo(res);
	
	/* Listen */
	if(listen(server_socket, BACKLOG) < 0)
		perror("Unable to listen on port");
	
	/* ---------------------------------------------------------------------------- */
	
	/* Zero select FD sets */
	FD_ZERO(&master_list);
	FD_ZERO(&watch_list);
	
	/* Register the listening socket */
	FD_SET(server_socket, &master_list);
	/* Register STDIN */
	FD_SET(STDIN, &master_list);
	
	head_socket = server_socket;
	get_ip(); /*sets ip of server to global variable server_ip*/
	
	while(TRUE){
		memcpy(&watch_list, &master_list, sizeof(master_list));
		
		printf("\n[PA1-Server@CSE489/589]$ ");
		fflush(stdout);
		
		/* select() system call. This will BLOCK */
		selret = select(head_socket + 1, &watch_list, NULL, NULL, NULL);
		if(selret < 0)
			perror("select failed.");
		
		/* Check if we have sockets/STDIN to process */
		if(selret > 0){
			/* Loop through socket descriptors to check which ones are ready */
			for(sock_index=0; sock_index<=head_socket; sock_index+=1){
				
				if(FD_ISSET(sock_index, &watch_list)){
					
					/* Check if new command on STDIN */
					if (sock_index == STDIN){
						char *cmd = (char*) malloc(sizeof(char)*CMD_SIZE);
						
						memset(cmd, '\0', CMD_SIZE);
						if(fgets(cmd, CMD_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to cmd
							exit(-1);
						
						
						if(strcmp(cmd, "IP\n") == 0){
							char *command = "IP";
							cse4589_print_and_log("[%s:SUCCESS]\n", command);
							cse4589_print_and_log("IP:%s\n", server_ip);
							cse4589_print_and_log("[%s:END]\n", command);
						}
						if(strcmp(cmd, "EXIT\n") == 0){
							char *command = "EXIT";
							cse4589_print_and_log("[%s:SUCCESS]\n", command);
							cse4589_print_and_log("[%s:END]\n", command);
							return 0;
						}
						if(strcmp(cmd, "PORT\n") == 0){
							char *command = "PORT";
							int port = atoi(argv[1]);
							cse4589_print_and_log("[%s:SUCCESS]\n", command);
							cse4589_print_and_log("PORT:%d\n", port);
							cse4589_print_and_log("[%s:END]\n", command);
						}
						if(strcmp(cmd, "AUTHOR\n") == 0){
							cse4589_print_and_log("I, stepankr, have read and understood the course academic integrity policy.\n");
						}
						if(strcmp(cmd, "LIST\n") == 0){
							print_clients();
						}

						
						
						//Process PA1 commands here ...
						
						
						free(cmd);
					}
					/* Check if new client is requesting connection */
					else if(sock_index == server_socket){
						
						caddr_len = sizeof(client_addr);
						fdaccept = accept(server_socket, (struct sockaddr *)&client_addr, &caddr_len);
						if(fdaccept < 0)
							perror("Accept failed.");
						
						

						printf("\nRemote Host connected!\n"); 
						/*connected clients ip and port*/
						char client_ip[INET_ADDRSTRLEN];
						int client_port;
						inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
						client_port = ntohs(client_addr.sin_port);

						printf("\nRemote Host connected from %s:%d!\n", client_ip, client_port);
						struct hostent *host_entry = gethostbyaddr(&(client_addr.sin_addr), sizeof(struct in_addr), AF_INET);
						char *hostname;
						if (host_entry == NULL) {
    					hostname = "unknown";
						} 						
						else {
   						 hostname = host_entry->h_name;
							}
						num_clients = num_clients + 1;
						
						/* Add to watched socket list */
						FD_SET(fdaccept, &master_list);
						if(fdaccept > head_socket) head_socket = fdaccept;

						struct client new_client;
						strcpy(new_client.ip, client_ip);
						strcpy(new_client.name, hostname);
						new_client.port = client_port;
            
						clients[num_clients] = new_client;
						
						send(fdaccept, clients, sizeof(clients), 0);
  


					
					}
					/* Read from existing clients */
					else{
						/* Initialize buffer to receieve response */
						char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
						memset(buffer, '\0', BUFFER_SIZE);
						
						if(recv(sock_index, buffer, BUFFER_SIZE, 0) <= 0){
							close(sock_index);
							printf("Remote Host terminated connection!\n");
							
							/* Remove from watched list */
							FD_CLR(sock_index, &master_list);
						}
						else {
							//Process incoming data from existing clients here ...
							if(strcmp(buffer, "EXIT\n")==0){
								close(sock_index);
								char ip[INET_ADDRSTRLEN];
								int client_port;
								inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
								remove_client_by_ip(ip);
								FD_CLR(sock_index, &master_list);
							}
							if(strcmp(buffer, "REFRESH\n") == 0){
							send(sock_index, clients, sizeof(clients), 0);
							}
							
							printf("\nClient sent me: %s\n", buffer);
							printf("ECHOing it back to the remote host ... ");
							if(send(sock_index, buffer, strlen(buffer), 0) == strlen(buffer))
								printf("Done!\n");
								
									
							fflush(stdout);
								
						}
						
						free(buffer);
					}
				}
			}
		}
	}
	
	return 0;
}
void print_clients() {
	printf("%-5s%-35s%-20s%-8s\n", "ID", "Hostname", "IP Address", "Port");

    int list_id = 1;
    for (int i = 1; i <= 50; i++) {
        if (strlen(clients[i].name) > 0) {
            printf("%-5d%-35s%-20s%-8d\n", list_id, clients[i].name, clients[i].ip, clients[i].port);
            list_id++;
        }
    }
}

void get_ip(){
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
    strcpy(server_ip, inet_ntoa(local_address.sin_addr)); /*copies ip to global server_ip variable*/
	
	freeaddrinfo(res);
	close(fdsocket);

}
void remove_client_by_ip(char* ip) {
    for (int i = 0; i < 100; i++) {
        if (strcmp(clients[i].ip, ip) == 0) {
            printf("Removing client %s\n", clients[i].name);
            clients[i] = (struct client){0};
            return;
        }
    }
}

