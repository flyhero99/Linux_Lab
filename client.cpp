#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

#define msleep(n) usleep(n*1000)
#define MAX_X 80
#define MAX_Y 24
#define EMPTY 0
#define TANK 1
#define BULLET 2
#define SHU 3
#define HENG 4
#define PORT 8888

// char IP_ADDRESS[] = "192.168.43.201";
char IP_ADDRESS[] = "127.0.0.1";

char game_map[24][80] =
{
/*00*/' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*01*/' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*02*/' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','h','a','h','a','h','a',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','x','i','x','i','x','i',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','h','e','h','e','h','e',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*03*/' ',' ',' ',' ',' ',' ',' ',' ',' ','|','_','_','_','_','_','_','_','_','_','_','_','_','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','_','_','_','_','_','_','_','_','_','_','_','_','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','_','_','_','_','_','_','_','_','_','_','_','_','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*04*/' ','=','=','=','=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','=','=','=','=',' ',
/*05*/' ','&',' ',' ','&','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','&',' ',' ','&',' ',
/*06*/' ',' ','&','&',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','&','&',' ',' ',
/*07*/' ',' ','&','&',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','&','&',' ',' ',
/*08*/' ','&',' ',' ','&','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ','&','&','&','&','&',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','&',' ',' ','&',' ',
/*09*/' ','=','=','=','=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','&','&',' ',' ',' ',' ','&',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','=','=','=','=',' ',
/*10*/' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','&',' ','&',' ',' ',' ','&',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*11*/' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','&',' ',' ','&',' ',' ','&',' ',' ',' ','&','&','&','&','&',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*12*/' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ','&',' ','&',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*13*/' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ',' ','&','&',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','=','=','=','=',' ',
/*14*/' ','=','=','=','=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','&',' ',' ',' ',' ',' ','&',' ',' ',' ','&','&','&','&','&',' ',' ',' ',' ','&','&','&','&','&',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','&',' ',' ','&',' ',
/*15*/' ','&',' ',' ','&','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','&','&',' ',' ',
/*16*/' ',' ','&','&',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','&','&',' ',' ',
/*17*/' ',' ','&','&',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','&',' ',' ','&',' ',
/*18*/' ','&',' ',' ','&','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','=','=','=','=',' ',
/*19*/' ','=','=','=','=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*20*/' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_','_','_','_','_','_','_','_','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_','_','_','_','_','_','_','_','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_','_','_','_','_','_','_','_','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*21*/' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*22*/' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','h','o','h','o','h','o',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','l','u','l','u','l','u',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','l','a','l','a','l','a',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',
/*23*/' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',
};

/* global variables */
bool is_start = false;
int is_end = 0;
pthread_mutex_t start_lock, win_lock;

/* function declarations */
void initial();
void * handle_input(void *arg);
void * handle_recv(void *arg);
void handle_msg(vector<char *> vec);
void show_background();
void show_lose();
void show_win();
void display_tank(int y, int x, char c);
void display_bullet(int y, int x);
void display_empty(int y, int x);
vector<char *> split(char *str, char *c);
int connect_to_server(char *host, int portnum);
void show_map();


int main() {
    int sock_fd = connect_to_server(IP_ADDRESS, PORT);
    if(sock_fd == -1) { perror("socket error!"); exit(1); }
    initial();

    pthread_t send_tid, recv_tid;
    pthread_create(&send_tid, NULL, handle_input, (int *)&sock_fd);
    pthread_create(&recv_tid, NULL, handle_recv, (int *)&sock_fd);

    while(1);
    return 0;
}

/**
 * Use a new thread to handle the user input and send it to the server.
 */
void * handle_input(void *arg) {
    int sock_fd = * (int *)arg;
    while(1) {
        char ch = getch();
        char sendbuf[105];

        pthread_mutex_lock(&start_lock);
        if(is_start == false) {
            strcpy(sendbuf, "start");
            write(sock_fd, sendbuf, 5);
        }
        else {
            if(is_end == 0) {
                sprintf(sendbuf, "%c", ch);
                write(sock_fd, sendbuf, 1);
            }
        }
        pthread_mutex_unlock(&start_lock);
    }
}

/**
 * Use a new thread to handle receive message from the server
 * and explain it, and display the contents on the screen.
 */
