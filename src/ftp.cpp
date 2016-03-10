#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#include <string.h>
#include "ftp.h"

void show_msg(char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
} 

ftp_client::ftp_client(const char *server_name, int port)
{
    bzero(&server, sizeof(struct sockaddr_in));
    struct hostent *hp = gethostbyname(server_name);
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    sock_ctrl = socket(AF_INET, SOCK_STREAM, 0);
}

ftp_client::~ftp_client()
{
    close(sock_ctrl);
}

void ftp_client::connect_to_server(const char *username, const char *password)
{
    connect(sock_ctrl, (struct sockaddr *)&server, sizeof(server));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);
    bzero(buf, buf_size);

    sprintf(buf, "USER %s\r\n", username);
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);
    bzero(buf, buf_size);

    sprintf(buf, "PASS %s\r\n", password);
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);
    bzero(buf, buf_size);
}

void ftp_client::cd(const char *dirname)
{
    sprintf(buf, "CWD %s\r\n", dirname);
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);
    bzero(buf, buf_size);
}

void ftp_client::get_file(const char *filename, const char *diskname)
{
    sprintf(buf, "PASV\r\n");
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);

    int port = get_port();
    if (port == -1)
    {
        show_msg("get port error!\n");
        return;
    }
    sockaddr_in new_server;
    memcpy(&new_server, &server, sizeof(server));
    new_server.sin_port = htons(port);
    int new_sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(new_sock, (struct sockaddr *)&new_server, sizeof(new_server));

    sprintf(buf, "RETR %s\r\n", filename);
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);

    int fd = open(diskname, O_RDWR | O_CREAT, 0644);
    int len = 0;
    while ((len = read(new_sock, buf, buf_size))) 
    {
        write(fd, buf, len);
        bzero(buf, buf_size);
    }
    close(fd);
    close(new_sock);
}

int ftp_client::get_port()
{
    char *left = strchr(buf, '(');
    if (left == NULL)
        return -1;
    unsigned int u[6];
    sscanf(left + 1, "%u,%u,%u,%u,%u,%u", u, u+1, u+2, u+3, u+4, u+5);
    uport.b[1] = u[4];
    uport.b[0] = u[5];
    return uport.port;
}

void ftp_client::get_dir(const char *dirname)
{

}

void ftp_client::list(const char *dirname)
{
    sprintf(buf, "PASV\r\n");
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);

    int port = get_port();
    if (port == -1)
    {
        show_msg("get port error!\n");
        return;
    }
    bzero(buf, buf_size);
    sockaddr_in new_server;
    memcpy(&new_server, &server, sizeof(server));
    new_server.sin_port = htons(port);
    int new_sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(new_sock, (struct sockaddr *)&new_server, sizeof(new_server));

    if (dirname == NULL)
    {
        sprintf(buf, "LIST\r\n");
    }
    else
    {
        sprintf(buf, "LIST %s\r\n", dirname);
    }
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);
    bzero(buf, buf_size);

    int rx_len = 0;
    while ((rx_len = read(new_sock, buf, buf_size)))
    {
        show_msg(buf);
        bzero(buf, buf_size);
    }

    close(new_sock);
}

void ftp_client::bye()
{
    sprintf(buf, "QUIT\r\n");
    write(sock_ctrl, buf, strlen(buf));
    read(sock_ctrl, buf, buf_size);
    show_msg(buf);
    close(sock_ctrl);
}

int main()
{
    ftp_client client;
//    client.connect_to_server("john", "justleefeng");
//    client.cd("/home/john");
//    client.get_file("ftp.cpp", "/home/john/test_ftp/ftp.cpp");
//    client.list("/home/john");
    client.bye();
    return 0;
}
