#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#define BUF_SIZE 4096

void show_msg(char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vprintf("%s\n", format);
    va_end(ap);
}

int main(int argc, char const *argv[])
{
    char read_buf[BUF_SIZE];
    char send_buf[BUF_SIZE];
    socket control_sock;
	struct hostent *hp;
	struct sockaddr_in server;
	memset(&server, 0, sizeof(struct sockaddr_in));

	char *server_name = "localhost";
	int port = 21;

	/* 初始化socket */
	control_sock = socket(AF_INET, SOCK_STREAM, 0);
	hp = gethostbyname(server_name);
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	int read_len;
	char *username = "john";
	/* 连接到服务器端 */
	connect(control_sock,(struct sockaddr *)&server, sizeof(server));
	/* 客户端接收服务器端的一些欢迎信息 */
	read(control_sock, read_buf, read_len);
	show_msg(read_buf);
	/* 命令 ”USER username\r\n” */
	sprintf(send_buf,"USER %s\r\n", username);
	/*客户端发送用户名到服务器端 */
	write(control_sock, send_buf, strlen(send_buf));
	/* 客户端接收服务器的响应码和信息，正常为 ”331 User name okay, need password.” */
	read(control_sock, read_buf, read_len);
	show_msg(read_buf);

	char *password = "justleefeng";
	/* 命令 ”PASS password\r\n” */
	sprintf(send_buf,"PASS %s\r\n",password);
	/* 客户端发送密码到服务器端 */
	write(control_sock, send_buf, strlen(send_buf));
	/* 客户端接收服务器的响应码和信息，正常为 ”230 User logged in, proceed.” */
	read(control_sock, read_buf, read_len);
	show_msg(read_buf);
	/* 命令 ”PASV\r\n” */
	sprintf(send_buf,"PASV\r\n");
	/* 客户端告诉服务器用被动模式 */
	write(control_sock, send_buf, strlen(send_buf));
	/*客户端接收服务器的响应码和新开的端口号，
	* 正常为 ”227 Entering passive mode (<h1,h2,h3,h4,p1,p2>)” */
	read(control_sock, read_buf, read_len);
	show_msg(read_buf);
	socket data_sock = socket(AF_INET, SOCK_STREAM, 0);
	/* 连接服务器新开的数据端口 */
	connect(data_sock,(struct sockaddr *)&server, sizeof(server));
	/* 命令 ”CWD dirname\r\n” */
	sprintf(send_buf,"CWD %s\r\n", dirname);
	/* 客户端发送命令改变工作目录 */
	write(control_sock, send_buf, strlen(send_buf));
	/* 客户端接收服务器的响应码和信息，正常为 ”250 Command okay.” */
	read(control_sock, read_buf, read_len);
	show_msg(read_buf);

	/* 命令 ”SIZE filename\r\n” */
	sprintf(send_buf,"SIZE %s\r\n",filename);
	/* 客户端发送命令从服务器端得到下载文件的大小 */
	write(control_sock, send_buf, strlen(send_buf));
	/* 客户端接收服务器的响应码和信息，正常为 ”213 <size>” */
	read(control_sock, read_buf, read_len);
        show_msg(read_buf);


	/* 命令 ”RETR filename\r\n” */
	sprintf(send_buf,"RETR %s\r\n",filename);
	/* 客户端发送命令从服务器端下载文件 */
	write(control_sock, send_buf, strlen(send_buf));
	/* 客户端接收服务器的响应码和信息，正常为 ”150 Opening data connection.” */
	read(control_sock, read_buf, read_len);
        show_msg(read_buf);

	/* 客户端创建文件 */
	file_handle = open(disk_name, CRFLAGS, RWXALL);
	for( ; ; ) {
	/* 客户端通过数据连接 从服务器接收文件内容 */
	read(data_sock, read_buf, read_len);
	/* 客户端写文件 */
	write(file_handle, read_buf, read_len);
	}
	/* 客户端关闭文件 */
	rc = close(file_handle);
	/* 客户端关闭数据连接 */
	close(data_sock);
	/* 客户端接收服务器的响应码和信息，正常为 ”226 Transfer complete.” */
	read(control_sock, read_buf, read_len);

	/* 命令 ”QUIT\r\n” */
	sprintf(send_buf,"QUIT\r\n");
	/* 客户端将断开与服务器端的连接 */
	write(control_sock, send_buf, strlen(send_buf));
	/* 客户端接收服务器的响应码，正常为 ”200 Closes connection.” */
	read(control_sock, read_buf, read_len);
	/* 客户端关闭控制连接 */
	close(control_sock);
	return 0;
}
