#include <vector>
#include <time.h>
#include <string>
//#include <winsock.h>
#include <thread>
#include <mutex>

#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include <QString>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int
#define SOCKET_ERROR -1

namespace ServerCfg
{
    const char LOCALHOST[] = "127.0.0.1";
    const u_short PORT = 8081;
    const int PROTOCOL = IPPROTO_TCP;
    const int BACKLOG = 1;
    const int BUFF_SIZE = 1024;
    const short MAX_ATTEMPTS = 2;
    const int BAN_TIME = 5 * 60;
    const char BANSTATUS[][10] = { "ERROR:111", "ERROR:112" };
}

struct IpBan
{
    char ip[16];
    time_t t;
};

std::vector<IpBan> banList;

namespace ServerThreads
{
    const char USERPASS[] = "PaSs4332";
    const char ROOTPASS[] = "root";
    const char USERLOGIN[] = "user";
    const char ROOTLOGIN[] = "root";
    const char GETBANLIST[] = "getbanlist";
    const char UNBAN[] = "unban";
    struct ServerThread
    {
        //HANDLE Thread;
        //DWORD ThreadID;
        std::thread *Thread;
    };

    std::vector<ServerThreads::ServerThread*> clientThreads;
    std::vector<SOCKET> clientSockets;
    std::vector<char*> clientIPs;
    std::mutex g_lock;

    bool get_answear(char *recvbuf, int &bytesRecv, int clientNumber)
    {
        bytesRecv = recv(clientSockets[clientNumber], recvbuf, ServerCfg::BUFF_SIZE, 0);
        if (bytesRecv == 0)// || bytesRecv == WSAECONNRESET)
        {
            printf("[SERVER] Client #%i disconnected.\n", clientNumber + 1);
            return false;
        }
        if (bytesRecv < 0)
        {
            printf("[SERVER] Connection lost with client #%i.\n", clientNumber + 1);
            return false;
        }
        return true;
    }

    bool send_message(int &bytesSent, int clientNumber, char *sendbuf, const char *msg)
    {
        g_lock.lock();
        strcpy(sendbuf, msg);
        bytesSent = send(clientSockets[clientNumber], sendbuf, strlen(sendbuf), 0);
        g_lock.unlock();
        return true;
    }

    int get_login(int &bytesRecv, int &bytesSent, int clientNumber, char *recvbuf, char *sendbuf,bool &is_root, bool &login_getted)
    {
        if (!get_answear(recvbuf, bytesRecv, clientNumber))
                return -1;

        g_lock.lock();
        printf("[Client #%i] Bytes recv: %ld | [MSG: %s]\n", clientNumber + 1, bytesRecv, recvbuf);
        g_lock.unlock();

        if (strcmp(USERLOGIN, recvbuf) != 0)
        {
            if (strcmp(ROOTLOGIN, recvbuf) == 0)
            {
                is_root = true;
                login_getted = true;
                send_message(bytesSent, clientNumber, sendbuf, ROOTLOGIN);
                printf("[Client #%i] Root loggined.\n", clientNumber + 1, bytesSent, sendbuf);
            }
            else
            {
                login_getted = false;
                send_message(bytesSent, clientNumber, sendbuf, "Incorrect login.");
                printf("[Client #%i] Incorrect login.\n", clientNumber + 1, bytesSent, sendbuf);
                return 1;
            }
        }
        else
        {
            login_getted = true;
            send_message(bytesSent, clientNumber, sendbuf, USERLOGIN);
            printf("[Client #%i] User loggined.\n", clientNumber + 1, bytesSent, sendbuf);
        }

        return 0;
    }

