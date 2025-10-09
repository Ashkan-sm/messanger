#include "client.h"
#include <iostream>
#include <fstream>


void Client::find_server_handle()
{

    int datalen;
    char databuf[1024];
    udp_connection = new Connection_Client(12345,INADDR_ANY,1);


    while (1) {
        std::string msg=udp_connection->recv();
//        std::string server_ip_=stringbuff.substr(0,stringbuff.find(' '));
//        std::string server_port= stringbuff.substr(stringbuff.find(' ')+1);

        QString out_server_id=QString::fromStdString(msg);

        if(!m_ServerList.contains(out_server_id)) {
            m_ServerList.append(out_server_id);
            emit ServerListChanged();
        }


    }
}

void Client::user_handle(std::string server_ip,std::string server_port)
{

    tcp_connection=new Connection_Client(std::stoi(server_port.c_str(),0,10), inet_addr(server_ip.c_str()));

    tcp_connection->connection_manager(this);
}





void create_directory_if_not_exists(const std::string& path) {
    struct stat info;

    // Check if the directory already exists
    if (stat(path.c_str(), &info) != 0) {
        // Directory does not exist, create it
        if (mkdir(path.c_str(), 0755) == 0) {
            std::cout << "Directory created: " << path << std::endl;
        } else {
            std::cerr << "Failed to create directory: " << path << std::endl;
        }
    } else if (info.st_mode & S_IFDIR) {
        std::cout << "Directory already exists: " << path << std::endl;

    } else {
        std::cerr << "The path exists, but it is not a directory: " << path << std::endl;
    }
}
Client::Client(QObject *parent): QObject{parent}
{

    setEXE_PATH(QCoreApplication::applicationDirPath());
    std::string path = "savedata_client";
    create_directory_if_not_exists(path);
    std::thread t([this]() {
        this->find_server_handle();
    });
    t.detach();


}

void Client::send_message_login(QString user, QString pass)
{
    messager::Packet login_msg;
    login_msg.set_data(pass.toStdString());
    login_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_LOGOUT);
    login_msg.set_name(user.toStdString());
    tcp_connection->writemssg(login_msg.SerializeAsString());
}

QString Client::now_time()
{
    std::time_t nowtime;
    std::time(&nowtime);
    std::tm *info=std::localtime(&nowtime);
    char timecstr[80];
    std::strftime(timecstr,80,"%c",info);
    return QString::fromStdString((std::string)timecstr);
}

void Client::send_private_message(QString to,QString text)
{
    messager::Packet pv_msg;
    pv_msg.set_data(text.toStdString());
    pv_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_PRIVATE_MESSAGES);
    pv_msg.set_receiver_name(to.toStdString());

    tcp_connection->writemssg(pv_msg.SerializeAsString());

}

void Client::send_private_file(QString to, QString path)
{
    tcp_connection->sendfile(path.toStdString(),to);

    tcp_connection->writemssg("EOLOG");

}

void Client::go_to_private(QString text)
{
    m_Chats[text]="";
    std::ifstream savefile;
    savefile.open(("savedata_client/"+MyId+text.toStdString()+".txt"),std::ios::in);
    tcp_connection->writemssg(("/status_update "+text.toStdString()+" 3").c_str());
    tcp_connection->writemssg("EOLOG");

    if(savefile.is_open()){
        std::string line;
        while (std::getline(savefile, line)) {
            // setText(Text()+QString::fromStdString(line)+"<br>\n");

            m_Chats[text]=m_Chats[text].toString()+QString::fromStdString(line);

            emit ChatsChanged();
        }
        savefile.close();
    }

    setChatwindow(text);
    m_Notif[text]=0;
    emit NotifChanged();

}

void Client::appendChats(QString key, QString text)
{
    m_Chats[key]=m_Chats[key].toString()+text;
    std::fstream savefile;
    savefile.open(("savedata_client/"+MyId+key.toStdString()+".txt"),std::ios::in | std::ios::app);
    savefile.clear();
    savefile.seekp(0, std::ios::end);
    savefile << text.toStdString()+"\n";
    savefile.close();
    std::cout << "Wrote Append Chat" << std::endl;
    emit ChatsChanged();
}


QString Client::Text() const
{
    return m_Text;
}

void Client::setText(const QString &newText)
{
    if (m_Text == newText)
        return;
    m_Text = newText;
    emit TextChanged();
}

std::vector<QString> Client::Clients() const
{
    return m_Clients;
}

void Client::setClients(const std::vector<QString> &newClients)
{
    if (m_Clients == newClients)
        return;
    m_Clients = newClients;
    emit ClientsChanged();
}

QString Client::Chatwindow() const
{
    return m_Chatwindow;
}

void Client::setChatwindow(const QString &newChatwindow)
{
    if (m_Chatwindow == newChatwindow)
        return;
    m_Chatwindow = newChatwindow;
    emit ChatwindowChanged();
}

int Client::WindowId() const
{
    return m_WindowId;
}

void Client::setWindowId(int newWindowId)
{
    if (m_WindowId == newWindowId)
        return;
    m_WindowId = newWindowId;
    emit WindowIdChanged();
}

QVariantMap Client::Chats() const
{
    return m_Chats;
}

