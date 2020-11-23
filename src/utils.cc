

#include "utils.h"

typedef boost::log::sinks::synchronous_sink<boost::log::sinks::syslog_backend>
    sink_t;

void ashoka::utils::init_logging(bool daemon, bool debug)
{
  boost::shared_ptr<boost::log::core> core = boost::log::core::get();
  if (daemon)
  {
    // https://www.boost.org/doc/libs/1_74_0/libs/log/doc/html/log/detailed/sink_backends.html#log.detailed.sink_backends.syslog
    boost::shared_ptr<boost::log::sinks::syslog_backend> backend(
        new boost::log::sinks::syslog_backend());

    boost::log::sinks::syslog::custom_severity_mapping<std::string> mapping(
        "MyLevel");
    mapping["debug"] = boost::log::sinks::syslog::debug;
    mapping["normal"] = boost::log::sinks::syslog::info;
    mapping["warning"] = boost::log::sinks::syslog::warning;
    mapping["failure"] = boost::log::sinks::syslog::critical;
    backend->set_severity_mapper(mapping);

    core->add_sink(boost::make_shared<sink_t>(backend));
  }

  core->set_filter(
      boost::log::trivial::severity >=
#ifdef NDEBUG
      (debug ? boost::log::trivial::debug : boost::log::trivial::info)
#else
      boost::log::trivial::trace
#endif
  );
}

void ashoka::utils::watchdog(int dur)
{
  int fd = open("/dev/watchdog", O_WRONLY);
  // ioctl(fd, WDIOC_SETTIMEOUT, 60);
  // ioctl(fd, WDIOC_SETPRETIMEOUT, 10);
  if (fd == -1)
  {
    BOOST_LOG_TRIVIAL(fatal) << "can't open watchdog";
    return;
  }

  int to, pt;
  ioctl(fd, WDIOC_GETTIMEOUT, &to);
  ioctl(fd, WDIOC_GETPRETIMEOUT, &pt);
  BOOST_LOG_TRIVIAL(info) << "watchdog: (" << to << ',' << pt << ") " << dur;
  while (true)
  {
#ifndef NDEBUG
    int left;
    ioctl(fd, WDIOC_GETTIMELEFT, &left);
    BOOST_LOG_TRIVIAL(debug) << "the timeout was is " << left << "seconds";
#endif
    ioctl(fd, WDIOC_KEEPALIVE, 0);
    sleep(dur);
  }
}

boost::posix_time::ptime ashoka::utils::str2time(const std::string &time, const std::string &format)
{
  const std::locale loc = std::locale(std::locale::classic(), new boost::posix_time::time_input_facet(format));
  std::istringstream is(time);
  is.imbue(loc);

  boost::posix_time::ptime t;
  is >> t;
  return t;
}

unsigned long ashoka::utils::str2ul(const std::string &s)
{
  unsigned long v;
  std::istringstream ss(s);
  ss >> v;
  return v;
}