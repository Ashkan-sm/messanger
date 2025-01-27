#include "client.h"
#include <iostream>
#include <fstream>

void Client::find_server_handle()
{

    int datalen;
    char databuf[1024];
    udp_connection = new Connection_Client(12345,INADDR_ANY,1);


    while (1) {

        if (udp_connection->read_sock(databuf,sizeof(databuf)) < 0) {
            perror("Reading datagram message error");
            exit(1);
        } else {
            std::string stringbuff(databuf);
            std::string server_ip_=stringbuff.substr(0,stringbuff.find(' '));
            std::string server_port= stringbuff.substr(stringbuff.find(' ')+1);

            QString out_server_id=QString::fromStdString(server_ip_+" "+server_port);

            if(!m_ServerList.contains(out_server_id)) {
                m_ServerList.append(out_server_id);
                emit ServerListChanged();
            }

        }
    }
}

void Client::user_handle(std::string server_ip,std::string server_port)
{

    tcp_connection=new Connection_Client(std::stoi(server_port.c_str(),0,10), inet_addr(server_ip.c_str()));

    tcp_connection->connection_manager(this);
}



void Client::
writemssg(int socket, const char *msg)
{

    std::string msgstr(msg);

    std::string header(10,' ');
    std::string msglength=std::to_string(msgstr.length());
    header.replace(0,msglength.length(),msglength);

    std::string buffer(header+msgstr);

    send(socket,buffer.c_str(),buffer.length(),0);
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
    writemssg(tcp_connection->nClientSocket,text.toStdString().c_str());
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

    writemssg(tcp_connection->nClientSocket,("/private_msg "+to.toStdString()).c_str());
    writemssg(tcp_connection->nClientSocket,(text.toStdString()).c_str());
    writemssg(tcp_connection->nClientSocket,"EOLOG");

}

void Client::send_private_file(QString to, QString path)
{
    sendfile(tcp_connection->nClientSocket,path.toStdString(),to);
    writemssg(tcp_connection->nClientSocket,"EOLOG");


}

void Client::go_to_private(QString text)
{
    m_Chats[text]="";
    std::ifstream savefile;
    savefile.open(("savedata_client/"+MyId+text.toStdString()+".txt"),std::ios::in);
    writemssg(tcp_connection->nClientSocket,("/status_update "+text.toStdString()+" 3").c_str());
    writemssg(tcp_connection->nClientSocket,"EOLOG");
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
    writemssg(tcp_connection->nClientSocket,("/get_file "+text).toStdString().c_str());
    writemssg(tcp_connection->nClientSocket,"EOLOG");
}

void Client::sendfile(int socket, std::string filename,QString to) {
    std::ifstream file(filename.substr(7),std::ios::binary);
    qInfo()<<filename.substr(7);
    if (file.is_open()){
        writemssg(socket,("/send_file "+filename.substr(filename.rfind('/'))+" "+to.toStdString()).c_str());
        writemssg(socket,"EOLOG");



        qInfo()<<filename+" "+to.toStdString();
        long long int size=filesize(filename.substr(7).c_str());
        std::cout<<"size send : "<<size<<std::endl;
        writemssg(socket, std::to_string(size).c_str());

        char buffer[1024];
        do {
            file.read(buffer,sizeof(buffer));
            if (file.gcount()){
                auto a =file.gcount();
                send(socket,buffer,file.gcount(),0);
            }
        } while (!file.eof());

    }

    else{
        qInfo()<<"didnt open file";
    }
}
std::ifstream::pos_type Client::filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    std::ifstream::pos_type loc=in.tellg();
    in.close();
    return loc;
}

std::string Client::revcfile(int socket, std::string filename) {

    std::ofstream resivedfile("./savedata_client/"+filename);

    ssize_t bytes_received;

    char buffer[1024];
    char sizestr[1024];
    tcp_connection->readmsg(socket,sizestr);
    long long int size = atoi(sizestr);
    long long int recvCounter=0;
    while(recvCounter+1024<size){
        bytes_received = recv(socket, buffer, sizeof(buffer), 0);
        resivedfile.write(buffer,bytes_received);
        recvCounter += bytes_received;
    }
    bytes_received = recv(socket, buffer, size-recvCounter, 0);
    resivedfile.write(buffer,bytes_received);


    resivedfile.close();

    return filename.substr(filename.find('.')+1);
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
    writemssg(tcp_connection->nClientSocket,"/logout");
    writemssg(tcp_connection->nClientSocket,"EOLOG");
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
            writemssg(tcp_connection->nClientSocket,("/status_update "+sender+" 3").c_str());
            writemssg(tcp_connection->nClientSocket,"EOLOG");

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
        std::string type=revcfile(tcp_connection->nClientSocket,path);

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
