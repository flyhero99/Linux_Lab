#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <bits/stdc++.h>

#define msleep(n) usleep(n*1000)
#define MAXLINE 100
#define PORT 8888
#define MAXX 80
#define MAXY 24
#define EMPTY 0
#define TANK 1
#define BULLET 2
#define BLOCK 3
#define WALL 4

using namespace std;

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

/* class declarations */
class Game;
class Bullet;
class Tank;

/* struct declarations */
struct io_data_t { // save the data of a fd
    int fd;
    int epfd;
    struct epoll_event *ev;
    Game *game;
    Tank *tank;

    io_data_t(int _fd, int _epfd, struct epoll_event *_ev, Game *_game, Tank *_tank)
    : fd(_fd), epfd(_epfd), ev(_ev), game(_game), tank(_tank) {}
};

struct bullet_info { // the parameter passed to the thread function
    io_data_t *io;
    int x; int y; char toward;

    bullet_info(io_data_t *_io, int _x, int _y, char _toward)
    : io(_io), x(_x), y(_y), toward(_toward) {}
};

/* global variables */
map<int, io_data_t*> client_datas;
set<Tank*> tanks;
set<Bullet*> bullets;
vector<Game *> games;

/* function declaration */
void * bullet_fly(void *arg);
void remove_bullet(int x, int y);
void remove_tank(int x, int y);
Bullet * get_bullet(int y, int x);
int handle_recv(int epfd, int connfd);
void make_games();
Game * get_empty_game();
Game * get_client_game(int connfd);
Bullet * get_bullet(int y, int x);
void setnonblocking(int sock);
int make_server_socket();

/* class definition */
class Game {
public:
    set<int> clients;
    int GAME_MAP[MAXY][MAXX] = {};
    Game() {
        clients.clear();
        initMap();
    }
    void initMap() {
        for(int i = 0; i < MAXY; i++) {
            for(int j = 0; j < MAXX; j++) {
                if(game_map[i][j] == ' ') setEmpty(i, j);
                else setWall(i, j);
            }
        }
    }
    void setTank(int y, int x) { GAME_MAP[y][x] = TANK; }
    void setEmpty(int y, int x) { GAME_MAP[y][x] = EMPTY; }
    void setBullet(int y, int x) { GAME_MAP[y][x] = BULLET; }
    void setWall(int y, int x) { GAME_MAP[y][x] = WALL; }
    bool isTank(int y, int x) { return GAME_MAP[y][x] == TANK; }
    bool isWall(int y, int x) { return GAME_MAP[y][x] == WALL; }
    bool isEmpty(int y, int x) { return GAME_MAP[y][x] == EMPTY; }
    bool isBullet(int y, int x) { return GAME_MAP[y][x] == BULLET; }
};

class Bullet { // the bullet that a tank shoots
public:
    int x, y;
    char mark;
    bool exist;
    Game *game;
    Bullet(io_data_t *io, int _x, int _y, char toward) {
        pthread_t bull_id;
        pthread_attr_t attr;
        game = io->game; x = _x; y = _y; mark = '*'; exist = true;
        if(toward == 'u') y--;
        else if(toward == 'd') y++;
        else if(toward == 'l') x--;
        else if(toward == 'r') x++;
        game->GAME_MAP[y][x] = BULLET;

        bullets.insert(this);
        bullet_info bi(io, x, y, toward);
        pthread_attr_init(&attr); /*初始化,得到默认的属性值*/
        pthread_create(&bull_id, &attr, bullet_fly, (bullet_info *)&bi);
        pthread_join(bull_id, NULL);
    }
};

class Tank {
public:
    int x, y;
    char mark, toward; //u-up, d-down, l-left, r-right
    bool live;
    Game *game;

    Tank(Game *_game) {
        game = _game;
        x = rand() % (72-6) + 6;
        y = rand() % (20-4) + 4;
        while(game->GAME_MAP[y][x] != EMPTY) {
            x = rand() % (72-6) + 6;
            y = rand() % (20-4) + 4;
        }
        mark = 'v'; toward = 'd';
        tanks.insert(this);
        game->GAME_MAP[y][x] = TANK;
    }

    void set_toward(char move_toward) {
        if(move_toward == 'l') { this->toward = 'l'; this->mark = '<'; }
        if(move_toward == 'r') { this->toward = 'r'; this->mark = '>'; }
        if(move_toward == 'u') { this->toward = 'u'; this->mark = '^'; }
        if(move_toward == 'd') { this->toward = 'd'; this->mark = 'v'; }
    }

