#ifndef NETWORK_H_
#define NETWORK_H_

class network
{
public:
    network(char *_channel);
    virtual ~network();
    bool setup;
    int start();
    bool charSearch(char *toSearch, char *searchFor);

private:
    char *port;
    char *channel;
    int s;
    char *nick;
    char *usr;
    char * timeNow();
    bool NetSend(char *msg);
    void NetPong(char *buf);
    void NetHandel(char *buf);
    bool NetCheck(char *buf, char *Searchbuf);
    char * NetSender(char *buf);
    char * NetChannel(char *buf);
};

#endif // NETWORK_H_
