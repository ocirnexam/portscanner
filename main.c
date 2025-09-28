#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PRINT_CLOSED_PORT_FALSE 1
#define PRINT_CLOSED_PORT_TRUE 0

typedef struct {
    char *ip;
    int port;
    struct sockaddr_in address_info;
    int socket_nr;
} socket_info_t;

void print_port_status(socket_info_t *s_info, int print_closed_port);
void check_single_port(char *ip, int port);
void check_all_ports(char *ip);

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3) {
        printf("Error!\nUsage: portscanner <ip> [<port>]\n");
        return -1;
    }
    if (argc == 2) {
        check_all_ports(argv[1]);
    }
    if (argc == 3) {
        check_single_port(argv[1], atoi(argv[2]));
    }
    
    printf("\n\n");
    return 0;
}

void check_single_port(char *ip, int port)
{
    socket_info_t s_info;
    int socket_nr;
    struct sockaddr_in address_info;
    socket_nr = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_nr < 0) {
        perror("Opening socket failed!\n");
        return;
    }
    address_info.sin_family = AF_INET;
    address_info.sin_addr.s_addr = inet_addr(ip);
    address_info.sin_port = htons(port);
    s_info.socket_nr = socket_nr;
    s_info.address_info = address_info;
    s_info.ip = ip;
    s_info.port = port;
    print_port_status(&s_info, PRINT_CLOSED_PORT_TRUE);
    close(socket_nr);
}

void check_all_ports(char *ip)
{
    socket_info_t s_info;
    int socket_nr;
    struct sockaddr_in address_info;
    for(int port = 0; port < 65535; port++)
    {
        socket_nr = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_nr < 0)
        {
            printf("Socket error!\n");
            return;
        }
        address_info.sin_family = AF_INET;
        address_info.sin_addr.s_addr = inet_addr(ip);

        address_info.sin_port = htons(port);
        s_info.socket_nr = socket_nr;
        s_info.address_info = address_info;
        s_info.ip = ip;
        s_info.port = port;
        print_port_status(&s_info, PRINT_CLOSED_PORT_FALSE);
        close(socket_nr);
    }
}

void print_port_status(socket_info_t *s_info, int print_closed_port)
{
    if (connect(s_info->socket_nr, (struct sockaddr *)&s_info->address_info, sizeof(s_info->address_info)) == 0)
    {
        printf("IP %s: Port %d open!\n", s_info->ip, s_info->port);
    }
    else if (print_closed_port == PRINT_CLOSED_PORT_TRUE)
    {
        printf("IP %s: Port %d is closed!\n", s_info->ip, s_info->port);
    }
}