    bool move(char key) {
        if(key == 'w') {
            set_toward('u');
            if(canMove(y-1, x)) {
                game->setEmpty(y, x);
                y--;
                game->setTank(y, x);
                return true;
            }
            return false;
        }
        else if(key == 'a') {
            set_toward('l');
            if(canMove(y, x-1)) {
                game->setEmpty(y, x);
                x--;
                game->setTank(y, x);
                return true;
            }
            return false;
        }
        else if(key == 's') {
            set_toward('d');
            if(canMove(y+1, x)) {
                game->setEmpty(y, x);
                y++;
                game->setTank(y, x);
                return true;
            }
            return false;
        }
        else if(key == 'd') {
            set_toward('r');
            if(canMove(y, x+1)) {
                game->setEmpty(y, x);
                x++;
                game->setTank(y, x);
                return true;
            }
            return false;
        }
    }

    bool canMove(int y, int x) {
        return (1 <= y && y <= 22 && 1 <= x && x <= 78 && game->GAME_MAP[y][x] == EMPTY);
    }
};

int main() {
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds;
    ssize_t n;
    char line[MAXLINE];
    srand((unsigned int)time(NULL));

    socklen_t clientaddr_len;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;

    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1) { perror("create socket failure!"); exit(-1); }

    //准备服务器地址
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &serveraddr.sin_addr);

    //绑定
    int res = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(res==-1) { perror("bind failed"); exit(-1); }
    else { printf("bind success\n"); }

    //监听
    if(listen(listenfd, 100)==-1) { perror("listen failed\n"); exit(-1); }
    printf("listen success\n");

    //声明epoll_event结构体的变量, ev用于注册事件, events数组用于回传要处理的事件
    struct epoll_event ev, events[1000];

    //生成用于处理epoll专用的文件描述符, 参数256可随意，现在该参数的作用被遗弃。
    epfd = epoll_create(1024);

    ev.data.fd = listenfd;          //设置epoll管理的socket，
    ev.events = EPOLLIN | EPOLLET;  //设置要处理的事件类型为可读事件，并且工作方式为边沿触发
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);    //注册epoll事件

    // the main loop
    for( ; ; ) {
        nfds = epoll_wait(epfd, events, 1000, 10000); //等待epoll事件的发生
        if(nfds == 0) cout << "Waiting for epoll requests...\n";
        for(int i = 0; i < nfds; i++) {             //处理所发生的所有事件
            if(events[i].data.fd == listenfd) {     //监听事件
//                connfd = accept_client(listenfd, hostname);
                int connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddr_len);
                if(connfd < 0) { perror("connfd < 0"); exit(1); }
                char *hostname = inet_ntoa(clientaddr.sin_addr);
                cout << "connect from " << hostname << endl;

                ev.data.fd = connfd; //设置epoll管理的socket
                ev.events = EPOLLIN | EPOLLET; //设置要处理的事件类型为可读事件，并且工作方式为边沿触发
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev); //注册epoll事件

//                io_data_t data = {connfd, &ev, NULL, NULL};
                io_data_t *p = new io_data_t(connfd, epfd, &ev, NULL, NULL);
                client_datas.insert(make_pair(connfd, p));
                make_games();

            }
            else {
                if( events[i].events & EPOLLIN ) {     //读事件
                    printf("EPOLLIN\n");
                    handle_recv(listenfd, events[i].data.fd);
                }
            }
        }
    }
    return 0;
}

