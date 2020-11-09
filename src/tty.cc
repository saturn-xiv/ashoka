#include "tty.h"

void ashoka::tty::SerialPort::open(const std::string &name,
                                   boost::asio::serial_port::baud_rate baud_rate,
                                   boost::asio::serial_port::flow_control flow_control,
                                   boost::asio::serial_port::parity parity,
                                   boost::asio::serial_port::stop_bits stop_bits,
                                   boost::asio::serial_port_base::character_size character_size)
{
    if (this->port)
    {
        throw std::runtime_error("serial port " + name + " already open");
    }
    BOOST_LOG_TRIVIAL(info) << "open serial port " << name;

    this->port = std::shared_ptr<boost::asio::serial_port>(new boost::asio::serial_port(this->io_service));
    this->port->open(name);
    this->port->set_option(baud_rate);
    this->port->set_option(flow_control);
    this->port->set_option(parity);
    this->port->set_option(stop_bits);
    this->port->set_option(character_size);

    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
    this->read();
}

void ashoka::tty::SerialPort::close()
{
    std::lock_guard<std::mutex> lock(this->locker);
    if (port)
    {
        port->cancel();
        port->close();
        port.reset();
    }
    io_service.stop();
    io_service.reset();
}

void ashoka::tty::SerialPort::read()
{
    this->port->async_read_some(
        boost::asio::buffer(raw, SERIAL_PORT_READ_BUF_SIZE),
        boost::bind(&ashoka::tty::SerialPort::on_receive,
                    this, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void ashoka::tty::SerialPort::on_receive(const boost::system::error_code &ec, size_t len)
{
    std::lock_guard<std::mutex> lock(this->locker);

    if (ec)
    {
        BOOST_LOG_TRIVIAL(error) << "serial port read " << ec;
    }
    else
    {
        for (auto i = 0; i < len; i++)
        {
            this->buffer += raw[i];
        }
        for (;;)
        {
            auto match = this->match(this->buffer);
            if (!match)
            {
                break;
            }
            auto range = match.value();
            auto response = this->buffer.substr(range.first, range.second - range.first);
            BOOST_LOG_TRIVIAL(info) << "serial port receive(" << response.length() << ") " << response;
            this->on_receive(response);
            BOOST_LOG_TRIVIAL(debug) << "clear buffer(" << range.second << ") " << this->buffer.substr(0, range.second);
            this->buffer = this->buffer.substr(range.second);
            BOOST_LOG_TRIVIAL(debug) << this->buffer;
            // this->buffer.clear();
        }
    }
    this->read();
}

void ashoka::tty::SerialPort::write(const std::string &request)
{
    BOOST_LOG_TRIVIAL(info) << "write to serial port(" << request.length() << ") " << request;
    std::lock_guard<std::mutex> lock(this->locker);
    auto len = port->write_some(boost::asio::buffer(request));
    BOOST_LOG_TRIVIAL(info) << "write done " << len;
}
