//
//  main.c
//  SPMileStone2
//
//  Created by Umer Jabbar on 06/04/2018.
//  Copyright © 2018 ZotionApps. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>


int client(char* buff, ssize_t size);
void* clientToServer(void* sock );


int main (int argc, char *argv[]){
    
    while(0==0){
        
        int sockfd = 0;
        char recvBuff[1024];
        struct sockaddr_in serv_addr;
        
        char buff[2000];
        
        printf("write server IP:Port \n");
        ssize_t count = read(0, buff,  100);
        buff[count - 1] = '\0';
        char *token = strtok(buff, ":");
        char *ip;
        int port = 0;
        if(token != NULL){
            ip = token;
            token = strtok(NULL, ":");
            if(token != NULL){
                port = atoi(token);
            }
        }
        
        
        
        memset(recvBuff, '0',sizeof(recvBuff));
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            
            printf("\n Error : Could not create socket \n");
            continue;
        }
        
        memset(&serv_addr, '0', sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        
        
        //    if(inet_pton(AF_INET, "172.15.53.133", &(serv_addr.sin_addr))<=0)
        //    {
        //        printf("\n inet_pton error occured\n");
        //        return 1;
        //    }
        
        if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
        {
            printf("\n inet_pton error occured\n");
            continue;
        }
        
        if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("error");
            printf("\n Error : Connect Failed \n");
            continue;
        }
        
        ssize_t cli = write(STDOUT_FILENO, "connected\n", strlen("connected\n"));
        if(cli == -1){
            perror("client on console");
        }
        
        //    ssize_t rr = read(sockfd, buff, 1000);
        //    if(rr == -1){
        //        perror("read from socket");
        //    }
        //    ssize_t r1 = write(STDIN_FILENO, buff, rr);
        //    if(r1 == -1){
        //        perror("write to console");
        //    }
        
        
        pthread_t cTos;
        pthread_create(&cTos, NULL, clientToServer, (void*) &sockfd);
        
        while(0==0){
            
            char buff[2000];
            
            
            
            ssize_t r2 = read(sockfd, buff, 2000);
            if(r2 == -1){
                perror("read from fd2[0]");
                continue;
            }else if(r2 == 0){
                perror("server is dead");
                pthread_cancel(cTos);
                break;
            }
            
            buff[r2] = '\0';
            
            if(strcmp(buff, "exit") == 0){
                pthread_cancel(cTos);
                close(sockfd);
                break;
            }
            
            int count = client(buff, r2);
            
            ssize_t w2 = write(STDOUT_FILENO, "server:~ ", sizeof("server:~ "));
            if(w2 == -1){
                perror("write on console");
            }
            
            ssize_t w3 = write(STDOUT_FILENO, buff, count);
            if(w3 == -1){
                perror("write on console");
            }
            
            ssize_t cli = write(STDOUT_FILENO, "~ ", strlen("~ "));
            if(cli == -1){
                perror("client on console");
            }
        }
    }
}


void* clientToServer(void* sock ){
    
    int sockfd = *(int*) sock;
    
    while(0==0){
        char buff[2000];
        
        ssize_t li = write(STDOUT_FILENO, "~ ", strlen("~ "));
        if(li == -1){
            perror("client on console");
        }

        ssize_t r1 = read(STDIN_FILENO, buff, 2000);
        if(r1 == -1){
            perror("read from console");
            continue;
        }
        else if(buff[0] == '\n'){
            continue;
        }
        ssize_t w1 = write(sockfd, buff, r1);
        if(w1 == -1){
            perror("write on fd1[1]");
            break;
        }else if(w1 == 0){
            perror("server is dead");
            break;
        }
        
    }
    
    pthread_exit(NULL);
    
}

int client(char* buff, ssize_t size){
    
    buff[size] = '\n';
    
    return (int)size+1;
}




