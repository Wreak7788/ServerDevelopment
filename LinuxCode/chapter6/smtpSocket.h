#pragma once

#include<string>
#include<vector>

using std::string;
class SmtpSocket final
{
public:
    static bool sendMail(const string& server, short port, const string& from,
    const string& fromPassword, const std::vector<string>& to, const string & subject,
    const string& mailData);

public:
    SmtpSocket(void);
    ~SmtpSocket(void);

    bool isConnected() const {return m_hSocket;}
    bool connect(const char* pszUrl, short port=25);
    bool logon(const char* pszUser, const char* pszPassword);
    bool setMailFrom(const char* pszFrom);
    bool setMailTo(const std::vector<string>& sendTo);
    bool send(const string& subject, const string& mailData);
    void closeConnection();
    void quie();
    
private:
    bool checkResponse(const char* recvCode);

private:
    bool                m_bConnected;
    SOCKET              m_hSocket;
    string              m_strUser;
    string              m_strPassword;
    string              m_strFrom;
    std::vector<string> m_strTo;
};