    int get_pass(short &attempt, int &bytesRecv, int &bytesSent, int clientNumber, char *recvbuf, char *sendbuf, char *buff, bool &is_root, bool &pass_correct)
    {
        if (!get_answear(recvbuf, bytesRecv, clientNumber))
                return -1;

        g_lock.lock();
        printf("[Client #%i] Bytes recv: %ld | [MSG: %s]\n", clientNumber + 1, bytesRecv, recvbuf);

        pass_correct = true;
        if (is_root)
        {
            if (strcmp(ROOTPASS, recvbuf) != 0)
                pass_correct = false;
        }
        else
        {
            if (strcmp(USERPASS, recvbuf) != 0)
                pass_correct = false;
        }

        if (!pass_correct)
        {
            attempt--;
            if (!attempt)
            {
                strcpy(sendbuf, ServerCfg::BANSTATUS[0]);
                bytesSent = send(clientSockets[clientNumber], sendbuf, strlen(sendbuf), 0);
                printf("[Client #%i] Have been banned.\n", clientNumber + 1, bytesSent, sendbuf);
                /* =================ADD BANLIST==================== */
                IpBan iBan;

                strcpy(iBan.ip, clientIPs[clientNumber]);
                iBan.t = time(NULL);

                banList.push_back(iBan);
				g_lock.unlock();
                /* ================================================ */
                return -1;
            }
            strcpy(sendbuf, "Password is incorrect! You have ");
            //strcat(sendbuf, itoa(attempt, buff, 10));
            strcat(sendbuf, " attempts yet.");
        }
        else
        {
            strcpy(sendbuf, "Password is correct!");
        }
        bytesSent = send(clientSockets[clientNumber], sendbuf, strlen(sendbuf), 0);
        printf("[Client #%i] Bytes sent: %ld | [MSG: %s]\n", clientNumber + 1, bytesSent, sendbuf);
        g_lock.unlock();
        return 0;
    }

    bool get_banlist(int &bytesSent, int clientNumber)
    {
        char str[1000] = "";
        char buf[128];
        strcat(str, "\n");

        for (int i = 0; i < banList.size(); i++)
        {
            time_t t = time(NULL);
            strcat(str, QString::number(i).toStdString().c_str());
            strcat(str, ") IP: ");
            strcat(str, banList[i].ip);
            strcat(str, "\t pardon: ");
            strcat(str, QString::number(t - banList[i].t).toStdString().c_str());//itoa((t - banList[i].t), buf, 10));
            strcat(str, "\n");
        }
        strcat(str, "\0");

        if (strlen(str) < 2)
            strcpy(str, "BanList empty.");

        bytesSent = send(clientSockets[clientNumber], str, strlen(str), 0);
        printf("[SERVER] BanList sended to Client #%i.\n", clientNumber + 1);
        return true;
    }

    bool is_numeric(const char *str)
    {
        for(int i = 0; i < strlen(str); i++)
        {
            if (!isdigit(str[i]))
                return false;
        }
        return true;
    }

    bool unban_ip(int &bytesSent, int clientNumber, char *recvbuf, char *sendbuf, std::string str)
    {
        try
        {
            g_lock.lock();
            if (str.substr(strlen(UNBAN) + 1).compare("all") == 0)
            {
                banList.clear();
                banList.shrink_to_fit();

                strcpy(sendbuf, "All IP's are unbanned.");
                bytesSent = send(clientSockets[clientNumber], sendbuf, strlen(sendbuf), 0);
                printf("[Client #%i] Bytes sent: %ld | [MSG: %s]\n", clientNumber + 1, bytesSent, sendbuf);
            }
            else
            {
                int index(-1);
                if (is_numeric(str.substr(strlen(UNBAN) + 1, str.size()).c_str()))
                    index = atoi(str.substr(strlen(UNBAN) + 1, str.size()).c_str());

                if (index < banList.size() && index >= 0)
                {
                    banList.erase(banList.begin() + index);

                    strcpy(sendbuf, "IP unbanned.");
                    bytesSent = send(clientSockets[clientNumber], sendbuf, strlen(sendbuf), 0);
                    printf("[Client #%i] Bytes sent: %ld | [MSG: %s]\n", clientNumber + 1, bytesSent, sendbuf);
                }
                else
                {
                    strcpy(sendbuf, "Incorrect list number.");
                    bytesSent = send(clientSockets[clientNumber], sendbuf, strlen(sendbuf), 0);
                    printf("[Client #%i] Bytes sent: %ld | [MSG: %s]\n", clientNumber + 1, bytesSent, sendbuf);
                }
            }
            g_lock.unlock();
        }
        catch(...)
        {
            return false;
        }
        return true;
    }

    int user_iteract(int &bytesRecv, int &bytesSent, int clientNumber, char *recvbuf, char *sendbuf, bool is_root)
    {
        bool is_command(false);


        if (!get_answear(recvbuf, bytesRecv, clientNumber))
            return -1;

        std::string str(recvbuf);

        g_lock.lock();
        printf("[Client #%i] Bytes recv: %ld | [MSG: %s]\n", clientNumber + 1, bytesRecv, recvbuf);
        g_lock.unlock();

        if (is_root)
        {
            if (strcmp(recvbuf, GETBANLIST) == 0)
            {
                is_command = true;
                get_banlist(bytesSent, clientNumber);
            }


            if (strcmp(str.substr(0, strlen(UNBAN)).c_str(), UNBAN) == 0)
            {
                is_command = unban_ip(bytesSent, clientNumber, recvbuf, sendbuf, str);
            }
        }

        if (!is_command)
        {
            send_message(bytesSent, clientNumber, sendbuf, "Command incorrect!");
            g_lock.lock();
            printf("[Client #%i] Bytes sent: %ld | [MSG: %s]\n", clientNumber + 1, bytesSent, sendbuf);
            g_lock.unlock();
        }
        return 0;
    }

