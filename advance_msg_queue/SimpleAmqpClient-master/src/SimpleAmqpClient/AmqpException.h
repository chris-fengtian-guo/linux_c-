#ifndef SIMPLEAMQPCLIENT_AMQPEXCEPTION_H
#define SIMPLEAMQPCLIENT_AMQPEXCEPTION_H


#include <boost/cstdint.hpp>
#include <stdexcept>
#include <string>

#include "Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif




struct amqp_rpc_reply_t_;
struct amqp_channel_close_t_;
struct amqp_connection_close_t_;

namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT AmqpException : public std::runtime_error {
 public:
  
  static void Throw(const amqp_rpc_reply_t_ &reply);

  
  static void Throw(const amqp_channel_close_t_ &reply);

  
  static void Throw(const amqp_connection_close_t_ &reply);

  
  explicit AmqpException(const std::string &what, const std::string &reply_text,
                         boost::uint16_t class_id,
                         boost::uint16_t method_id) throw();

  
  virtual ~AmqpException() throw() {}

  
  virtual bool is_soft_error() const throw() = 0;

  
  virtual boost::uint16_t reply_code() const throw() = 0;

  
  virtual boost::uint16_t class_id() const throw() { return m_class_id; }

  
  virtual boost::uint16_t method_id() const throw() { return m_method_id; }

  
  virtual std::string reply_text() const throw() { return m_reply_text; }

 protected:
  
  std::string m_reply_text;
  boost::uint16_t m_class_id;
  boost::uint16_t m_method_id;
  
};


class SIMPLEAMQPCLIENT_EXPORT ConnectionException : public AmqpException {
 public:
  
  explicit ConnectionException(const std::string &what,
                               const std::string &reply_text,
                               boost::uint16_t class_id,
                               boost::uint16_t method_id) throw()
      : AmqpException(what, reply_text, class_id, method_id) {}

  virtual bool is_soft_error() const throw() { return false; }
};


class SIMPLEAMQPCLIENT_EXPORT ChannelException : public AmqpException {
 public:
  
  explicit ChannelException(const std::string &what,
                            const std::string &reply_text,
                            boost::uint16_t class_id,
                            boost::uint16_t method_id) throw()
      : AmqpException(what, reply_text, class_id, method_id) {}

  virtual bool is_soft_error() const throw() { return true; }
};


class SIMPLEAMQPCLIENT_EXPORT ConnectionForcedException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit ConnectionForcedException(const std::string &what,
                                     const std::string &reply_text,
                                     boost::uint16_t class_id,
                                     boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT InvalidPathException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit InvalidPathException(const std::string &what,
                                const std::string &reply_text,
                                boost::uint16_t class_id,
                                boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT FrameErrorException : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit FrameErrorException(const std::string &what,
                               const std::string &reply_text,
                               boost::uint16_t class_id,
                               boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT SyntaxErrorException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit SyntaxErrorException(const std::string &what,
                                const std::string &reply_text,
                                boost::uint16_t class_id,
                                boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT CommandInvalidException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit CommandInvalidException(const std::string &what,
                                   const std::string &reply_text,
                                   boost::uint16_t class_id,
                                   boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT ChannelErrorException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit ChannelErrorException(const std::string &what,
                                 const std::string &reply_text,
                                 boost::uint16_t class_id,
                                 boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT UnexpectedFrameException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit UnexpectedFrameException(const std::string &what,
                                    const std::string &reply_text,
                                    boost::uint16_t class_id,
                                    boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT ResourceErrorException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit ResourceErrorException(const std::string &what,
                                  const std::string &reply_text,
                                  boost::uint16_t class_id,
                                  boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT NotAllowedException : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit NotAllowedException(const std::string &what,
                               const std::string &reply_text,
                               boost::uint16_t class_id,
                               boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT NotImplementedException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit NotImplementedException(const std::string &what,
                                   const std::string &reply_text,
                                   boost::uint16_t class_id,
                                   boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT InternalErrorException
    : public ConnectionException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit InternalErrorException(const std::string &what,
                                  const std::string &reply_text,
                                  boost::uint16_t class_id,
                                  boost::uint16_t method_id) throw()
      : ConnectionException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT ContentTooLargeException
    : public ChannelException {
 public:
  
  static const boost::uint16_t REPLY_CODE;

  
  explicit ContentTooLargeException(const std::string &what,
                                    const std::string reply_text,
                                    boost::uint16_t class_id,
                                    boost::uint16_t method_id) throw()
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT NoRouteException : public ChannelException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit NoRouteException(const std::string &what,
                            const std::string reply_text,
                            boost::uint16_t class_id,
                            boost::uint16_t method_id) throw()
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT NoConsumersException : public ChannelException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit NoConsumersException(const std::string &what,
                                const std::string reply_text,
                                boost::uint16_t class_id,
                                boost::uint16_t method_id) throw()
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT AccessRefusedException : public ChannelException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit AccessRefusedException(const std::string &what,
                                  const std::string reply_text,
                                  boost::uint16_t class_id,
                                  boost::uint16_t method_id) throw()
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT NotFoundException : public ChannelException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit NotFoundException(const std::string &what,
                             const std::string reply_text,
                             boost::uint16_t class_id,
                             boost::uint16_t method_id) throw()
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT ResourceLockedException
    : public ChannelException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit ResourceLockedException(const std::string &what,
                                   const std::string reply_text,
                                   boost::uint16_t class_id,
                                   boost::uint16_t method_id) throw()
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};


class SIMPLEAMQPCLIENT_EXPORT PreconditionFailedException
    : public ChannelException {
 public:
  
  static const boost::uint16_t REPLY_CODE;
  
  explicit PreconditionFailedException(const std::string &what,
                                       const std::string reply_text,
                                       boost::uint16_t class_id,
                                       boost::uint16_t method_id) throw()
      : ChannelException(what, reply_text, class_id, method_id) {}

  virtual boost::uint16_t reply_code() const throw() { return REPLY_CODE; }
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
