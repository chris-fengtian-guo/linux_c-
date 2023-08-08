#ifndef SIMPLEAMQPCLIENT_CHANNEL_H
#define SIMPLEAMQPCLIENT_CHANNEL_H


#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/utility/string_ref.hpp>
#include <boost/variant.hpp>
#include <string>
#include <vector>

#include "SimpleAmqpClient/BasicMessage.h"
#include "SimpleAmqpClient/Envelope.h"
#include "SimpleAmqpClient/Table.h"
#include "SimpleAmqpClient/Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT Channel : boost::noncopyable {
 public:
  
  typedef boost::shared_ptr<Channel> ptr_t;

  static const std::string
      EXCHANGE_TYPE_DIRECT;  
  static const std::string
      EXCHANGE_TYPE_FANOUT;                      
  static const std::string EXCHANGE_TYPE_TOPIC;  

  struct SIMPLEAMQPCLIENT_EXPORT OpenOpts {
    
    struct SIMPLEAMQPCLIENT_EXPORT BasicAuth {
      std::string username;
      std::string password;

      BasicAuth() {}
      BasicAuth(const std::string &username, const std::string &password)
          : username(username), password(password) {}
      bool operator==(const BasicAuth &) const;
    };

    
    struct SIMPLEAMQPCLIENT_EXPORT ExternalSaslAuth {
      std::string identity;

      ExternalSaslAuth() {}
      explicit ExternalSaslAuth(const std::string &identity)
          : identity(identity) {}
      bool operator==(const ExternalSaslAuth &) const;
    };

    
    struct SIMPLEAMQPCLIENT_EXPORT TLSParams {
      std::string client_key_path;   
      std::string client_cert_path;  
      std::string ca_cert_path;      
      bool verify_hostname;  
      bool verify_peer;      

      TLSParams() : verify_hostname(true), verify_peer(true) {}
      bool operator==(const TLSParams &) const;
    };

    std::string host;   
    std::string vhost;  
    int port;           
    int frame_max;      
    
    boost::variant<BasicAuth, ExternalSaslAuth> auth;
    
    boost::optional<TLSParams> tls_params;

    
    static OpenOpts FromUri(const std::string &uri);

    OpenOpts() : vhost("/"), port(5672), frame_max(131072) {}
    bool operator==(const OpenOpts &) const;
  };

  
  static ptr_t Open(const OpenOpts &opts);

  
  SAC_DEPRECATED("Channel::Create is deprecated. Use Channel::Open.")
  static ptr_t Create(const std::string &host = "127.0.0.1", int port = 5672,
                      const std::string &username = "guest",
                      const std::string &password = "guest",
                      const std::string &vhost = "/", int frame_max = 131072);

  
  SAC_DEPRECATED(
      "Channel::CreateSaslExternal is deprecated. Use Channel::Open.")
  static ptr_t CreateSaslExternal(const std::string &host = "127.0.0.1",
                                  int port = 5672,
                                  const std::string &identity = "guest",
                                  const std::string &vhost = "/",
                                  int frame_max = 131072);

  
  SAC_DEPRECATED("Channel::CreateSecure is deprecated. Use Channel::Open.")
  static ptr_t CreateSecure(const std::string &path_to_ca_cert = "",
                            const std::string &host = "127.0.0.1",
                            const std::string &path_to_client_key = "",
                            const std::string &path_to_client_cert = "",
                            int port = 5671,
                            const std::string &username = "guest",
                            const std::string &password = "guest",
                            const std::string &vhost = "/",
                            int frame_max = 131072,
                            bool verify_hostname_and_peer = true);

  
  SAC_DEPRECATED("Channel::CreateSecure is deprecated. Use Channel::Open.")
  static ptr_t CreateSecure(const std::string &path_to_ca_cert,
                            const std::string &host,
                            const std::string &path_to_client_key,
                            const std::string &path_to_client_cert, int port,
                            const std::string &username,
                            const std::string &password,
                            const std::string &vhost, int frame_max,
                            bool verify_hostname, bool verify_peer);

  
  SAC_DEPRECATED(
      "Channel::CreateSecureSaslExternal is deprecated. Use Channel::Open.")
  static ptr_t CreateSecureSaslExternal(const std::string &path_to_ca_cert,
                                        const std::string &host,
                                        const std::string &path_to_client_key,
                                        const std::string &path_to_client_cert,
                                        int port, const std::string &identity,
                                        const std::string &vhost, int frame_max,
                                        bool verify_hostname, bool verify_peer);

  
  SAC_DEPRECATED("Channel::CreateFromUri is deprecated. Use Channel::Open.")
  static ptr_t CreateFromUri(const std::string &uri, int frame_max = 131072);

  
  SAC_DEPRECATED(
      "Channel::CreateSecureFromUri is deprecated. Use Channel::Open.")
  static ptr_t CreateSecureFromUri(const std::string &uri,
                                   const std::string &path_to_ca_cert,
                                   const std::string &path_to_client_key = "",
                                   const std::string &path_to_client_cert = "",
                                   bool verify_hostname_and_peer = true,
                                   int frame_max = 131072);

 private:
  class ChannelImpl;

 public:
  explicit Channel(ChannelImpl *impl);
  virtual ~Channel();

  
  int GetSocketFD() const;

  
  bool CheckExchangeExists(boost::string_ref exchange_name);

  
  void DeclareExchange(
      const std::string &exchange_name,
      const std::string &exchange_type = Channel::EXCHANGE_TYPE_DIRECT,
      bool passive = false, bool durable = false, bool auto_delete = false);

  
  void DeclareExchange(const std::string &exchange_name,
                       const std::string &exchange_type, bool passive,
                       bool durable, bool auto_delete, const Table &arguments);

  
  void DeleteExchange(const std::string &exchange_name, bool if_unused = false);

  
  void BindExchange(const std::string &destination, const std::string &source,
                    const std::string &routing_key);

  
  void BindExchange(const std::string &destination, const std::string &source,
                    const std::string &routing_key, const Table &arguments);

  
  void UnbindExchange(const std::string &destination, const std::string &source,
                      const std::string &routing_key);

  
  void UnbindExchange(const std::string &destination, const std::string &source,
                      const std::string &routing_key, const Table &arguments);

  
  bool CheckQueueExists(boost::string_ref queue_name);

  
  std::string DeclareQueue(const std::string &queue_name, bool passive = false,
                           bool durable = false, bool exclusive = true,
                           bool auto_delete = true);

  
  std::string DeclareQueue(const std::string &queue_name, bool passive,
                           bool durable, bool exclusive, bool auto_delete,
                           const Table &arguments);

  
  std::string DeclareQueueWithCounts(const std::string &queue_name,
                                     boost::uint32_t &message_count,
                                     boost::uint32_t &consumer_count,
                                     bool passive = false, bool durable = false,
                                     bool exclusive = true,
                                     bool auto_delete = true);

  
  std::string DeclareQueueWithCounts(const std::string &queue_name,
                                     boost::uint32_t &message_count,
                                     boost::uint32_t &consumer_count,
                                     bool passive, bool durable, bool exclusive,
                                     bool auto_delete, const Table &arguments);

  
  void DeleteQueue(const std::string &queue_name, bool if_unused = false,
                   bool if_empty = false);

  
  void BindQueue(const std::string &queue_name,
                 const std::string &exchange_name,
                 const std::string &routing_key = "");

  
  void BindQueue(const std::string &queue_name,
                 const std::string &exchange_name,
                 const std::string &routing_key, const Table &arguments);

  
  void UnbindQueue(const std::string &queue_name,
                   const std::string &exchange_name,
                   const std::string &routing_key = "");

  
  void UnbindQueue(const std::string &queue_name,
                   const std::string &exchange_name,
                   const std::string &routing_key, const Table &arguments);

  
  void PurgeQueue(const std::string &queue_name);

  
  void BasicAck(const Envelope::ptr_t &message);

  
  void BasicAck(const Envelope::DeliveryInfo &info);

  
  void BasicAck(const Envelope::DeliveryInfo &info, bool multiple);

  
  void BasicReject(const Envelope::ptr_t &message, bool requeue,
                   bool multiple = false);

  
  void BasicReject(const Envelope::DeliveryInfo &info, bool requeue,
                   bool multiple = false);

  
  void BasicPublish(const std::string &exchange_name,
                    const std::string &routing_key,
                    const BasicMessage::ptr_t message, bool mandatory = false,
                    bool immediate = false);

  
  bool BasicGet(Envelope::ptr_t &message, const std::string &queue,
                bool no_ack = true);

  
  void BasicRecover(const std::string &consumer);

  
  std::string BasicConsume(const std::string &queue,
                           const std::string &consumer_tag = "",
                           bool no_local = true, bool no_ack = true,
                           bool exclusive = true,
                           boost::uint16_t message_prefetch_count = 1);

  
  std::string BasicConsume(const std::string &queue,
                           const std::string &consumer_tag, bool no_local,
                           bool no_ack, bool exclusive,
                           boost::uint16_t message_prefetch_count,
                           const Table &arguments);

  
  void BasicQos(const std::string &consumer_tag,
                boost::uint16_t message_prefetch_count);

  
  void BasicCancel(const std::string &consumer_tag);

  
  Envelope::ptr_t BasicConsumeMessage(const std::string &consumer_tag);

  
  Envelope::ptr_t BasicConsumeMessage(
      const std::vector<std::string> &consumer_tags);

  
  Envelope::ptr_t BasicConsumeMessage();

  
  bool BasicConsumeMessage(const std::string &consumer_tag,
                           Envelope::ptr_t &envelope, int timeout = -1);

  
  bool BasicConsumeMessage(const std::vector<std::string> &consumer_tags,
                           Envelope::ptr_t &envelope, int timeout = -1);

  
  bool BasicConsumeMessage(Envelope::ptr_t &envelope, int timeout = -1);

 private:
  static ChannelImpl *OpenChannel(const std::string &host, int port,
                                  const std::string &username,
                                  const std::string &password,
                                  const std::string &vhost, int frame_max,
                                  bool sasl_external);

  static ChannelImpl *OpenSecureChannel(const std::string &host, int port,
                                        const std::string &username,
                                        const std::string &password,
                                        const std::string &vhost, int frame_max,
                                        const OpenOpts::TLSParams &tls_params,
                                        bool sasl_external);

  
  boost::scoped_ptr<ChannelImpl> m_impl;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
