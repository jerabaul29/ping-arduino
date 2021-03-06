// C++ implementation of the Blue Robotics 'Ping' binary message protocol

//~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!
// THIS IS AN AUTOGENERATED FILE
// DO NOT EDIT
//~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!

#pragma once

/// ping_message objects provide a wrapper over vector
/// for convenient access and packing of field data
///
/// These files are generated by the ping_generator.py
/// script found in this directory

#ifdef QT_CORE_LIB
#include <QObject>
#endif

namespace PingMessageNamespace {
#ifdef QT_CORE_LIB
    Q_NAMESPACE
#endif
    enum ping_message_id {
        Ack = 1,
        Nack = 2,
        AsciiText = 3,
        GeneralRequest = 6,
        DeviceInformation = 4,
        ProtocolVersion = 5,
    };
#ifdef QT_CORE_LIB
    Q_ENUM_NS(ping_message_id)
#endif
};

class ping_message
{
public:
    ping_message() : msgData { nullptr } {}

    ping_message(const ping_message &msg)
        : _bufferLength { msg.msgDataLength() }
        , msgData { (uint8_t*)malloc(sizeof(uint8_t) * _bufferLength) }
    {
        memcpy(msgData, msg.msgData, _bufferLength);
    }

    ping_message(const uint16_t bufferLength)
        : _bufferLength { bufferLength }
        , msgData { (uint8_t*)malloc(sizeof(uint8_t) * _bufferLength) }
    {}

    ping_message(const uint8_t* buf, const uint16_t length)
        : _bufferLength { length }
        , msgData { (uint8_t*)malloc(sizeof(uint8_t) * _bufferLength) }
    {
        memcpy(msgData, buf, _bufferLength);
    }

    ping_message& operator = (const ping_message &msg) {
        _bufferLength = msg.msgDataLength();
        if(msgData) free(msgData);
        msgData = (uint8_t*)malloc(sizeof(uint8_t) * _bufferLength);
        memcpy(msgData, msg.msgData, _bufferLength);
        return *this;
    }

    ~ping_message() { if(msgData) free(msgData); }

protected:
    uint16_t _bufferLength;

public:
    uint8_t* msgData;
    uint16_t bufferLength() const { return _bufferLength; }; // size of internal buffer allocation
    uint16_t msgDataLength() const { return headerLength + payload_length() + checksumLength; }; // size of entire message buffer (header, payload, and checksum)

    uint16_t payload_length()                const { return *(uint16_t*)(msgData + 2); } // This is ok only because alignment is static
    uint16_t message_id()                    const { return *(uint16_t*)(msgData + 4); } // This is ok only because alignment is static
    uint8_t  src_device_id()                 const { return *(msgData + 6); }
    uint8_t  dst_device_id()                 const { return *(msgData + 7); }
    uint8_t* payload_data(int offset=0)      const { return (msgData + 8 + offset); }
    uint16_t checksum()                      const { return *(uint16_t*)(msgData + msgDataLength() - checksumLength); }

    static const uint8_t headerLength = 8;
    static const uint8_t checksumLength = 2;

    bool verifyChecksum() const {
        if(msgDataLength() > bufferLength()) {
            return false;
        }
        return checksum() == calculateChecksum();
    }

    void updateChecksum() {
        *(uint16_t*)(msgData + msgDataLength() - checksumLength) = calculateChecksum();
    }

    uint16_t calculateChecksum() const {
        uint16_t checksum = 0;

        if(msgDataLength() <= bufferLength()) {
            for(uint32_t i = 0, data_size = msgDataLength() - checksumLength; i < data_size; i++) {
                checksum += static_cast<uint8_t>(msgData[i]);
            }
        }

        return checksum;
    }
};

