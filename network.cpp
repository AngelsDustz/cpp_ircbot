#define _WIN32_WINNT 0x501

#include "network.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAXDATASIZE 256
bool canSayHello=true;

network::network(char *_channel)
{
    nick = "NICK AngelsBot\r\n";
    usr = "USER AngelsBot tolmoon tolsun :AngelsBot\r\n";
    channel = _channel;
}

network::~network()
{
    closesocket(s);
}

int network::start()
{
    struct addrinfo hints, *servinfo;
    WSADATA wsaData;

    setup = true;
    port = "6667";

    memset(&hints, 0 ,sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    WSAStartup(MAKEWORD(2,2),&wsaData);

    int res;
    if ((res = getaddrinfo("irc.esper.net",port,&hints,&servinfo)) != 0)
    {
        setup = false;
        printf("getaddrinfo failed with error : %d\n",res);
    }

    if ((s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        perror("client: socket");
    }

    if (connect(s, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        closesocket(s);
        perror("Client Connect");
    }

    freeaddrinfo(servinfo);

    int numbytes;
    char buf[MAXDATASIZE];

    int count = 0;
    while (1)
    {
        count++;

        switch(count)
        {
    case 3:
        NetSend(nick);
        NetSend(usr);
        break;
    case 5:
        NetSend(channel);
    default:
        break;
        }
    numbytes = recv(s, buf, MAXDATASIZE-1, 0);
    buf[numbytes]='\0';
    if (!charSearch(buf,"PING")){
    cout << buf;}
    if (charSearch(buf,"421") && charSearch(buf,"PONG"))
    {
        return 2;
    }

    NetHandel(buf);

    if (charSearch(buf, "PING :"))
    {
        NetPong(buf);
    }

    if (numbytes==0)
    {
        cout << "--------------- CONNECTION CLOSED ---------------\n";
        cout << timeNow() << endl;

        break;
    }
    }
}

bool network::charSearch(char *toSearch, char *searchFor)
{
    int len = strlen(toSearch);
    int forLen = strlen(searchFor); // The length of the searchfor field

    //Search through each char in toSearch
    for (int i = 0; i < len;i++)
    {
        //If the active char is equil to the first search item then search toSearch
        if (searchFor[0] == toSearch[i])
        {
            bool found = true;
            //search the char array for search field
            for (int x = 1; x < forLen; x++)
            {
                if (toSearch[i+x]!=searchFor[x])
                {
                    found = false;
                }
            }

            //if found return true;
            if (found == true)
                return true;
        }
    }

    return 0;
}

char * network::timeNow()
{//returns the current date and time
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    return asctime (timeinfo);
}

bool network::NetSend(char *msg)
{
    int len;
    int bytes_sent;
    char *msgbuf=msg;
    len = strlen(msgbuf);
    bytes_sent = send(s,msgbuf,len,0);
    if (!charSearch(msg, "PONG")){
    cout << "Send : " << msg;}
    if (bytes_sent == 0)
        return false;
    else
        return true;
}

void network::NetPong(char *buf)
{
    //Get the reply address
    //loop through bug and find the location of PING
    //Search through each char in toSearch

    char * toSearch = "PING :";

    for (int i = 0; i < strlen(buf);i++)
        {
            //If the active char is equil to the first search item then search toSearch
            if (buf[i] == toSearch[0])
            {
                bool found = true;
                //search the char array for search field
                for (int x = 1; x < 4; x++)
                {
                    if (buf[i+x]!=toSearch[x])
                    {
                        found = false;
                    }
                }

                //if found return true;
                if (found == true)
                {
                    int count = 0;
                    //Count the chars
                    for (int x = (i+strlen(toSearch)); x < strlen(buf);x++)
                    {
                        count++;
                    }

                    //Create the new char array
                    char returnHost[count + 5];
                    returnHost[0]='P';
                    returnHost[1]='O';
                    returnHost[2]='N';
                    returnHost[3]='G';
                    returnHost[4]=' ';

                    count = 0;
                    //set the hostname data
                    for (int x = (i+strlen(toSearch)); x < strlen(buf);x++)
                    {
                        returnHost[count+5]=buf[x];
                        count++;
                    }

                    //send the pong
                    NetSend(returnHost);

                    return;
                }
            }
        }

}

void network::NetHandel(char *buf)
{
    if (NetCheck(buf, "JOIN #cubeworld"))
    {
        if (canSayHello==true){
        NetSend("PRIVMSG ");
        NetSend(NetChannel(buf));
        NetSend(" :Welcome to #cubeworld  ");
        NetSend(NetSender(buf));
        NetSend(" !\r\n");
        }
    }

    if (NetCheck(buf,"!stop") && (NetSender(buf)=="AngelsDustz"))
    {
        NetSend("QUIT Stop command initialized!\r\n");
    }

    if (NetCheck(buf, "!mac"))
    {
        NetSend("PRIVMSG ");
        NetSend(NetChannel(buf));
        NetSend(" :Want to play on a Mac or Linux check out this thread! http://cubeworldforum.org/topic/9840-\r\n");
        //NetSend("PRIVMSG #cubeworld :Want to play on a Mac or Linux check out this thread! http://cubeworldforum.org/topic/9840-\r\n");
    }

    if (NetCheck(buf, "!loading"))
    {
        NetSend("PRIVMSG ");
        NetSend(NetChannel(buf));
        NetSend(" :Got problems with loading please wait and/or blue fog read this! http://cubeworldforum.org/topic/7622-\r\n");
        //NetSend("PRIVMSG #cubeworld :Got problems with loading please wait and/or blue fog read this! http://cubeworldforum.org/topic/7622-\r\n");
    }

    if (NetCheck(buf, "!commands"))
    {
        NetSend("PRIVMSG ");
        NetSend(NetChannel(buf));
        NetSend(" :Current commands are : !mac, !loading, !commands, !lag and !ping.\r\n");
        //NetSend("PRIVMSG #cubeworld :Current commands are : !mac, !loading, !commands, !lag and !ping.\r\n");
    }

    if (NetCheck(buf, "!lag"))
    {
        NetSend("PRIVMSG ");
        NetSend(NetChannel(buf));
        NetSend(" :Having lag issues check out http://cubeworldforum.org/topic/13738-\r\n");
    }

    if (NetCheck(buf, "!ping"))
    {
        NetSend("PRIVMSG ");
        NetSend(NetChannel(buf));
        NetSend(" :Pong!\r\n");
    }
}

bool network::NetCheck(char *buf, char *Searchbuf)
{
    int len = strlen(buf);
    int len2 = strlen(Searchbuf);
    int len3 = len - len2;
    len3 = len3 - 2;
    string temp,temp2;
    temp = string(buf);
    temp2 = string(Searchbuf);
    if (temp.find(temp2)!=len3)
        return false;
    else
        return true;
}

char * network::NetSender(char *buf)
{
    string data=string(buf);
    char *Sender;
    int i=1;
    do {
        i++;
    } while (buf[i]!='!');
    string temp;
    temp=data;
    temp.erase(i,data.size());
    temp.erase(0,1);
    char * sRet = new char[temp.size()+1];
    copy(temp.begin(),temp.end(), sRet);
    sRet[temp.size()]='\0';
    return sRet;
}

char * network::NetChannel(char *buf)
{
    string data=string(buf);
    char *Channel;
    int i=data.find("PRIVMSG")+8;
    int temp1=strlen(buf);
    int a=i;
    do {
        a++;
    } while (buf[a]!=':');
    string temp;
    temp=data;
    temp.erase(a,temp1);
    temp.erase(0,i);
    Channel = new char[temp.size()+1];
    copy(temp.begin(),temp.end(), Channel);
    Channel[temp.size()]='\0';
    if (Channel[0]!='#')
    {
        return NetSender(buf);
    } else return Channel;
}