void * handle_recv(void *arg) {
    int sock_fd = * (int *)arg;
    while(1) {
        char recvbuf[100] = {};
        int n = read(sock_fd, recvbuf, 100);

        char nl[5] = "\n", sp[5] = " ";
        vector<char *> vec = split(recvbuf, nl);

        if(strcmp(vec[0], "begin") == 0) {
            pthread_mutex_lock(&start_lock);
            is_start = true;
            pthread_mutex_unlock(&start_lock);

            show_map();
            handle_msg(vec);
        }
        else if(strcmp(vec[0], "win") == 0) {
            pthread_mutex_lock(&start_lock);
            is_end = 1;
            pthread_mutex_unlock(&start_lock);

            handle_msg(vec);
            show_win();
        }
        else if(strcmp(vec[0], "lose") == 0) {
            pthread_mutex_lock(&start_lock);
            is_end = 2;
            pthread_mutex_unlock(&start_lock);

            handle_msg(vec);
            show_lose();
        }
        else if(strcmp(vec[0], "movetank") == 0) {
            handle_msg(vec);
        }
        else if(strcmp(vec[0], "shoot") == 0) {
            handle_msg(vec);
        }
    }
}

/**
 * Display a tank at position (x, y).
 */
void display_tank(int y, int x, char c) {
    mvaddch(y, x, c);
    refresh();
}

/**
 * Display a bullet at position (x, y).
 */
void display_bullet(int y, int x) {
    mvaddch(y, x, '*');
    refresh();
}

/**
 * Display an empty space at position (x, y).
 */
void display_empty(int y, int x) {
    mvaddch(y, x, ' ');
    refresh();
}

void initial() {
    initscr();//开启curses模式
    cbreak();//开启cbreak模式,除了 DELETE 或 CTRL 等仍被视为特殊控制字元外一切输入的字元将立刻被一一读取
    nonl();//用来决定当输入资料时, 按下 RETURN 键是否被对应为 NEWLINE 字元
    noecho();//echo() and noecho(): 此函式用来控制从键盘输入字元时是否将字元显示在终端机上
    intrflush(stdscr, false);
    keypad(stdscr, true);//当开启 keypad 後, 可以使用键盘上的一些特殊字元, 如上下左右>等方向键
    curs_set(0);
    refresh();//将做清除萤幕的工作
}

/**
 * Split the string with the expected token.
 */
vector<char *> split(char *str, char *c) {
    char *p;
    vector<char *> res;
    p = strtok(str, c);
    while(p != NULL) {
        res.push_back(p);
        p = strtok(NULL, c);
    }
    return res;
}

/**
 * Explain the message that the server sends.
 */
void handle_msg(vector<char *> vec) {
    char sp[5] = " ";
    for(int i = 1; i < vec.size(); i++) {
        vector<char *> tmpvec = split(vec[i], sp);
        if(strcmp(tmpvec[0], "bullet") == 0) {
            int x = atoi(tmpvec[1]), y = atoi(tmpvec[2]);
            display_bullet(y, x);
        }
        else if(strcmp(tmpvec[0], "tank") == 0) {
            int x = atoi(tmpvec[1]), y = atoi(tmpvec[2]);
            char mark = tmpvec[3][0];
            display_tank(y, x, mark);
        }
        else if(strcmp(tmpvec[0], "empty") == 0){
            int x = atoi(tmpvec[1]), y = atoi(tmpvec[2]);
            display_empty(y, x);
        }
    }
}

/**
 * The player win, show "YOU WIN" on the top of the screen.
 */
void show_win() {
    attron(A_REVERSE);
    mvaddstr(0, 35, " YOU WIN!!! ");
    attroff(A_REVERSE);
    refresh();
}

/**
 * The player lose, show "YOU LOSE" on the top of the screen.
 */
void show_lose() {
    attron(A_REVERSE);
    mvaddstr(0, 35, " YOU LOSE...");
    attroff(A_REVERSE);
    refresh();
}

/**
 * Display the map at the beginning.
 */
void show_map() {
    box(stdscr, '|', '-');//将以 | 及 - 围成一个方框.
    attron(A_REVERSE);
    mvaddstr(0, 35, "TANK BALTTLE");
    attroff(A_REVERSE);
    for(int y = 1; y <= 22; y++) {
        for(int x = 1; x <= 78; x++) {
            mvaddch(y, x, game_map[y][x]);
        }
    }
    refresh();
}

int connect_to_server(char *host, int portnum) {
	int sock;
	struct sockaddr_in servadd; /* the number to call */
	struct hostent *hp; /* used to get number */
	/** Step 1: Get a socket **/
	sock = socket(AF_INET, SOCK_STREAM, 0); /* get a line */
	if(sock == -1)
		return -1;
	/** Step 2: connect to server **/
	bzero( &servadd, sizeof(servadd) ); /* zero the address */
	hp = gethostbyname( host ); /* lookup host's ip # */
	if(hp == NULL)
		return -1;
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(portnum); /* fill in port number */
	servadd.sin_family = AF_INET ; /* fill in socket type */
	if (connect(sock, (struct sockaddr *)&servadd, sizeof(servadd)) !=0)
		return -1;
	return sock;
}