class ping_message_empty : public ping_message
{
public:
    ping_message_empty(const ping_message& msg) : ping_message { msg } {}
    ping_message_empty(const uint8_t* buf, const uint16_t length) : ping_message { buf, length } {}
    ping_message_empty()
        : ping_message { 8 + 2 }
    {
        msgData[0] = 'B';
        msgData[1] = 'R';
        (uint16_t&)msgData[2] = 0; // payload size
        (uint16_t&)msgData[4] = 0; // ID
        msgData[6] = 0;
        msgData[7] = 0;
    }

    void set_id(const uint16_t id) { memcpy((msgData + 4 + 0), &id, 2); }
};

class ping_message_ack : public ping_message
{
public:
    ping_message_ack(const ping_message& msg) : ping_message { msg } {}
    ping_message_ack(const uint8_t* buf, const uint16_t length) : ping_message { buf, length } {}
    ping_message_ack()
        : ping_message { static_cast<uint16_t>(12) }
    {
        msgData[0] = 'B';
        msgData[1] = 'R';
        (uint16_t&)msgData[2] = 2; // payload size
        (uint16_t&)msgData[4] = 1; // ID
        msgData[6] = 0;
        msgData[7] = 0;
    }

    uint16_t acked_id() const { uint16_t d; memcpy(&d, (payload_data(0)), 2.0); return d; };
    void set_acked_id(const uint16_t acked_id) { memcpy((payload_data(0)), &acked_id, 2.0);};
};

class ping_message_nack : public ping_message
{
public:
    ping_message_nack(const ping_message& msg) : ping_message { msg } {}
    ping_message_nack(const uint8_t* buf, const uint16_t length) : ping_message { buf, length } {}
    ping_message_nack(uint16_t nack_message_length)
        : ping_message { static_cast<uint16_t>(12 + nack_message_length) }
    {
        msgData[0] = 'B';
        msgData[1] = 'R';
        (uint16_t&)msgData[2] = 2 + nack_message_length; // payload size
        (uint16_t&)msgData[4] = 2; // ID
        msgData[6] = 0;
        msgData[7] = 0;
    }

    uint16_t nacked_id() const { uint16_t d; memcpy(&d, (payload_data(0)), 2.0); return d; };
    void set_nacked_id(const uint16_t nacked_id) { memcpy((payload_data(0)), &nacked_id, 2.0);};
    char* nack_message() const { return (char*)(payload_data(2.0)); }
    void set_nack_message_at(const uint16_t i, const char data) { memcpy((payload_data(2.0 + i)), &data, 1); }
};

class ping_message_ascii_text : public ping_message
{
public:
    ping_message_ascii_text(const ping_message& msg) : ping_message { msg } {}
    ping_message_ascii_text(const uint8_t* buf, const uint16_t length) : ping_message { buf, length } {}
    ping_message_ascii_text(uint16_t ascii_message_length)
        : ping_message { static_cast<uint16_t>(10 + ascii_message_length) }
    {
        msgData[0] = 'B';
        msgData[1] = 'R';
        (uint16_t&)msgData[2] = 0 + ascii_message_length; // payload size
        (uint16_t&)msgData[4] = 3; // ID
        msgData[6] = 0;
        msgData[7] = 0;
    }

    char* ascii_message() const { return (char*)(payload_data(0)); }
    void set_ascii_message_at(const uint16_t i, const char data) { memcpy((payload_data(0 + i)), &data, 1); }
};

class ping_message_general_request : public ping_message
{
public:
    ping_message_general_request(const ping_message& msg) : ping_message { msg } {}
    ping_message_general_request(const uint8_t* buf, const uint16_t length) : ping_message { buf, length } {}
    ping_message_general_request()
        : ping_message { static_cast<uint16_t>(12) }
    {
        msgData[0] = 'B';
        msgData[1] = 'R';
        (uint16_t&)msgData[2] = 2; // payload size
        (uint16_t&)msgData[4] = 6; // ID
        msgData[6] = 0;
        msgData[7] = 0;
    }

