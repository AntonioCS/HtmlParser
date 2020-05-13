#include <stdexcept>
#include "Parser.h"
#include "Element.h"

namespace HtmlParser
{
 
    Document Parser::parse(const std::vector<Token>& tokens)
    {
        std::string doctype{};
        Element root{ "html" };
        Element* parent{ &root };

        for (const auto& token : tokens) {
            switch (token.type) {
                case TokenType::DOCTYPE:
                    doctype = token.attributes;
                    break;
                case TokenType::TAG_SELF_CLOSE:
                case TokenType::TAG_OPEN:
                {
                    //we already have html
                    if (token.tag == "html") {
                        continue;
                    }

                    Element::Type elementType{ token.type == TokenType::TAG_SELF_CLOSE ? Element::Type::SELF_CLOSE : Element::Type::NORMAL };
                    Element ele{ elementType, parent, token.tag, token.attributes};

                    parent->addChild(std::move(ele));

                    if (token.type != TokenType::TAG_SELF_CLOSE)
                        parent = parent->getLastChild();
                }
                break;
                case TokenType::TAG_CLOSE:
                {
                    if (parent != nullptr && parent->getElementName() == token.tag) {
                        parent = parent->getParent();
                    } else {
                        std::string error_msg{ parent->getLastChild()->getElementName() };
                        error_msg.insert(0, "No close tag for: ");

                        throw std::runtime_error{error_msg};
                    }
                }
                break;
                case TokenType::TEXT:
                {
                    Element ele{Element::Type::TEXT, parent, token.tag};

                    if (parent)
                        parent->addChild(std::move(ele));
                    else
                        throw std::runtime_error{ "No parent for element of type text" };
                }
                break;
                default:
                    throw std::runtime_error{ "Unknown token type" };
            }
        }

        return Document{
            std::move(root),
            doctype
        };
    }
}