int handle_recv(int epfd, int connfd) {
    char recv_from_client[MAXLINE] = {};
    int nread = read(connfd, recv_from_client, MAXLINE);//读取客户端socket流

    if (nread <= 0) {
        printf("client close the connection\n");
        for(int i = 0; i < games.size(); i++) {
            if(games[i]->clients.count(connfd) != 0) {
                auto it = games[i]->clients.find(connfd);
                games[i]->clients.erase(it);
            }
        }
        client_datas.erase(connfd);
        close(connfd);
        return -1;
    }

    cout << "receive: " << recv_from_client << endl;

    /* allocate the client to an available game room */
    if(strcmp(recv_from_client, "start") == 0) {
        io_data_t *data = client_datas[connfd];

        char send_to_client[MAXLINE] = {};
        Game *game = get_empty_game();

        bool flag = false;
        if(game->clients.size() == 0) { game->clients.insert(connfd); flag = false; }
        else {
            if(game->clients.count(connfd) == 1) flag = false;
            else {
                game->clients.insert(connfd);
                flag = true;
            }
        }

        if(data->tank == NULL) {
            data->tank = new Tank(game);
        }
        data->game = game;

        if(flag) {
            game->initMap();
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                game->setTank(client_datas[(*it)]->tank->y, client_datas[(*it)]->tank->x);
            }
            strcat(send_to_client, "begin\n");
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                char t[50] = {};
                sprintf(t, "tank %d %d %c\n",
                            client_datas[(*it)]->tank->x,
                            client_datas[(*it)]->tank->y,
                            client_datas[(*it)]->tank->mark);
                strcat(send_to_client, t);
            }
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                write((*it), send_to_client, strlen(send_to_client));
                epoll_ctl(epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
            }
        }
        else {
            strcpy(send_to_client, "wait");
            write(connfd, send_to_client, strlen(send_to_client));
            epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, client_datas[connfd]->ev);
        }
        cout << send_to_client << endl;
    }
    else {
        Game *game = client_datas[connfd]->game;
        int oldx = client_datas[connfd]->tank->x, oldy = client_datas[connfd]->tank->y;

        /* press 'w' —— 向上走 */
        if(strcmp(recv_from_client, "w") == 0) {
            char send_to_client[MAXLINE] = {};
            strcat(send_to_client, "movetank\n");
            bool moveres = client_datas[connfd]->tank->move('w');
            char tmp[20] = {};
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                io_data_t *temp_data = client_datas[(*it)];

                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                        temp_data->tank->y, temp_data->tank->mark);
                strcat(send_to_client, tmp);
            }
            if(moveres) {
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d", oldx, oldy);
                strcat(send_to_client, tmp);
            }
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                write((*it), send_to_client, strlen(send_to_client));
                epoll_ctl(epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
            }
        }

        /* press 's' —— 向下走 */
        else if(strcmp(recv_from_client, "s") == 0) {
            char send_to_client[MAXLINE] = {};
            strcat(send_to_client, "movetank\n");
            bool moveres = client_datas[connfd]->tank->move('s');
            char tmp[20] = {};
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                int fd_it = (*it);
                io_data_t *temp_data = client_datas[fd_it];
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                        temp_data->tank->y, temp_data->tank->mark);
                strcat(send_to_client, tmp);
            }
            if(moveres) {
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d", oldx, oldy);
                strcat(send_to_client, tmp);
            }
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                write((*it), send_to_client, strlen(send_to_client));
                epoll_ctl(epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
            }
        }

        /* press 'a' —— 向左走 */
        else if(strcmp(recv_from_client, "a") == 0) {
            char send_to_client[MAXLINE] = {};
            strcat(send_to_client, "movetank\n");
            bool moveres = client_datas[connfd]->tank->move('a');
            char tmp[20] = {};
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                int fd_it = (*it);
                io_data_t *temp_data = client_datas[fd_it];
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                        temp_data->tank->y, temp_data->tank->mark);
                strcat(send_to_client, tmp);
            }
            if(moveres) {
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d", oldx, oldy);
                strcat(send_to_client, tmp);
            }
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                write((*it), send_to_client, strlen(send_to_client));
                epoll_ctl(epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
            }
        }

        /* press 'd' —— 向右走 */
        else if(strcmp(recv_from_client, "d") == 0) {
            char send_to_client[MAXLINE] = {};
            strcat(send_to_client, "movetank\n");
            bool moveres = client_datas[connfd]->tank->move('d');
            char tmp[20] = {};
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                int fd_it = (*it);
                io_data_t *temp_data = client_datas[fd_it];
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                        temp_data->tank->y, temp_data->tank->mark);
                strcat(send_to_client, tmp);
            }
            if(moveres) {
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d", oldx, oldy);
                strcat(send_to_client, tmp);
            }
            for(auto it = game->clients.begin(); it != game->clients.end(); it++) {
                write((*it), send_to_client, strlen(send_to_client));
                epoll_ctl(epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
            }
        }

        /* press 'j' —— 射击 */
        else if(strcmp(recv_from_client, "j") == 0) {
            io_data_t *data = client_datas[connfd];
            Bullet *b = new Bullet(data, data->tank->x, data->tank->y, data->tank->toward);
        }
    }
    return 0;
}

