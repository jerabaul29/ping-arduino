#pragma once

#include "Stream.h"
#include "pingmessage_all.h"

class Ping1D
{
public:
  Ping1D(Stream& ser, uint32_t baudrate) : _stream ( ser )
  {
  }
  
  PingMessage* read()
  {
    while(_stream.available()) {
        if (_parser.parseByte(_stream.read()) == PingParser::NEW_MESSAGE) {
          handleMessage(&_parser.rxMsg);
          return &_parser.rxMsg;
        }
    }
    return nullptr;
  }
  
  size_t write(uint8_t* data, uint16_t length)
  {
    return _stream.write(data, length);
  }
  /**
  *
  *
  *
  *  *  *
  */
  bool initialize(uint16_t ping_interval_ms = 50)
  {
      if(!request(Ping1DNamespace::Device_id)) {
        return false;
      }

      if (!request(Ping1DNamespace::Fw_version)) {
        return false;
      }

      if(!request(Ping1DNamespace::Voltage_5)) {
        return false;
      }

      if (!request(Ping1DNamespace::Pcb_temperature)) {
        //return false;
      }

      // Configure ping interval
      //if (!set_ping_interval(ping_interval_ms) {
        //return false;
      //}
      
      return true;
  }
  
  PingMessage* waitReply(enum Ping1DNamespace::msg_ping1D_id id, uint16_t timeout_ms)
  {
      uint32_t tstart = millis();
      while (millis() < tstart + timeout_ms) {

        PingMessage* pmsg = read();

        if (!pmsg) {
          continue;
        }

        handleMessage(pmsg);

        if (pmsg->message_id() == Ping1DNamespace::Nack) {
          ping_msg_ping1D_nack nack(*pmsg);

          if (nack.nacked_id() == id) {
            return nullptr;
          }
        }

        if (pmsg->message_id() == id) {
          return pmsg;
        }
      }

      debug("timed out waiting for id %d", id);
      return nullptr;
  }
  
  void handleMessage(PingMessage* pmsg)
  {
    switch (pmsg->message_id()) {
      case Ping1DNamespace::Nack: {
          ping_msg_ping1D_nack nack(*pmsg);
          debug("got NACK (%d) %s", nack.nacked_id(), nack.nack_msg());
      }
          break;
      case Ping1DNamespace::Distance:
      {
          ping_msg_ping1D_distance m(*pmsg);
          _distance = m.distance();
          _confidence = m.confidence();
      }
          break;
      case Ping1DNamespace::Distance_simple:
      {
          ping_msg_ping1D_distance_simple m(*pmsg);
          _distance = m.distance();
          _confidence = m.confidence();
      }
          break;
      default:
          break;
    }
  }

  // ex ping_msg_ping1D_voltage_5 msg(*pd.request(Ping1DNamespace::Voltage_5));
  PingMessage* request(enum Ping1DNamespace::msg_ping1D_id id, uint16_t timeout_ms = 400)
  {
    ping_msg_ping1D_empty msg;
    msg.set_id(id);
    msg.updateChecksum();
    write(msg.msgData, msg.msgDataLength());
    return waitReply(id, timeout_ms);
  }
  
  // ex auto msg = pd.request<ping_msg_ping1D_voltage_5>();
  template <typename T>
  T* request() {
    T resp; // todo there should be some other (static) way to get the message id?
    static ping_msg_ping1D_empty req;
    req.set_id(resp.message_id());
    req.updateChecksum();
    write(req.msgData, req.msgDataLength());
    return (T*)waitReply(resp.message_id());
  }


  bool update() {
      return request(Ping1DNamespace::Distance_simple);
  }

  uint16_t distance() {
    return _distance;
  }

  uint16_t confidence() {
    return _confidence;
  }
  
private:
  Stream& _stream;
  PingParser _parser;

  uint16_t _distance;
  uint16_t _confidence;
};