    bool ThreadRoutine_(int clientNumber)
    {
        int bytesSent;
        int bytesRecv = SOCKET_ERROR;

        char buff[ServerCfg::BUFF_SIZE];
        char sendbuf[ServerCfg::BUFF_SIZE];

        short attempt = ServerCfg::MAX_ATTEMPTS;

        bool login_getted(false);
        bool pass_correct(false);

        bool is_root(false);

        while(true)
        {
            char recvbuf[ServerCfg::BUFF_SIZE] = "";
            if (!login_getted)
            {
                int q = get_login(bytesRecv, bytesSent, clientNumber, recvbuf, sendbuf, is_root, login_getted);

                if (q == 1) continue;
                if (q == -1) break;
            }
            if (login_getted && !pass_correct)
            {
                int q = get_pass(attempt, bytesRecv, bytesSent, clientNumber, recvbuf, sendbuf, buff, is_root, pass_correct);

                if (q == 1) continue;
                if (q == -1) break;
            }
            if (login_getted && pass_correct)
            {
                int q = user_iteract(bytesRecv, bytesSent, clientNumber, recvbuf, sendbuf, is_root);

                if (q == 1) continue;
                if (q == -1) break;
            }
        }
        shutdown(clientSockets[clientNumber], 0);
        return true;
    }

    bool CreateClientThread(ServerThread **sThread, int clientNumber)
    {
        (*sThread)->Thread = new std::thread(ThreadRoutine_, clientNumber);
        clientThreads.push_back(*sThread);
        return true;
    }

    bool CloseAllThreads()
    {
        for(int i = 0; i < clientThreads.size(); i++)
            delete clientThreads[i]->Thread;
        clientThreads.clear();
        clientThreads.shrink_to_fit();
        clientSockets.clear();
        clientSockets.shrink_to_fit();
        return true;
    }
}

class Server
{
private:
//	WSADATA wsaData;
    int iResult;

    SOCKET m_socket; // создаем сокет
    sockaddr_in service;

    char sendbuf[ServerCfg::BUFF_SIZE];
    int bytesSent;

    void accept_socket(SOCKET &AcceptSocket, sockaddr_in &ClientInfo, int &adr)
    {
        AcceptSocket = SOCKET_ERROR;
            while (AcceptSocket == SOCKET_ERROR)
                AcceptSocket = accept(m_socket, (struct sockaddr* ) &ClientInfo, (socklen_t *) &adr);
    }

    bool ban_analyze(sockaddr_in &ClientInfo)
    {
        for(int i = 0; i < banList.size(); i++)
        {
            //if (strcmp(htonl(ClientInfo.sin_addr), banList[i].ip) == 0)
            if (strcmp(inet_ntoa(ClientInfo.sin_addr), banList[i].ip) == 0)
            {
                time_t cur_t = time(NULL);
                if (cur_t - banList[i].t >= ServerCfg::BAN_TIME)
                {
                    banList.erase (banList.begin() + i);
                    printf( "[SERVER] IP %s unbanned.\n", inet_ntoa(ClientInfo.sin_addr));
                    //is_banned = false;
                    return false;
                }
                else
                {
                    //is_banned = true;
                    return true;
                }
            }
        }
        return false;
    }

    void connect_user(SOCKET &AcceptSocket, sockaddr_in &ClientInfo, int count)
    {
        ServerThreads::ServerThread *thread = new ServerThreads::ServerThread();

        printf( "[SERVER] Client #%i connected. %s\n", count + 1, inet_ntoa(ClientInfo.sin_addr));
        strcpy(sendbuf, "ACCEPT");

        bytesSent = send(AcceptSocket, sendbuf, strlen(sendbuf), 0);

        ServerThreads::clientIPs.push_back(inet_ntoa(ClientInfo.sin_addr));

        ServerThreads::clientSockets.push_back(AcceptSocket);
        ServerThreads::CreateClientThread(&thread, count);
    }