void * bullet_fly(void *arg) {
    bullet_info *bi = (bullet_info *)arg;
    //bi->io->game; bi->io->ev; bi->io->fd; bi->io->tank;
    char toward = bi->toward;
    Bullet *t = get_bullet(bi->y, bi->x);

    /* 子弹朝右打 */
    if(toward == 'd') {
        while(t->exist && t->y <= 22) {
            bi->io->game->setEmpty(t->y, t->x);
            /* 子弹前进一格 */
            if(bi->io->game->isEmpty(t->y+1, t->x)) { // 前面是空的
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "bullet %d %d\n", t->x, t->y+1);
                strcat(to_send, tmp);
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\n", t->x, t->y);
                strcat(to_send, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *td = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                    strcat(to_send, tmp);
                }
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("%s\n", to_send);
                bi->io->game->setBullet(t->y+1, t->x);
                t->y = t->y+1;
            }
            else if(bi->io->game->isWall(t->y+1, t->x)) { // 前面是墙
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：子弹和墙都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x, t->y+1, t->x, t->y);
                strcat(to_send, tmp);
                /* 消息：更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("%s\n", to_send);
                /* 更新后端逻辑： 子弹和墙所在位置设空，删除子弹 */
                bi->io->game->setEmpty(t->y+1, t->x);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isBullet(t->y+1, t->x)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：两颗子弹都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x, t->y+1, t->x, t->y);
                strcat(to_send, tmp);
                /* 更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                bi->io->game->setEmpty(t->y+1, t->x);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y+1, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isTank(t->y+1, t->x)) {
                char to_win[100] = {}, to_lose[100] = {}, tmp[50] = {};
                strcat(to_win, "win\n"); strcat(to_lose, "lose\n");
                /* 消息：子弹和坦克都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x, t->y+1, t->x, t->y);
                strcat(to_win, tmp); strcat(to_lose, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // the winner's tank should be shown
                        io_data_t *td = client_datas[(*it)];
                        memset(tmp, 0, sizeof(tmp));
                        sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                        strcat(to_win, tmp); strcat(to_lose, tmp);
                    }
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // send win msg to the shooter
                        write((*it), to_win, strlen(to_win));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                    else { // send lose msg to the loser
                        write((*it), to_lose, strlen(to_lose));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                }
                printf("to_win: %s\n", to_win);
                printf("to_lose: %s\n", to_lose);
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                remove_tank(t->y+1, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            msleep(50);
        }
    }
    else if(toward == 'u') {
        while(t->exist && t->y >= 1) {
            bi->io->game->setEmpty(t->y, t->x);
            /* 子弹前进一格 */
            if(bi->io->game->isEmpty(t->y-1, t->x)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "bullet %d %d\n", t->x, t->y-1);
                strcat(to_send, tmp);
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\n", t->x, t->y);
                strcat(to_send, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *td = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                    strcat(to_send, tmp);
                }
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("send: %s\n", to_send);
                bi->io->game->setBullet(t->y-1, t->x);
                t->y = t->y-1;
            }
            else if(bi->io->game->isWall(t->y-1, t->x)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：子弹和墙都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x, t->y-1, t->x, t->y);
                strcat(to_send, tmp);
                /* 消息：更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("%s\n", to_send);
                /* 更新后端逻辑： 子弹和墙所在位置设空，删除子弹 */
                bi->io->game->setEmpty(t->y-1, t->x);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isBullet(t->y-1, t->x)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：两颗子弹都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x, t->y-1, t->x, t->y);
                strcat(to_send, tmp);
                /* 更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                bi->io->game->setEmpty(t->y-1, t->x);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y-1, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isTank(t->y-1, t->x)) {
                char to_win[100] = {}, to_lose[100] = {}, tmp[50] = {};
                strcat(to_win, "win\n"); strcat(to_lose, "lose\n");
                /* 消息：子弹和坦克都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x, t->y-1, t->x, t->y);
                strcat(to_win, tmp); strcat(to_lose, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // the winner's tank should be shown
                        io_data_t *td = client_datas[(*it)];
                        memset(tmp, 0, sizeof(tmp));
                        sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                        strcat(to_win, tmp); strcat(to_lose, tmp);
                    }
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // send win msg to the shooter
                        write((*it), to_win, strlen(to_win));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                    else { // send lose msg to the loser
                        write((*it), to_lose, strlen(to_lose));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                }
                printf("to_win: %s\n", to_win);
                printf("to_lose: %s\n", to_lose);
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                remove_tank(t->y-1, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            msleep(50);
        }
    }
    else if(toward == 'l') {
        while(t->exist && t->x >= 1) {
            bi->io->game->setEmpty(t->y, t->x);
            /* 子弹前进一格 */
            if(bi->io->game->isEmpty(t->y, t->x-1)) {
                char to_send[100] = {}, tmp[50] = {}; strcat(to_send, "shoot\n");
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "bullet %d %d\n", t->x-1, t->y);
                strcat(to_send, tmp);
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\n", t->x, t->y);
                strcat(to_send, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *td = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                    strcat(to_send, tmp);
                }
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("%s\n", to_send);
                bi->io->game->setBullet(t->y, t->x-1);
                t->x = t->x-1;
            }
            else if(bi->io->game->isWall(t->y, t->x-1)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：子弹和墙都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x-1, t->y, t->x, t->y);
                strcat(to_send, tmp);
                /* 消息：更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("%s\n", to_send);
                /* 更新后端逻辑： 子弹和墙所在位置设空，删除子弹 */
                bi->io->game->setEmpty(t->y, t->x-1);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isBullet(t->y, t->x-1)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：两颗子弹都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x-1, t->y, t->x, t->y);
                strcat(to_send, tmp);
                /* 更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                bi->io->game->setEmpty(t->y, t->x-1);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y, t->x-1);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isTank(t->y, t->x-1)) {
                char to_win[100] = {}, to_lose[100] = {}, tmp[50] = {};
                strcat(to_win, "win\n"); strcat(to_lose, "lose\n");
                /* 消息：子弹和坦克都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x-1, t->y, t->x, t->y);
                strcat(to_win, tmp); strcat(to_lose, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // the winner's tank should be shown
                        io_data_t *td = client_datas[(*it)];
                        memset(tmp, 0, sizeof(tmp));
                        sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                        strcat(to_win, tmp); strcat(to_lose, tmp);
                    }
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // send win msg to the shooter
                        write((*it), to_win, strlen(to_win));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                    else { // send lose msg to the loser
                        write((*it), to_lose, strlen(to_lose));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                }
                printf("to_win: %s\n", to_win);
                printf("to_lose: %s\n", to_lose);
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                remove_tank(t->y, t->x-1);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            msleep(50);
        }
    }
    else if(toward == 'r') {
        while(t->exist && t->x <= 78) {
            bi->io->game->setEmpty(t->y, t->x);
            /* 子弹前进一格 */
            if(bi->io->game->isEmpty(t->y, t->x+1)) {
                char to_send[100] = {}, tmp[50] = {}; strcat(to_send, "shoot\n");
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "bullet %d %d\n", t->x+1, t->y);
                strcat(to_send, tmp);
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\n", t->x, t->y);
                strcat(to_send, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *td = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                    strcat(to_send, tmp);
                }
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("%s\n", to_send);
                bi->io->game->setBullet(t->y, t->x+1);
                t->x = t->x+1;
            }
            else if(bi->io->game->isWall(t->y, t->x+1)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：子弹和墙都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x+1, t->y, t->x, t->y);
                strcat(to_send, tmp);
                /* 消息：更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                printf("%s\n", to_send);
                /* 更新后端逻辑： 子弹和墙所在位置设空，删除子弹 */
                bi->io->game->setEmpty(t->y, t->x+1);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isBullet(t->y, t->x+1)) {
                char to_send[100] = {}, tmp[50] = {};
                strcat(to_send, "shoot\n");
                /* 消息：两颗子弹都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x+1, t->y, t->x, t->y);
                strcat(to_send, tmp);
                /* 更新坦克位置 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    io_data_t *temp_data = client_datas[(*it)];
                    memset(tmp, 0, sizeof(tmp));
                    sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                            temp_data->tank->y, temp_data->tank->mark);
                    strcat(to_send, tmp);
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    write((*it), to_send, strlen(to_send));
                    epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                }
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                bi->io->game->setEmpty(t->y, t->x+1);
                bi->io->game->setEmpty(t->y, t->x);
                remove_bullet(t->y, t->x+1);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            else if(bi->io->game->isTank(t->y, t->x+1)) {
                char to_win[100] = {}, to_lose[100] = {}, tmp[50] = {};
                strcat(to_win, "win\n"); strcat(to_lose, "lose\n");
                /* 消息：子弹和坦克都消失 */
                memset(tmp, 0, sizeof(tmp));
                sprintf(tmp, "empty %d %d\nempty %d %d\n", t->x+1, t->y, t->x, t->y);
                strcat(to_win, tmp); strcat(to_lose, tmp);
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // the winner's tank should be shown
                        io_data_t *td = client_datas[(*it)];
                        memset(tmp, 0, sizeof(tmp));
                        sprintf(tmp, "tank %d %d %c\n", td->tank->x, td->tank->y, td->tank->mark);
                        strcat(to_win, tmp); strcat(to_lose, tmp);
                    }
                }
                /* 发送消息 */
                for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
                    if((*it) == bi->io->fd) { // send win msg to the shooter
                        write((*it), to_win, strlen(to_win));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                    else { // send lose msg to the loser
                        write((*it), to_lose, strlen(to_lose));
                        epoll_ctl(client_datas[(*it)]->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
                    }
                }
                printf("to_win: %s\n", to_win);
                printf("to_lose: %s\n", to_lose);
                /* 更新后端逻辑： 两颗子弹所在位置设空，删除两颗子弹 */
                remove_tank(t->y, t->x+1);
                remove_bullet(t->y, t->x);
                t->exist = false;
            }
            msleep(50);
        }
    }
    if(t->exist) {
        char to_send[100] = {};
        strcat(to_send, "shoot\n");
        char tmp[50] = {};
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "empty %d %d\n", t->x, t->y);
        strcat(to_send, tmp);
        for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
            io_data_t *temp_data = client_datas[(*it)];
            memset(tmp, 0, sizeof(tmp));
            sprintf(tmp, "tank %d %d %c\n", temp_data->tank->x,
                    temp_data->tank->y, temp_data->tank->mark);
            strcat(to_send, tmp);
        }
        for(auto it = bi->io->game->clients.begin(); it != bi->io->game->clients.end(); it++) {
            write((*it), to_send, strlen(to_send));
            epoll_ctl(bi->io->epfd, EPOLL_CTL_MOD, (*it), client_datas[(*it)]->ev);
        }
    }
    return NULL;
}

