#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string>
#include <cstring>

#include <QTextStream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET int
#define SOCKET_ERROR -1


namespace ServerCfg
{
    const char LOCALHOST[] = "127.0.0.1";
    const u_short PORT = 8081;
    const int PROTOCOL = IPPROTO_TCP;
    const int BACKLOG = 1;
    const int BUFF_SIZE = 1024;
    const char BANSTATUS[][10] = { "ERROR:111", "ERROR:112" };

    const char USERLOGIN[] = "user";
    const char ROOTLOGIN[] = "root";
    const char PASSCORRECT[] = "Password is correct!";
    const char GETBANLIST[] = "getbanlist";
    const char UNBAN[] = "unban";
}

bool first = false;

class Client
{
private:
    /* ================================================================= */
    //WSADATA wsaData; // содержит информацию о реализации сокетов Windows
    int iResult;

    bool have_ip;

    SOCKET m_socket; // создаем сокет
    sockaddr_in service;
    /* ================================================================= */
    int bytesSent;
    int bytesRecv;
    char sendbuf[ServerCfg::BUFF_SIZE];
    char recvbuf[ServerCfg::BUFF_SIZE];

    bool loginned;
    bool pass_correct;
    bool is_root;

    bool getbanlist;
    bool unban;
    bool active_command;
    /* ================================================================= */
    int send_(const char *str)
    {
        return send(m_socket, str, strlen(str), 0);
    }

    int recv_(char *str)
    {
        return recv(m_socket, str, ServerCfg::BUFF_SIZE, 0);
    }

    int send_command()
    {
        //std::cin.clear();
        //std::cin.sync();
        printf("[CLIENT] Command: ");
        //std::cin >> sendbuf;
        //scanf(sendbuf);
        if (!first)
        {
        std::string str;
        std::getline(std::cin, str);
        first = true;
        }

        QTextStream s(stdin);
        QString value = s.readLine();
        //std::cin.getline(sendbuf, 256);

        if (strcmp(value.toStdString().c_str(), "exit") == 0)
        {
            throw 0;
        }
        //std::cin >> sendbuf;
        //std::cin.getline(sendbuf, 1024, '\n');
        bytesSent = send_(value.toStdString().c_str());
        if (bytesSent < 0)
            throw 0;
    }

    int command_viewer()
    {
        send_command();

        bytesRecv = SOCKET_ERROR;
        while (bytesRecv == SOCKET_ERROR)
        {
            char recvbuf[ServerCfg::BUFF_SIZE] = "";
            if (!get_answear(m_socket, recvbuf, bytesRecv))
            {
                throw -1;
            }

            if (is_root)
            {
                if (strcmp(recvbuf, ServerCfg::GETBANLIST) == 0)
                {
                    bytesSent = send_(ServerCfg::GETBANLIST);//send(m_socket, GETBANLIST, strlen(GETBANLIST), 0);
                }
            }
            printf("[CLIENT] Bytes recv: %ld | [MSG: %s]\n", bytesRecv, recvbuf);
        }
    }

    bool get_answear(SOCKET m_socket, char *recvbuf, int &bytesRecv)
    {
        bytesRecv = recv_(recvbuf);//recv(m_socket, recvbuf, ServerCfg::BUFF_SIZE, 0);
        if (bytesRecv == 0)// || bytesRecv == WSAECONNRESET)
        {
            printf("[CLIENT] Connection closed.\n");
            return false;
        }
        if (bytesRecv < 0)
        {
            printf("[CLIENT] Connection lost.\n");
            return false;
        }
        return true;
    }

    bool try_login(SOCKET m_socket,
                   char *recvbuf, char *sendbuf, int &bytesRecv,
                   int &bytesSent, bool &full_break)
    {
        printf("[CLIENT] Login: ");
        std::cin >> sendbuf;
        bytesSent = send_(sendbuf);//send(m_socket, sendbuf, strlen(sendbuf), 0);
        if (bytesSent < 0)
            return false;
        //printf("[CLIENT] Bytes sent: %ld\n", bytesSent);

        bytesRecv = SOCKET_ERROR;
        while (bytesRecv == SOCKET_ERROR)
        {
            char recvbuf[ServerCfg::BUFF_SIZE] = "";
            if (!get_answear(m_socket, recvbuf, bytesRecv))
            {
                full_break = true;
                return false;
            }

            if (strcmp(recvbuf, ServerCfg::USERLOGIN) == 0)
            {
                printf("[SERVER] Hello, user.\n");
                //loginned = true;

            }
            else
            {
                if (strcmp(recvbuf, ServerCfg::ROOTLOGIN) == 0)
                {
                    printf("[SERVER] Hello, root.\n");
                    //loginned = true;
                    //is_root = true;
                }
                else
                {
                    printf("[CLIENT] Bytes recv: %ld | [MSG: %s]\n", bytesRecv, recvbuf);
                    return false;
                }
            }

            //printf("[CLIENT] Bytes recv: %ld | [MSG: %s]\n", bytesRecv, recvbuf);
        }

    }

