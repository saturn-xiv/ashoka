#ifndef ASHOKA_COMMON_H_
#define ASHOKA_COMMON_H_

#include <cassert>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <toml.hpp>
#include <pistache/endpoint.h>

#include "config.h"

#define APPLICATION_JSON_UTF8 "application/json; charset=UTF-8"
#define TEXT_HTML_UTF8 "text/html; charset=UTF-8"

#endif