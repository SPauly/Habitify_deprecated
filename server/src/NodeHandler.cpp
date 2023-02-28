#include "NodeHandler.h"

namespace Habitify
{
    HabCom::Status NodeHandler::HandleNode(const HabCom::Node *request)
    {
        mvec_node_wrappers.push_back(std::make_shared<NodeWrapper>(request));
    }

    HabCom::Status NodeHandler::SaveNode()
    {
        //only temporary implementation for prove of concept
        auto temp = *mvec_node_wrappers.back().get();
        std::cout << "\t[NODE ID: " << temp->id().id() << "]" <<std::endl;
        std::cout << "\t\tName:     " << temp ->name() << std::endl;
        std::cout << "\t\tRelevance:    " << temp->relevance() << std::endl;
        std::cout << "\t\tType:    " << temp->type() << std::endl;
        std::cout << "\t\tContent:    ";
        switch (temp->type())
        {
        case HabCom::NodeType::BOOLEAN:
            std::cout << "(bool) = " << temp->boolean() <<std::endl;
            break;
        default:
            break;
        }

        HabCom::Status ret;
        ret.set_flag(1); 
        return ret;
    }
}