    void answer_control(char *answer)
    {
        while(true)
        {
            std::cin >> answer;
            if ((strlen(answer) > 1 || (answer[0] != 'Y' && answer[0] != 'N' && answer[0] != 'y' && answer[0] != 'n')))
            {
                printf("Error, try again: [Y/N] ");
                continue;
            }
            else break;
        }
    }

    void ipaddres_control(char *ip)
    {
        while(true)
        {
            std::cin >> ip;
            /*if (inet_addr(ip) == INADDR_NONE)
            {
                printf("Error, try again: ");
                continue;
            }
            else */break;
        }
    }

    int get_login()
    {
        printf("[CLIENT] Login: ");
        std::cin >> sendbuf;
        bytesSent = send_(sendbuf);//send(m_socket, sendbuf, strlen(sendbuf), 0);
        if (bytesSent < 0)
            throw 0;

        bytesRecv = SOCKET_ERROR;
        while (bytesRecv == SOCKET_ERROR)
        {
            char recvbuf[ServerCfg::BUFF_SIZE] = "";
            if (!get_answear(m_socket, recvbuf, bytesRecv))
            {
                throw -1;
            }

            if (strcmp(recvbuf, ServerCfg::USERLOGIN) == 0)
            {
                loginned = true;
            }
            else
            {
                if (strcmp(recvbuf, ServerCfg::ROOTLOGIN) == 0)
                {
                    loginned = true;
                    is_root = true;
                }
                else
                {
                    printf("[CLIENT] Bytes recv: %ld | [MSG: %s]\n", bytesRecv, recvbuf);
                    return 1;
                }
            }
        }
        return 0;
    }

    int get_pass()
    {
        char loginres[ServerCfg::BUFF_SIZE] = "";
        char answear[3];
        printf("[CLIENT] Password: ");
        std::cin >> sendbuf;

        bytesSent = send_(sendbuf);//send(m_socket, sendbuf, strlen(sendbuf), 0);
        if (bytesSent < 0)
            throw 0;
        //printf("[CLIENT] Bytes sent: %ld\n", bytesSent);

        bytesRecv = SOCKET_ERROR;
        while (bytesRecv == SOCKET_ERROR)
        {
            char recvbuf[ServerCfg::BUFF_SIZE] = "";
            if (!get_answear(m_socket, recvbuf, bytesRecv))
            {
                throw -1;
            }

            if (strcmp(recvbuf, ServerCfg::BANSTATUS[0]) == 0)
            {
                printf("[SERVER] Password is incorrect.\n");
                printf("[SERVER] You have 0 attempts. You have been banned.\n");
                throw -1;
            }
            if (strcmp(recvbuf, ServerCfg::BANSTATUS[1]) == 0)
            {
                printf("[SERVER] You have been banned from this server.\n");
                throw -1;
            }

            printf("[CLIENT] Bytes recv: %ld | [MSG: %s]\n", bytesRecv, recvbuf);
            if (strcmp(recvbuf, ServerCfg::PASSCORRECT) == 0)
            {
                pass_correct = true;
            }
        }

        if (!pass_correct)
        {
            printf("[CLIENT] Try again? ");
            answer_control(answear);
            if (answear[0] == 'N' || answear[0] == 'n')
                return -1;
        }
        else
        {
            if (is_root)
                printf("[SERVER] Hello, root.\n");
            else
                printf("[SERVER] Hello, user.\n");
        }
    }

