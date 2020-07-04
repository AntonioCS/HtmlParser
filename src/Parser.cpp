#include <stdexcept>
#include <HtmlParser/Parser.h>
#include <HtmlParser/Element.h>

namespace HtmlParser
{
    Parser::Parser(const std::vector<Token>& tokens) : m_tokens(tokens)
    {
    }

    Document Parser::parse(const std::vector<Token>& tokens)
    {
        m_tokens = tokens;
        return parse();
    }

    Document Parser::parse()
    {
        const auto& tokens = m_tokens;
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

                    Element::Type elementType{ token.type == TokenType::TAG_SELF_CLOSE ? Element::Type::TAG_SELF_CLOSE : Element::Type::TAG };
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
                        std::string error_msg{ parent->getElementName() };
                        error_msg.insert(0, "No close tag for: ");

                        throw std::runtime_error{error_msg};
                    }
                }
                break;
                case TokenType::CODE:
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