void Client::setChats(const QVariantMap &newChats)
{
    if (m_Chats == newChats)
        return;
    m_Chats = newChats;
    emit ChatsChanged();
}




QVariantMap Client::Notif() const
{
    return m_Notif;
}
QVector<QString> Client::ServerList() const
{
    return m_ServerList;
}
QString Client::EXE_PATH() const
{
    return m_EXE_PATH;
}
QString Client::Chatid() const
{
    return m_Chatid;
}
void Client::setServerList(const QVector<QString> &newServerList)
{
    if (m_ServerList == newServerList)
        return;
    m_ServerList = newServerList;
    emit ServerListChanged();
}
void Client::setEXE_PATH(const QString &newEXE_PATH)
{
    if (m_EXE_PATH == newEXE_PATH)
        return;
    m_EXE_PATH = newEXE_PATH;
    emit EXE_PATHChanged();
}
void Client::setChatid(const QString &newChatid)
{
    if (m_Chatid == newChatid)
        return;
    m_Chatid = newChatid;
    emit ChatidChanged();
}
void Client::setNotif(const QVariantMap &newNotif)
{
    if (m_Notif == newNotif)
        return;
    m_Notif = newNotif;
    emit NotifChanged();
}


void Client::get_private_file(QString text) {

    tcp_connection->writemssg(("/get_file "+text).toStdString().c_str());
    tcp_connection->writemssg("EOLOG");

}





void Client::go_to_server(QString text) {
    std::cout<<text.toStdString()<<"yooo"<<std::endl;
    std::thread t([this](std::string ip, std::string port) {
        this->user_handle(ip, port);
    }, text.split(' ')[0].toStdString(), text.split(' ')[1].toStdString());
    t.detach();
}

void Client::logout() {
    connected=0;
    tcp_connection->writemssg("/logout");
    tcp_connection->writemssg("EOLOG");

}

void Client::client_run(messager::Packet msg) {

    if (msg.packet_type() == messager::Packet_PacketType_PACKET_TYPE_CLIENT_LIST) {
        m_Clients.clear();
        m_Notif.clear();
        std::stringstream users(msg.data());
        std::string client;
        while (users >> client) {
            m_Clients.push_back(QString::fromStdString(client));
            m_Notif[QString::fromStdString(client)] = 0;
        }
        emit NotifChanged();
        emit ClientsChanged();
    } else if (msg.packet_type() == messager::Packet_PacketType_PACKET_TYPE_PRIVATE_MESSAGES) {

        std::string sender = msg.name();

        std::fstream savefile;
        savefile.open(("savedata_client/" + MyId + sender + ".txt"), std::ios::in | std::ios::app);
        savefile.clear();
        savefile.seekp(0, std::ios::end);
        savefile << sender + "> " + msg.data() + "EOMSG\n";

        savefile.close();
        std::cout << "Wrote pv msg" << std::endl;

        if (sender == Chatwindow().toStdString() || sender == MyId) {

            m_Chats[QString::fromStdString(sender)] =
                    m_Chats[QString::fromStdString(sender)].toString() + QString::fromStdString(sender) + "> " +
                    QString::fromStdString(msg.data() + "EOMSG");
            messager::Packet send_msg;
            send_msg.set_data("3");
            send_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_STATUS_UPDATE);
            tcp_connection->writemssg(send_msg.SerializeAsString());


            emit ChatsChanged();
        } else {
            m_Notif[QString::fromStdString(sender)] = m_Notif[QString::fromStdString(sender)].toInt() + 1;
            emit NotifChanged();
            qInfo() << 1;
        }


    } else if (msg.packet_type() == messager::Packet_PacketType_PACKET_TYPE_STATUS_UPDATE) {
        std::string foruser = msg.name();
        std::string status = msg.data();
        std::string lastmsg = m_Chats[QString::fromStdString(foruser)].toString().toStdString();
//to be completed

    } else if (msg.packet_type() == messager::Packet_PacketType_PACKET_TYPE_FILE_SEND) {
//        std::string path=lines[0].substr(lines[0].find(' ')+1);
//
//        std::string type= tcp_connection->revcfile(path);
//
//        if (type=="png"){
//            appendChats(Chatid(),Chatid()+"> "+"/image "+QString::fromStdString(path)+"/time "+now_time()+"/status 2"+"EOMSG");
//        } else if(type=="jpg"){
//            appendChats(Chatid(),Chatid()+"> "+"/image "+QString::fromStdString(path)+"/time "+now_time()+"/status 2"+"EOMSG");
//        } else if(type=="mp4"){
//            appendChats(Chatid(),Chatid()+"> "+"/video "+QString::fromStdString(path)+"/time "+now_time()+"/status 2"+"EOMSG");
//        }
    } else if (msg.packet_type() == messager::Packet_PacketType_PACKET_TYPE_AUTHENTICATE) {
        if (connected == 0) {
            connected = 1;
            MyId = msg.data().substr(msg.data().find(' ') + 1);
            setWindowId(1);
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));

    }
}

void Client::send_message_signup(QString user, QString pass) {
    messager::Packet login_msg;
    login_msg.set_data(pass.toStdString());
    login_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_SIGNUP);
    login_msg.set_name(user.toStdString());
    tcp_connection->writemssg(login_msg.SerializeAsString());
}
