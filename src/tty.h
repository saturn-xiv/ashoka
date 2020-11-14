#ifndef ASHOKA_TTY_H_
#define ASHOKA_TTY_H_

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>

#define SERIAL_PORT_READ_BUF_SIZE 1 << 10

namespace ashoka
{
    namespace tty
    {
        class SerialPort
        {
        public:
            SerialPort()
            {
            }

            ~SerialPort()
            {
                this->close();
            }

            void open(const std::string &name)
            {
                this->open(name, 9600);
            }
            void open(const std::string &name, unsigned short baud_rate)
            {
                this->open(name,
                           boost::asio::serial_port_base::baud_rate(baud_rate),
                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port::flow_control::none),
                           boost::asio::serial_port_base::parity(boost::asio::serial_port::parity::none),
                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port::stop_bits::one),
                           boost::asio::serial_port_base::character_size(8));
            }
            void open(const std::string &name,
                      boost::asio::serial_port::baud_rate baud_rate,
                      boost::asio::serial_port::flow_control flow_control,
                      boost::asio::serial_port::parity parity,
                      boost::asio::serial_port::stop_bits stop_bits,
                      boost::asio::serial_port_base::character_size);
            void close();
            void write(const std::string &request);

            void on_receive(const boost::system::error_code &ec, size_t len);

        protected:
            virtual void on_receive(const std::string &response) = 0;
            virtual std::optional<std::pair<std::size_t, std::size_t>> match(const std::string &buf) = 0;

        private:
            void read();

            boost::asio::io_service io_service;
            std::shared_ptr<boost::asio::serial_port> port;
            std::mutex locker;
            char raw[SERIAL_PORT_READ_BUF_SIZE];
            std::string buffer;
        };

        namespace usr_g768
        {

            class Apn
            {
            public:
                Apn(const std::string host) : host(host), user(std::nullopt), password(std::nullopt), auth(0) {}
                Apn(const std::string host,
                    const std::optional<std::string> user,
                    const std::optional<std::string> password,
                    const unsigned short auth) : host(host), user(user), password(password), auth(auth) {}
                friend class SerialPort;
                friend class SMS;

            private:
                const std::string host;
                const std::optional<std::string> user;
                const std::optional<std::string> password;
                const unsigned short auth;
            };
            enum HttpdClientMethod
            {
                GET,
                POST
            };
            class HttpdClient
            {
            public:
                HttpdClient(const std::string host, std::string path)
                    : host(host), port(80), path(path), method(GET), over_time(10) {}

                HttpdClient(const std::string host, const unsigned short port, std::string path, const HttpdClientMethod method, size_t over_time)
                    : host(host), port(port), path(path), method(method), over_time(over_time) {}

                friend class SerialPort;
                friend class SMS;

            private:
                const std::string host;
                const unsigned short port;
                const std::string path;
                const HttpdClientMethod method;
                const size_t over_time;
            };
            enum NetTransportMode
            {
                TCPC,
                TCPS,
                UDPC
            };
            class NetTransport
            {
            public:
                NetTransport(const NetTransportMode mode, const std::string host, const unsigned short port) : mode(mode), host(host), port(port)
                {
                }
                friend class SerialPort;
                friend class SMS;

            private:
                const std::string host;
                const NetTransportMode mode;
                const unsigned short port;
            };
            class Request
            {
            public:
                Request(const std::string password) : password(password) {}
                std::string version() const { return this->at("VER"); }
                std::string sn() const { return this->at("SN"); }
                std::string icc_id() const { return this->at("ICCID"); }
                std::string imei() const { return this->at("IMEI"); }
                std::string ip() const { return this->at("CIP"); }
                std::string station() const { return this->at("LBS"); }
                std::string clock() const { return this->at("CCLK"); }
                std::string ping() const { return this->at("PING"); }
                std::string reboot() const { return this->at("Z"); }
                std::string sys_info() const { return this->at("SYSINFO"); }

                std::string apn(const std::string &host) const
                {
                    return this->apn(host, std::nullopt, std::nullopt, 0);
                }
                std::string apn(
                    const std::string &host,
                    const std::optional<std::string> &user,
                    const std::optional<std::string> &password,
                    const unsigned short auth) const
                {
                    std::stringstream ss;
                    ss << host << SEP;
                    if (user)
                    {
                        ss << user.value();
                    }
                    ss << SEP;
                    if (password)
                    {
                        ss << password.value();
                    }
                    ss << SEP;
                    ss << auth;
                    return ss.str();
                }

                std::string sms_work_mode() const { return this->at("WKMODE", "SMS"); }
                std::string sms_dest_number(const std::string &phone) const { return this->at("DSTNUM", phone); }

                std::string net_work_mode() const { return this->at("WKMODE", "NET"); }
                std::string net_a_enable(bool f) const { return this->at("SOCKAEN", f ? "ON" : "OFF"); }
                std::string net_a_host(const NetTransportMode mode, const std::string &host, unsigned short port) const
                {
                    std::stringstream ss;
                    switch (mode)
                    {
                    case TCPC:
                        ss << "TCPC";
                        break;
                    case TCPS:
                        ss << "TCPS";
                        break;
                    case UDPC:
                        ss << "UDPC";
                        break;
                    }
                    ss << SEP << host << SEP << port;
                    return this->at("SOCKA", ss.str());
                }

                std::string httpd_work_mode() const { return this->at("WKMODE", "HTTPD"); }
                std::string httpd_host(const std::string &host, unsigned short port) const
                {
                    std::stringstream ss;
                    ss << host << SEP << port;
                    return this->at("HTPSV", ss.str());
                }
                std::string httpd_path(const std::string &path) const { return this->at("HTPURL", path + "[3F]"); }
                std::string httpd_request_header() const { return this->at("HTPHD", "Connection: close[0D][0A]"); }
                std::string httpd_over_time(size_t time) const
                {
                    std::stringstream ss;
                    ss << time;
                    return this->at("HTPTO", ss.str());
                }
                std::string httpd_header_filter(bool enable) const { return this->at("HTPFLT", enable ? "ON" : "OFF"); }
                std::string httpd_method(HttpdClientMethod method) const
                {
                    const std::string key = "HTPTP";
                    switch (method)
                    {
                    case GET:
                        return this->at(key, "GET");
                    case POST:
                        return this->at(key, "POST");
                    }
                }

                std::string at(const std::string key, const std::string val) const
                {
                    return this->at(key + "=" + val);
                }
                std::string at(const std::string query) const
                {
                    return this->command("AT+" + query);
                }
                std::string command(const std::string &message) const
                {
                    return this->password + message + "\r";
                }

            private:
                const std::string password;
                const static char SEP = ',';
            };
            class SMS
            {
            public:
                void net_transport();
                void httpd_client();
                void sms();

            private:
                std::vector<std::string> phones;
            };
            class SerialPort : public ashoka::tty::SerialPort
            {
            public:
                void mode(const std::string &password, const NetTransport &net, const Apn &apn);
                void mode(const std::string &password, const HttpdClient &net, const Apn &apn);
                void mode(const std::string &password, const std::string &phone);

            protected:
                void on_receive(const std::string &response)
                {
                    BOOST_LOG_TRIVIAL(info) << "TODO";
                }
                std::optional<std::pair<std::size_t, std::size_t>> match(const std::string &buf)
                {
                    // TODO
                    if (buf.length() > 10)
                    {
                        return std::make_pair(0, 10);
                    }
                    return std::nullopt;
                }
            };
        } // namespace usr_g768
    }     // namespace tty
} // namespace ashoka
#endif