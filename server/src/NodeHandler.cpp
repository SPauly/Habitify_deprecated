#include "NodeHandler.h"

namespace Habitify
{
    HabCom::Status NodeHandler::HandleNode(const HabCom::Node *request)
    {
        mvec_node_wrappers.push_back(std::make_shared<NodeWrapper>(request));
        
    }
}