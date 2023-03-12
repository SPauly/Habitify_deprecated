#pragma once

#include <memory>
#include <thread>
#include <mutex>

#include "Habitify_protocol.pb.h"

namespace Habitify
{
    //Wrapper for the HabCom::Node message type
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

        //for assigning new nodes to it
        NodeWrapper &operator=(HabCom::Node &&_move) noexcept
        {
            std::scoped_lock lock(mux);
            node = std::make_unique<HabCom::Node>(std::move(_move));
            return *this;
        }

    public:
    // accessors
        inline const HabCom::Node* get_node()
        {
            std::scoped_lock lock(mux);
            return node.get(); //should we return a copy here instead?
        }

        inline std::unique_ptr<MutableNodeType> mutable_node()
        {
            std::unique_lock<std::mutex> lock(mux);
            return std::make_unique<MutableNodeType>(*node);
        }

        inline std::mutex& get_mutex()
        {
            return mux;
        }

    // accessors for node methods
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

    //Provide a Mutable Node type; this is essentialy used to provide the mutable functionality protobuf does not implement
    struct MutableNodeType
    {
        MutableNodeType() = delete;
        MutableNodeType(const HabCom::Node* _node) 
        {
            //copy data from _node to mutable version
            _name = _node->name();
            _id = _node->id().id();
            _min = _node->min();
            _max = _node->max();
            _pos_x = _node->pos_x();
            _pos_y = _node->pos_y();
            _colx = _node->color().x();
            _coly = _node->color().y();
            _colz = _node->color().z();
            _colw = _node->color().w();
            _relevance = _node->relevance();
            _type = _node->type();
            _presentation = _node->presentation();
            if(_node->data_size() > 0)
            {
                _time = _node->data().end()->time();
                if(_node->data().end()->has_boolean())
                    _boolean = new bool(_node->data().end()->boolean());
                if(_node->data().end()->has_number())
                    _number = new float(_node->data().end()->number());
                if(_node->data().end()->has_text())
                    _text = new std::string(_node->data().end()->text());
            }
        }
        
        ~MutableNodeType()
        {
            //destroy all dynamically allocated data
            if(_boolean)
                delete _boolean;
            if(_number)
                delete _number;
            if(_text)
                delete _text;
        }

        std::string _name{""};
        int32_t _id;
        int32_t _min, _max;
        int32_t _pos_x, _pos_y;
        int32_t _colx, _coly, _colz, _colw;
        int32_t _relevance, _type, _presentation;

        HabCom::Timestamp _time;
        bool* _boolean;
        float* _number;
        std::string* _text;
    };

    //This lock handles thread safe access to a mutable node
    class MutableNodeLock
    {
    public:
        MutableNodeLock(NodeWrapper& _wrapper) 
            : node_wrapper(_wrapper), mutable_node_ptr(_wrapper.mutable_node())
        {

        }

        ~MutableNodeLock()
        {
            if(mutable_node_ptr)
            {
                std::scoped_lock lock(node_wrapper.get_mutex());
                //let the wrapper handle the updated version
            }
        }

        MutableNodeType* operator->() const {
            return mutable_node_ptr.get();
        }

            
    
    private:
        NodeWrapper &node_wrapper;
        std::unique_ptr<MutableNodeType> mutable_node_ptr;
    };

    //This lock provides thread safe access to the data of a Node message
    class MutableNodeDataLock
    {

    };
}