    bool cmp(char* str1, const char* str2)
    {
        int size = 0;
        if (strlen(str1) > strlen(str2)) size = strlen(str2);
        else size = strlen(str1);

        bool result = false;

        for (int i = 0; i < size; i++)
        {
            if (str1[i] == str2[i])
                result = true;
            else
            {
                result = false;
                break;
            }
        }
        return result;
    }
public:
    Client()
    {
        bytesRecv = SOCKET_ERROR;
        strcpy(sendbuf, "");
        strcpy(recvbuf, "");

        loginned = false;
        pass_correct = false;
        is_root = false;

        getbanlist = false;
        unban = false;
        active_command = false;
    }

    ~Client()
    {
        shutdown(m_socket, 1);
    }

    int init()
    {
        time_t t = time(NULL);
        tm* aTm = localtime(&t);
        printf("%04d/%02d/%02d %02d:%02d:%02d \n",aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
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
        //bool have_ip(false);
        have_ip = false;

        //SOCKET m_socket; // создаем сокет
        m_socket = socket(AF_INET, SOCK_STREAM, ServerCfg::PROTOCOL);
        // ¬ качестве параметров используютс¤ семейство интернет-адресов (IP),
        // потоковые сокеты и протокол TCP/IP.

        if (m_socket < 0)//== INVALID_SOCKET) // ¬ы¤вление ошибок
        {
            printf("[ERROR: SOCKET] Error at socket(): \n");//%ld\n", WSAGetLastError());
            // WSAGetLastError возвращает номер последней возникнувшей ошибки
            //WSACleanup();
            //system("pause");
            return -1;
        }
    }

    int try_connect()
    {
        // ѕќƒ Ћё„≈Ќ»≈   —≈–¬≈–”
        printf("[STATUS] Client started.\n");
        /*==================================*/
        char answear[128];
        char ipaddres[16];
        if (!have_ip) {
        printf("[CLIENT] Do you want to enter server IP address? [Y/N] " );
        answer_control(answear);
        if (answear[0] == 'N' || answear[0] == 'n')
        {
            strcpy(ipaddres, ServerCfg::LOCALHOST);
            printf("[CLIENT] Connecting to server...\n" );
        }
        else
        {
            printf("[CLIENT] Enter server IP addres: ");
            ipaddres_control(ipaddres);
            printf("[CLIENT] Try to connect to %s...\n", ipaddres);
        }
        have_ip = true;
        }
        /*==================================*/
        // service содержит информаци¤ о семействе адресов,
        // IP адрес и номер порта
        service.sin_family = AF_INET; // семейство адресов »нтернет
        service.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(ipaddres); // локальный IP
        service.sin_port = htons(ServerCfg::PORT); // номер порта

        // ¬ы¤вление ошибок
        //if (connect(m_socket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
        if (connect(m_socket, (struct sockaddr*) &service, sizeof(service)) < 0)
        {
            printf("[ERROR: SOCKADDR] Connection failed.\n");
            //closesocket(m_socket);
            //WSACleanup();
            //system("pause");
            return -1;
        }
        bytesRecv = recv_(recvbuf);//recv(m_socket, recvbuf, ServerCfg::BUFF_SIZE, 0 );
    }

    int main_func()
    {
        try
        {
            while(true)
            {
                if (cmp(recvbuf, ServerCfg::BANSTATUS[1]))
                {
                    printf("[SERVER] You have been banned from this server.\n");
                    break;
                }
                /* ========================================================================== */
                // Ћогин
                if (!loginned)
                {
                    get_login();
                }
                /* ========================================================================== */
                // ќ“ѕ–ј¬ ј ƒјЌЌџ’ Ќј —≈–¬≈–
                if (loginned && !pass_correct)
                {
                    get_pass();
                }
                if (loginned && pass_correct)
                {
                    command_viewer();
                }
            }
        }
        catch(int value)
        {
            printf("[CLIENT] Terminate.\n");
            shutdown(m_socket, 1);
            return -1;
        }
        catch(...)
        {
            printf("[CLIENT] Fatal error! Terminate.\n");
            shutdown(m_socket, 1);
            return -1;
        }
        return 0;
    }
};


int main()
{
    time_t t = time(NULL);
    tm* aTm = localtime(&t);
    printf("%04d/%02d/%02d %02d:%02d:%02d \n",aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);

    Client c;
    c.init();
    if (c.try_connect() >= 0)
		c.main_func();
    //system("pause");
    return 0;
}
