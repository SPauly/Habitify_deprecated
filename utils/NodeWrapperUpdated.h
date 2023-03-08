#pragma once

#include <memory>
#include <thread>
#include <mutex>

#include "Habitify_protocol.pb.h"

namespace Habitify
{
    class NodeWrapper
    {
    public:
        NodeWrapper() 
            : node(std::make_unique<HabCom::Node>()) 
        {
        }
        virtual ~NodeWrapper() = default;

        NodeWrapper(const NodeWrapper &_copy)
        {
            std::scoped_lock lock(mux);
            node = std::make_unique<HabCom::Node>(*_copy.node);
        }

        NodeWrapper(NodeWrapper &&_move) noexcept
        {
            std::scoped_lock lock(mux);
            node = std::move(_move.node);
        }

        NodeWrapper &operator=(const NodeWrapper _copy)
        {
            std::scoped_lock lock(mux);
            node = std::make_unique<HabCom::Node>(*_copy.node);
            return *this;
        }

        NodeWrapper &operator=(NodeWrapper &&_move) noexcept
        {
            std::scoped_lock lock(mux);
            node = std::move(_move.node);
            return *this;
        }

    public:
    // accessors
        inline const std::string &get_name()
        {
            std::scoped_lock lock(mux);
            return node->name();
        }
        inline void set_name(const std::string& _value)
        {
            std::scoped_lock lock(mux);
            node->set_name(_value);
        }

        inline int32_t get_Id()
        {
            std::scoped_lock lock(mux);
            return node->id().id();
        }
        inline void set_Id(const int32_t& _value)
        {
            std::scoped_lock lock(mux);
            node->mutable_id()->set_id(_value);
        }

        inline int32_t get_min()
        {
            std::scoped_lock lock(mux);
            return node->min();
        }
        inline void set_min(const int32_t& _value)
        {
            std::scoped_lock lock(mux);
            node->set_min(_value);
        }

        inline int32_t get_max()
        {
            std::scoped_lock lock(mux);
            return node->max();
        }
        inline void set_max(const int32_t& _value)
        {
            std::scoped_lock lock(mux);
            node->set_max(_value);
        }

        inline int32_t get_pos_x()
        {
            std::scoped_lock lock(mux);
            return node->pos_x();
        }
        inline void set_pos_x(const int32_t& _value)
        {
            std::scoped_lock lock(mux);
            node->set_pos_x(_value);
        }

        inline int32_t get_pos_y()
        {
            std::scoped_lock lock(mux);
            return node->pos_y();
        }
        inline void set_pos_y(const int32_t& _value)
        {
            std::scoped_lock lock(mux);
            node->set_pos_y(_value);
        }


    private:
        std::mutex mux;
        std::unique_ptr<HabCom::Node> node;
    };
}