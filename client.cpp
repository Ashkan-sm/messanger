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

void Client::send_message(QString text)
{
    tcp_connection->writemssg(text.toStdString().c_str());
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
    tcp_connection->writemssg(("/private_msg "+to.toStdString()).c_str());
    tcp_connection->writemssg((text.toStdString()).c_str());
    tcp_connection->writemssg("EOLOG");
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

void Client::client_run(std::vector<std::string> lines) {
    int ii=0;
    for (auto i : lines){
        std::cout<<"line "<<ii++<<": "<<i<<std::endl;
    }

    if(strcmp(lines[0].c_str(),"/ClientId")==0){
        m_Clients.clear();
        m_Notif.clear();
        int i=1;
        while(i<lines.size()-2){
            m_Clients.push_back(QString::fromLocal8Bit(lines[i].c_str()));
            m_Notif[QString::fromLocal8Bit(lines[i].c_str())]=0;
            i++;
        }
        emit NotifChanged();
        emit ClientsChanged();
    }
    else if(strncmp(lines[0].c_str(),"/private_msg",strlen("/private_msg"))==0){

        std::string sender=(lines[0].substr(lines[0].find(' ')+1));

        std::fstream savefile;
        savefile.open(("savedata_client/"+MyId+sender+".txt"),std::ios::in | std::ios::app);
        savefile.clear();
        savefile.seekp(0, std::ios::end);
        savefile << sender+"> "+lines[1]+"EOMSG\n";

        savefile.close();
        std::cout << "Wrote pv msg" << std::endl;

        if (sender==Chatwindow().toStdString() || sender==MyId){

            m_Chats[QString::fromStdString(sender)]=m_Chats[QString::fromStdString(sender)].toString()+QString::fromStdString(sender)+"> "+QString::fromStdString(lines[1]+"EOMSG");
            tcp_connection->writemssg(("/status_update "+sender+" 3").c_str());
            tcp_connection->writemssg("EOLOG");

            emit ChatsChanged();
        }
        else{
            m_Notif[QString::fromStdString(sender)]=m_Notif[QString::fromStdString(sender)].toInt()+1;
            emit NotifChanged();
            qInfo()<<1;
        }




    }
    else if(strncmp(lines[0].c_str(),"/status_update",strlen("/status_update"))==0){
        std::string foruser=lines[0].substr(lines[0].find(' ')+1,lines[0].rfind(' ')-lines[0].find(' ')-1);
        char status=lines[0][lines[0].length()-1];
        std::string lastmsg=m_Chats[QString::fromStdString(foruser)].toString().toStdString();

        std::fstream savefile;

        while(1) {
            std::size_t index = lastmsg.rfind("/status");
            // std::cout<<id<<"**"<<lastmsg<<std::endl;
            if (index == std::string::npos) {
                break;
            }

            if (lastmsg[index + 8] < status) {
                auto temp = m_Chats[QString::fromStdString(foruser)].toString();
                temp[index + 8] = status;
                m_Chats[QString::fromStdString(foruser)] = temp;
                emit ChatsChanged();

                // fseek(savefile,index+8);
            } else {
                break;
            }
            lastmsg = lastmsg.substr(0, index + 1);

        }
        savefile.open(("savedata_client/"+MyId+foruser+".txt"), std::ios::in | std::ios::out);
        std::string all_lines;
        std::string line;
        if (savefile.is_open()) {
            while(getline(savefile,line)){
                auto index=line.find("/status");
                if (index !=std::string::npos){
                    line[index+8]=status;
                }
                all_lines+=line+'\n';
            }

            savefile.close();
            std::ofstream savefile2("savedata_client/"+MyId+foruser+".txt");

            savefile2.write(all_lines.c_str(),all_lines.size());
            savefile2.close();

        }
        else{
            std::cout<<"didntope "<<"savedata_client/"+MyId+foruser+".txt"<<std::endl;
        }
        std::cout<<"saved"<<std::endl;

    }
    else if(strncmp(lines[0].c_str(),"/sending_file",strlen("/sending_file"))==0){
        std::string path=lines[0].substr(lines[0].find(' ')+1);

        std::string type= tcp_connection->revcfile(path);

        if (type=="png"){
            appendChats(Chatid(),Chatid()+"> "+"/image "+QString::fromStdString(path)+"/time "+now_time()+"/status 2"+"EOMSG");
        } else if(type=="jpg"){
            appendChats(Chatid(),Chatid()+"> "+"/image "+QString::fromStdString(path)+"/time "+now_time()+"/status 2"+"EOMSG");
        } else if(type=="mp4"){
            appendChats(Chatid(),Chatid()+"> "+"/video "+QString::fromStdString(path)+"/time "+now_time()+"/status 2"+"EOMSG");
        }
    }

    else{
        if(connected==0 && strncmp(lines[1].c_str(),"Wellcome",strlen("Wellcome"))==0){
            connected=1;
            MyId=lines[1].substr(lines[1].find(' ')+1);
            setWindowId(1);
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