    void disconnect_user(SOCKET &AcceptSocket, sockaddr_in &ClientInfo, int count)
    {
        printf( "[SERVER] Client #%i not connected. IP: %s in banlist.\n", count + 1, inet_ntoa(ClientInfo.sin_addr));
        strcpy(sendbuf, ServerCfg::BANSTATUS[1]);

        bytesSent = send(AcceptSocket, sendbuf, strlen(sendbuf), 0);
        //closesocket(bytesSent);
    }

public:
    Server()
    {

    }

    ~Server()
    {
        ServerThreads::CloseAllThreads();
        //CloseHandle(ServerThreads::ghMutex);
        shutdown(m_socket, 1);
        //WSACleanup();
    }

    int init()
    {
        /* ================================================================= */
        // »Ќ»÷»јЋ»«ј÷»я —ќ ≈“ј
        //WSADATA wsaData; // содержит информацию о реализации сокетов Windows
        //iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        // MAKEWORD(2,2) данной функции запрашивает версию WinSock системы и
        // устанавливает ее как наивысшую допустимую версию сокетов Windows,
        // котора¤ может использоватьс¤.
/*
        if (iResult != NO_ERROR) // ¬ы¤вление ошибок
        {
            printf("[ERROR: WSADATA] Error at WSAStartup()\n");
            WSACleanup();
            system("pause");
            return -1;
        }
*/
        //SOCKET m_socket; // создаем сокет
        m_socket = socket(AF_INET, SOCK_STREAM, ServerCfg::PROTOCOL);
        // ¬ качестве параметров используютс¤ семейство интернет-адресов (IP),
        // потоковые сокеты и протокол TCP/IP.

        if (m_socket < 0)//== INVALID_SOCKET) // ¬ы¤вление ошибок
        {
            printf("[ERROR: SOCKET] Error at socket():");// %ld\n", WSAGetLastError());
            // WSAGetLastError возвращает номер последней возникнувшей ошибки
            //WSACleanup();
            //system("pause");
            return -1;
        }
        return 0;
    }

    int try_open_server()
    {
        // service содержит информаци¤ о семействе адресов,
        // IP адрес и номер порта
        service.sin_family = AF_INET; // семейство адресов »нтернет
        service.sin_addr.s_addr = INADDR_ANY;//inet_addr(ServerCfg::LOCALHOST); // локальный IP
        service.sin_port = htons(ServerCfg::PORT); // номер порта

        // ¬ы¤вление ошибок
        if (bind(m_socket, (struct sockaddr*) &service, sizeof(service)) == SOCKET_ERROR)
        {
            printf("[ERROR: SOCKADDR] bind() failed.\n");
            //closesocket(m_socket);
            //WSACleanup();
            //system("pause");
            return -1;
        }
        /* ================================================================= */
        // ѕ–ќ—Ћ”Ў»¬јЌ»≈ —ќ ≈“ј ƒЋя ¬’ќƒяў≈√ќ —ќ≈ƒ»Ќ≈Ќ»я
        if (listen(m_socket, ServerCfg::BACKLOG) == SOCKET_ERROR)
        {
            printf("[ERROR: LISTEN] Error listening on socket.\n");
            //WSACleanup();
            system("pause");
            return -1;
        }
        return 0;
    }

    int main_func()
    {
        printf("[STATUS] Server started.\n");
        printf("[SERVER] Waiting for a client to connect...\n" );

        time_t t = time(NULL);
        IpBan t_;
        char buff_ [256];

        for(int i = 0; i < 10; i++)
        {
            strcpy(t_.ip, "127.0.1.");
            sprintf(buff_,"%d",i);
            strcat(t_.ip,  buff_);//itoa(i, buff_, 10));
            t_.t = t;
            banList.push_back(t_);
        }

        int count = 0;
        bool is_banned(false);

        while (true)
        {
            is_banned = false;
            SOCKET AcceptSocket;
            sockaddr_in ClientInfo;
            int adr = sizeof(ClientInfo);

            accept_socket(AcceptSocket, ClientInfo, adr);
            /* ======================== */
            is_banned = ban_analyze(ClientInfo);
            /* ======================== */
            if (!is_banned)
            {
                connect_user(AcceptSocket, ClientInfo, count);
            }
            else
            {
                disconnect_user(AcceptSocket, ClientInfo, count);
            }
            count++;
        }
    }
};

int main()
{
    time_t t = time(NULL);
    tm* aTm = localtime(&t);
    printf("%04d/%02d/%02d %02d:%02d:%02d \n",aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);

    Server s;
    s.init();
    if (s.try_open_server() >= 0)
		s.main_func();
    system("pause");
    return 0;
}
