#include <stdexcept>
#include "Parser.h"
#include "Document.h"
#include "Element.h"

namespace HtmlParser
{
 
    void Parser::parse(const std::vector<Token>& tokens)
    {
        m_parent_list.clear();
        m_position = 0;

        auto html = new Element{ "html" };
        //Element* parent = &html;
        Document doc{html};
        Token* token{nullptr};
        
        m_parent_list.push_back(html);

        while ((token = getToken())) {
            switch (token->type) {
                case TokenType::DOCTYPE:
                    doc.setDoctype(std::move(token->attributes));
                    break;
                case TokenType::TAG_SELF_CLOSE:
                {                   
                    auto ele = new Element{ token->tag };
                    processAttributes(ele, token);
                    
                    m_parent_list.back()->addChild(ele);

                }
                break;
                case TokenType::TAG_OPEN:
                {
                    auto ele = new Element{ token->tag };
                    processAttributes(ele,token);

                    m_parent_list.back()->addChild(ele);
                    m_parent_list.push_back(ele);
                }
                    break;
                case TokenType::TAG_CLOSE:
                    {
                        if (isCloseTagOfCurrentParent(token->tag)) {
                            m_parent_list.pop_back();
                        } else {
                            throw std::runtime_error{ "No close tag for: " + m_parent_list.back()->getElementName() };
                        }
                    }
                break;
                case TokenType::TEXT:
                {
                    auto ele = new Element{};
                    ele->setInnerText(token->tag);
                    m_parent_list.back()->addChild(ele);
                }
                break;
                default:
                    throw std::runtime_error{ "Unknown token type" };
            }

            m_position++;
        }
    }

    Token* Parser::getToken()
    {
        if (m_position < m_tokens.size()) 
            return &(m_tokens[m_position]);
        return nullptr;
    }

    bool Parser::isCloseTagOfCurrentParent(const std::string& cs)
    {
        return (m_parent_list.back()->getElementName() == cs);
    }

    void Parser::processAttributes(Element* ele, const Token* token)
    {
        if (token->attributes.empty()) {
            ele->processAttributeString(token->attributes);
        }
    }
}
