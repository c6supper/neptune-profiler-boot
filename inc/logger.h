#ifndef LOGGER_H_
#define LOGGER_H_

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#define Verbose() BOOST_LOG_TRIVIAL(debug)
#define Warning() BOOST_LOG_TRIVIAL(warning)
#define Info() BOOST_LOG_TRIVIAL(info)

#endif  // LOGGER_H_
