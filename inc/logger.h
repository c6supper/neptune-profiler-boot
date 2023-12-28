#ifndef LOGGER_H_
#define LOGGER_H_

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#define VerboseLogger() BOOST_LOG_TRIVIAL(debug)
#define WarningLogger() BOOST_LOG_TRIVIAL(warning)
#define InfoLogger() BOOST_LOG_TRIVIAL(info)
#define FatalLogger() BOOST_LOG_TRIVIAL(fatal)

#endif  // LOGGER_H_
