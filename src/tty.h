#ifndef ASHOKA_TTY_H_
#define ASHOKA_TTY_H_

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>

#define SERIAL_PORT_READ_BUF_SIZE 1 << 10

namespace ashoka
{
    namespace tty
    {
        class Handler
        {
        };
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
        }
    } // namespace tty
} // namespace ashoka
#endif