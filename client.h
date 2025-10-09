

#ifndef CLIENT_H
#define CLIENT_H

#include<time.h>
#include <QObject>
#include <QCoreApplication>
#include <QQmlApplicationEngine>

#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>
#include <QQmlContext>
#include<cstdio>
#include<iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fstream>
#include <ifaddrs.h>
#include <unistd.h>

#include "model/Connection_Client.h"
#include "message.pb.h"


class Connection_Client;

class Client : public QObject
{



    Q_OBJECT
    Q_PROPERTY(QString Text READ Text WRITE setText NOTIFY TextChanged FINAL)
    Q_PROPERTY(std::vector<QString> Clients READ Clients WRITE setClients NOTIFY ClientsChanged FINAL)
    Q_PROPERTY(QString Chatwindow READ Chatwindow WRITE setChatwindow NOTIFY ChatwindowChanged FINAL)
    Q_PROPERTY(int WindowId READ WindowId WRITE setWindowId NOTIFY WindowIdChanged FINAL)
    Q_PROPERTY(QVariantMap Chats READ Chats WRITE setChats NOTIFY ChatsChanged FINAL)
    Q_PROPERTY(QVariantMap Notif READ Notif WRITE setNotif NOTIFY NotifChanged FINAL)
    Q_PROPERTY(QVector<QString> ServerList READ ServerList WRITE setServerList NOTIFY ServerListChanged FINAL)
    Q_PROPERTY(QString EXE_PATH READ EXE_PATH WRITE setEXE_PATH NOTIFY EXE_PATHChanged FINAL)
    Q_PROPERTY(QString Chatid READ Chatid WRITE setChatid NOTIFY ChatidChanged FINAL)


  // Localhost or the IP of the server
    int PORT = 5050;
    void user_handle(std::string ip ,std::string port);
    void find_server_handle();

    std::string MyId;
    int connected=0;
    void writemssg(int socket,const char* msg);
    void sendfile(int socket,std::string filename,QString to);
    std::string revcfile(int socket,std::string filename);



    std::ifstream::pos_type filesize(const char* filename);


public:
    void client_run(messager::Packet lines);

    Connection_Client* tcp_connection;
    Connection_Client* udp_connection;
    explicit Client(QObject *parent = nullptr);
    ~Client(){

    }



    QString Text() const;
    void setText(const QString &newText);

    std::vector<QString> Clients() const;
    void setClients(const std::vector<QString> &newClients);
    std::vector<QString> m_Clients;
    QString Chatwindow() const;
    void setChatwindow(const QString &newChatwindow);

    int WindowId() const;
    void setWindowId(int newWindowId);



    QVariantMap Chats() const;
    void setChats(const QVariantMap &newChats);
    QVariantMap m_Chats;
    QVariantMap Notif() const;
    QVector<QString> ServerList() const;
    QString EXE_PATH() const;
    QString Chatid() const;
    void setNotif(const QVariantMap &newNotif);
    void setServerList(const QVector<QString> &newServerList);
    void setEXE_PATH(const QString &newEXE_PATH);
    void setChatid(const QString &newChatid);
    QVariantMap m_Notif;
    QVector<QString> m_ServerList;
    QString m_EXE_PATH;
    QString m_Chatid{""};

signals:

    void TextChanged();

    void ClientsChanged();

    void ChatwindowChanged();

    void WindowIdChanged();

    void ChatsChanged();

    void NotifChanged();

    void ServerListChanged();
    void EXE_PATHChanged();
    void ChatidChanged();

public slots:
    void send_message_login(QString user,QString pass);
    void send_message_signup(QString user,QString pass);
    QString now_time();
    void send_private_message(QString to,QString text);
    void send_private_file(QString to,QString path);
    void go_to_private(QString text);
    void go_to_server(QString text);
    void get_private_file(QString text);
    void appendChats(QString key,QString text) ;
    void logout();
private:
    QString m_Text;

    QString m_Chatwindow;
    int m_WindowId{0};


};

#endif // CLIENT_H
