#ifndef PACKET_HPP
#define PACKET_HPP

#include <msgpack.hpp>
#include <vector>
#include "layer.hpp"

struct Packet {
    Packet();
    Packet(const msgpack::object &obj);

    uint64_t id;
    uint64_t ts_sec;
    uint32_t ts_nsec;
    uint32_t len;
    std::vector<unsigned char> payload;
    LayerList layers;

    std::unordered_set<std::string> history;
};

typedef std::vector<const Packet *> PacketList;

namespace msgpack
{
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
{
    namespace adaptor
    {

    template <>
    struct pack<Packet> {
        template <typename Stream>
        msgpack::packer<Stream> &operator()(msgpack::packer<Stream> &o, Packet const &v) const
        {
            o.pack_map(6);
            o.pack("id");
            o.pack(v.id);
            o.pack("ts_sec");
            o.pack(v.ts_sec);
            o.pack("ts_nsec");
            o.pack(v.ts_nsec);
            o.pack("len");
            o.pack(v.len);
            o.pack("payload");
            o.pack(v.payload);
            o.pack("layers");
            o.pack(v.layers);
            return o;
        }
    };

    template <>
    struct pack<PacketList> {
        template <typename Stream>
        msgpack::packer<Stream> &operator()(msgpack::packer<Stream> &o, PacketList const &v) const
        {
            o.pack_array(v.size());
            for (const Packet *pkt : v)
                o.pack(*pkt);
            return o;
        }
    };

    } // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

#endif