Bullet * get_bullet(int y, int x) {
    for(auto it = bullets.begin(); it != bullets.end(); it++) {
        if((*it)->x == x && (*it)->y == y) {
            return (*it);
        }
    }
}

void remove_bullet(int x, int y) {
    for(auto it = bullets.begin(); it != bullets.end(); ) {
        if((*it)->x == x && (*it)->y == y)
            bullets.erase(it);
        else
            it++;
    }
}

void remove_tank(int x, int y) {
    for(auto it = tanks.begin(); it != tanks.end(); ) {
        if((*it)->x == x && (*it)->y == y)
            tanks.erase(it);
        else
            it++;
    }
}

void setnonblocking(int sock) {
    int opts;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0) {
        perror("fcntl(sock, GETFL)");
        exit(1);
    }

    opts = opts | O_NONBLOCK;
    if(fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

int make_server_socket() {
    struct sockaddr_in serveraddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1) {
       perror("create socket failure!\n");
       exit(-1);
    }

    //准备服务器地址
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &serveraddr.sin_addr);

    //绑定
    int res = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(res == -1) {
         perror("bind failed\n");
         exit(-1);
    }
    else {
         printf("bind success\n");
    }

    //监听
    if(listen(sockfd, 100)==-1) {
        perror("listen failed\n");
        exit(-1);
    }
    printf("listen success\n");
    return 0;
}

int accept_client(int listenfd, char *hostname) {
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len; clientaddr_len = sizeof(clientaddr);
    int connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddr_len);
    if(connfd < 0) { perror("connfd < 0"); return -1; }
    hostname = inet_ntoa(clientaddr.sin_addr);
    return 0;
}

void make_games() {
    for(int i = 0; i < 500; i++) {
        Game *game = new Game();
        games.push_back(game);
    }
}

Game * get_empty_game() {
    for(int i = 0; i < games.size(); i++) {
        if(games[i]->clients.size() <= 2)
            return games[i];
    }
}

Game * get_client_game(int connfd) {
    for(int i = 0; i < games.size(); i++) {
        if(games[i]->clients.count(connfd) != 0)
            return games[i];
    }
}