    uint16_t requested_id() const { uint16_t d; memcpy(&d, (payload_data(0)), 2.0); return d; };
    void set_requested_id(const uint16_t requested_id) { memcpy((payload_data(0)), &requested_id, 2.0);};
};

class ping_message_device_information : public ping_message
{
public:
    ping_message_device_information(const ping_message& msg) : ping_message { msg } {}
    ping_message_device_information(const uint8_t* buf, const uint16_t length) : ping_message { buf, length } {}
    ping_message_device_information()
        : ping_message { static_cast<uint16_t>(16) }
    {
        msgData[0] = 'B';
        msgData[1] = 'R';
        (uint16_t&)msgData[2] = 6; // payload size
        (uint16_t&)msgData[4] = 4; // ID
        msgData[6] = 0;
        msgData[7] = 0;
    }

    uint8_t device_type() const { uint8_t d; memcpy(&d, (payload_data(0)), 1.0); return d; };
    void set_device_type(const uint8_t device_type) { memcpy((payload_data(0)), &device_type, 1.0);};
    uint8_t device_revision() const { uint8_t d; memcpy(&d, (payload_data(1.0)), 1.0); return d; };
    void set_device_revision(const uint8_t device_revision) { memcpy((payload_data(1.0)), &device_revision, 1.0);};
    uint8_t firmware_version_major() const { uint8_t d; memcpy(&d, (payload_data(2.0)), 1.0); return d; };
    void set_firmware_version_major(const uint8_t firmware_version_major) { memcpy((payload_data(2.0)), &firmware_version_major, 1.0);};
    uint8_t firmware_version_minor() const { uint8_t d; memcpy(&d, (payload_data(3.0)), 1.0); return d; };
    void set_firmware_version_minor(const uint8_t firmware_version_minor) { memcpy((payload_data(3.0)), &firmware_version_minor, 1.0);};
    uint8_t firmware_version_patch() const { uint8_t d; memcpy(&d, (payload_data(4.0)), 1.0); return d; };
    void set_firmware_version_patch(const uint8_t firmware_version_patch) { memcpy((payload_data(4.0)), &firmware_version_patch, 1.0);};
    uint8_t reserved() const { uint8_t d; memcpy(&d, (payload_data(5.0)), 1.0); return d; };
    void set_reserved(const uint8_t reserved) { memcpy((payload_data(5.0)), &reserved, 1.0);};
};

class ping_message_protocol_version : public ping_message
{
public:
    ping_message_protocol_version(const ping_message& msg) : ping_message { msg } {}
    ping_message_protocol_version(const uint8_t* buf, const uint16_t length) : ping_message { buf, length } {}
    ping_message_protocol_version()
        : ping_message { static_cast<uint16_t>(14) }
    {
        msgData[0] = 'B';
        msgData[1] = 'R';
        (uint16_t&)msgData[2] = 4; // payload size
        (uint16_t&)msgData[4] = 5; // ID
        msgData[6] = 0;
        msgData[7] = 0;
    }

    uint8_t version_major() const { uint8_t d; memcpy(&d, (payload_data(0)), 1.0); return d; };
    void set_version_major(const uint8_t version_major) { memcpy((payload_data(0)), &version_major, 1.0);};
    uint8_t version_minor() const { uint8_t d; memcpy(&d, (payload_data(1.0)), 1.0); return d; };
    void set_version_minor(const uint8_t version_minor) { memcpy((payload_data(1.0)), &version_minor, 1.0);};
    uint8_t version_patch() const { uint8_t d; memcpy(&d, (payload_data(2.0)), 1.0); return d; };
    void set_version_patch(const uint8_t version_patch) { memcpy((payload_data(2.0)), &version_patch, 1.0);};
    uint8_t reserved() const { uint8_t d; memcpy(&d, (payload_data(3.0)), 1.0); return d; };
    void set_reserved(const uint8_t reserved) { memcpy((payload_data(3.0)), &reserved, 1.0);};
};

