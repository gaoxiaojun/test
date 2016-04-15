#ifndef _CFTP_H
#define _CFTP_H
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
union u_ip
{
    unsigned char b[4];
    unsigned int ip;
} uip;

union u_port 
{
    unsigned char b[2];
    unsigned short port;
} uport;

class ftp_client
{
public:
    ftp_client(const char *server_name="localhost", int port=21);
    ~ftp_client();
    void connect_to_server(const char *username, const char *password);
    void cd(const char *dirname);
    void get_file(const char *filename, const char *diskname);
    void get_dir(const char *dirname);
    void list(const char *dirname);
    void noop();
    void bye();
private:
    int get_port();
    int sock_ctrl;
    struct sockaddr_in server;
    static const int buf_size = 4096;
    char buf[buf_size];    
};
#endif
