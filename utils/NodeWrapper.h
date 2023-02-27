#pragma once

#include <memory>
#include <string>
#include <thread>

#include "Habitify_protocol.pb.h"

namespace Habitify
{
    struct _temp_mutable_copy
    {
        _temp_mutable_copy() = default;
        virtual ~_temp_mutable_copy()
        {
            if (_boolean)
                delete _boolean;
            if (_number)
                delete _number;
            if (_string)
                delete _string;
        }

        std::string _id{"New Node"};
        int _type, _relevance, _type_presentation;
        int _min = 0, _max = 0;
        int _color_x, _color_y, _color_z, _color_w;

        // data
        bool *_boolean = nullptr;
        float *_number = nullptr;
        std::string *_string = nullptr;
    };
    class NodeWrapper
    {
    public:
        // Constructor
        NodeWrapper()
        {
            mptr_node = std::make_unique<HabCom::Node>();
        };
        NodeWrapper(const HabCom::Node *_node, bool _use_temp = false)
            : b_use_temp_copy(_use_temp)
        {
            mptr_node = std::make_unique<HabCom::Node>(*_node);
            if (_use_temp)
                make_temp_copy();
        };
        // Destructor
        virtual ~NodeWrapper() = default;
        // copy constructor
        NodeWrapper(const NodeWrapper &_copy) : b_use_temp_copy(_copy.b_use_temp_copy)
        {
            mptr_node = std::make_unique<HabCom::Node>(*_copy.mptr_node);
            if (_copy.mptr_copy)
                mptr_copy = std::make_shared<_temp_mutable_copy>(*_copy.mptr_copy);
        }
        // copy assignement operator
        NodeWrapper &operator=(const NodeWrapper _copy)
        {
            b_use_temp_copy = _copy.b_use_temp_copy;
            mptr_node = std::make_unique<HabCom::Node>(*_copy.mptr_node);
            if (_copy.mptr_copy)
                mptr_copy = std::make_shared<_temp_mutable_copy>(*_copy.mptr_copy);
            return *this;
        }
        // Move constructor
        NodeWrapper(NodeWrapper &&other) noexcept
        {
            mptr_node = std::move(other.mptr_node);
            b_use_temp_copy = std::move(other.b_use_temp_copy);
            mptr_copy = std::move(other.mptr_copy);
        }
        // Move assignement operator
        NodeWrapper &operator=(NodeWrapper &&other) noexcept
        {
            if (this != &other)
            {
                mptr_node = std::move(other.mptr_node);
                b_use_temp_copy = std::move(other.b_use_temp_copy);
                mptr_copy = std::move(other.mptr_copy);
            }
            return *this;
        }

        HabCom::Node *operator->()
        {
            return mptr_node.get();
        }

        bool set_temp_copy(bool _flag)
        {

            if (!_flag)
                mptr_copy.reset();
            else
                make_temp_copy();
            return b_use_temp_copy = _flag;
        }

        std::shared_ptr<_temp_mutable_copy> get_mutable()
        {
            return mptr_copy;
        }

        HabCom::Node *get_sendable()
        {
            if (b_use_temp_copy)
                merge_temp();

            return mptr_node.get();
        }

    protected:
        void make_temp_copy()
        {

            if (!mptr_copy)
            {
                mptr_copy = std::make_shared<_temp_mutable_copy>();
            }

            mptr_copy->_id = mptr_node->name();
            mptr_copy->_type = mptr_node->type();
            mptr_copy->_relevance = mptr_node->relevance();
            mptr_copy->_type_presentation = mptr_node->type_representation();
            mptr_copy->_min = mptr_node->min();
            mptr_copy->_max = mptr_node->max();
            mptr_copy->_color_x = mptr_node->color().x();
            mptr_copy->_color_y = mptr_node->color().y();
            mptr_copy->_color_z = mptr_node->color().z();
            mptr_copy->_color_w = mptr_node->color().w();

            if (mptr_node->has_boolean())
                mptr_copy->_boolean = new bool(mptr_node->boolean());
            if (mptr_node->has_number())
                mptr_copy->_number = new float(mptr_node->number());
            if (mptr_node->has_text())
                mptr_copy->_string = new std::string(mptr_node->text());
        }

        void merge_temp()
        {
            if (!mptr_copy)
                return;

            mptr_node->set_name(mptr_copy->_id);
            mptr_node->set_type((HabCom::NodeType)mptr_copy->_type);
            mptr_node->set_relevance((HabCom::Relevance)mptr_copy->_relevance);
            mptr_node->set_type_representation((HabCom::NodeTypePresentation)mptr_copy->_type_presentation);
            mptr_node->set_min(mptr_copy->_min);
            mptr_node->set_max(mptr_copy->_max);
            mptr_node->mutable_color()->set_x(mptr_copy->_color_x);
            mptr_node->mutable_color()->set_y(mptr_copy->_color_y);
            mptr_node->mutable_color()->set_z(mptr_copy->_color_z);
            mptr_node->mutable_color()->set_w(mptr_copy->_color_w);

            if (mptr_node->has_boolean())
                mptr_node->set_boolean(mptr_copy->_boolean);
            if (mptr_node->has_number())
                mptr_node->set_number(*mptr_copy->_number);
            if (mptr_node->has_text())
                mptr_node->set_text(*mptr_copy->_string);
        }

    private:
        std::unique_ptr<HabCom::Node> mptr_node;
        bool b_use_temp_copy = false;
        std::shared_ptr<_temp_mutable_copy> mptr_copy;
    };
}