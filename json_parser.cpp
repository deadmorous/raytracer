// json_parser.cpp

#include "json_parser.h"
#include "cxx_exception.h"

JsonParser::JsonParser(const QString& json) :
    m_json(json.trimmed())
{
    int pos = 0;
    m_parsedJson = parse(pos);
    if (pos != m_json.size())
        throwUnexpectedExtraInput();
}

QVariant JsonParser::toVariant() const
{
    return m_parsedJson;
}

QVariant JsonParser::operator()() const
{
    return m_parsedJson;
}

QVariant JsonParser::parse(int& pos) const
{
    skipWhitespace(pos);
    checkHasInput(pos);
    QChar c = m_json[pos];
    switch(c.toLatin1()) {
    case '{':
        return parseObject(pos);
    case '[':
        return parseArray(pos);
    case '"':
    case '\'':
        return parseString(pos);
    default: {
        // Try parse as boolean
        QRegExp rxBool("^(true|false)");
        if (rxBool.indexIn(m_json, pos, QRegExp::CaretAtOffset) == pos) {
            pos += rxBool.matchedLength();
            return rxBool.capturedTexts()[1] == "true";
        }

        // Try parse as number
        // http://www.regexlib.com/REDetails.aspx?regexp_id=185
        QRegExp rxNum("^[+-]?([0-9]*\\.?[0-9]+|[0-9]+\\.?[0-9]*)([eE][+-]?[0-9]+)?");
        if (rxNum.indexIn(m_json, pos, QRegExp::CaretAtOffset) == pos) {
            pos += rxNum.matchedLength();
            return rxNum.capturedTexts()[0].toDouble();
        }

        throwUnexpectedInput(pos);
        return QVariant();
    }
    }
}

QVariantMap JsonParser::parseObject(int& pos) const
{
    QVariantMap result;
    checkHasInput(pos);
    if (m_json[pos] != '{')
        throwUnexpectedInput(pos);
    ++pos;
    skipWhitespace(pos);
    forever {
        checkHasInput(pos);
        if (m_json[pos] == '}')
            break;
        QString name = parseName(pos);
        skipWhitespace(pos);
        checkHasInput(pos);
        if (m_json[pos] != ':')
            throwUnexpectedInput(pos);
        ++pos;
        QVariant value = parse(pos);
        result[name] = value;
        checkHasInput(pos);
        skipWhitespace(pos);
        if (m_json[pos] == '}')
            break;
        else if (m_json[pos] == ',') {
            ++pos;
            skipWhitespace(pos);
        }
        else
            throwUnexpectedInput(pos);
    }
    ++pos;
    return result;
}

QVariantList JsonParser::parseArray(int& pos) const
{
    QVariantList result;
    skipWhitespace(pos);
    checkHasInput(pos);
    if (m_json[pos] != '[')
        throwUnexpectedInput(pos);
    ++pos;
    skipWhitespace(pos);
    forever {
        checkHasInput(pos);
        if (m_json[pos] == ']')
            break;
        QVariant value = parse(pos);
        result << value;
        checkHasInput(pos);
        skipWhitespace(pos);
        if (m_json[pos] == ']')
            break;
        else if (m_json[pos] == ',') {
            ++pos;
            skipWhitespace(pos);
        }
        else
            throwUnexpectedInput(pos);
    }
    ++pos;
    return result;
}

QString JsonParser::parseString(int& pos) const
{
    QString result;
    skipWhitespace(pos);
    checkHasInput(pos);
    QChar quote = m_json[pos];
    if (quote != '"'   &&   quote != '\'')
        throwUnexpectedInput(pos);
    ++pos;
    int posEsc = -1;
    forever {
        checkHasInput(pos);
        if (posEsc == -1) {
            if (m_json[pos] == quote)
                break;
            else if (m_json[pos] == '\\') {
                posEsc = pos;
                ++pos;
            }
            else
                result += m_json[pos++];
        }
        else if (posEsc-pos == 1) {
            switch (m_json[pos].toLatin1()) {
            case 'r':   result += '\r';   ++pos;   posEsc = -1;   break;
            case 'n':   result += '\n';   ++pos;   posEsc = -1;   break;
            case 't':   result += '\t';   ++pos;   posEsc = -1;   break;
            case '\\':
            case '"':
            case '\'':
                result += m_json[pos++];   posEsc = -1;   break;
            default:
                throw cxx::exception("TODO: JsonParser::parseString()");
            }
        }
        else
            throw cxx::exception("TODO: JsonParser::parseString()");
    }
    ++pos;
    return result;
}

QString JsonParser::parseName(int& pos) const
{
    skipWhitespace(pos);
    checkHasInput(pos);
    if (m_json[pos] == '"')
        return parseString(pos);
    QRegExp rxName("^[_a-zA-Z$][_a-zA-Z0-9]*");
    if (rxName.indexIn(m_json, pos, QRegExp::CaretAtOffset) != pos)
        throwUnexpectedInput(pos);
    QString result = m_json.mid(pos, rxName.matchedLength());
    pos += rxName.matchedLength();
    return result;
}

void JsonParser::skipWhitespace(int& pos) const
{
    bool anythingSkipped;
    do {
        anythingSkipped = false;

        // Try to parse C comment
        QRegExp rxCComment("^/\\*.*\\*/");
        if (rxCComment.indexIn(m_json, pos, QRegExp::CaretAtOffset) == pos) {
            pos += rxCComment.matchedLength();
            anythingSkipped = true;
        }

        // Try to parse C++ comment
        QRegExp rxCppComment("^//[^\n]*(\n|$)");
        if (rxCppComment.indexIn(m_json, pos, QRegExp::CaretAtOffset) == pos) {
            pos += rxCppComment.matchedLength();
            anythingSkipped = true;
        }

        QRegExp rxWhitespace("^\\s*");
        int index = rxWhitespace.indexIn(m_json, pos, QRegExp::CaretAtOffset);
        Q_ASSERT(index == pos);
        Q_UNUSED(index);
        if (rxWhitespace.matchedLength() > 0) {
            anythingSkipped = true;
            pos += rxWhitespace.matchedLength();
        }
    }
    while(anythingSkipped);
}

void JsonParser::checkHasInput(int pos) const {
    if (pos >= m_json.size())
        throwUnexpectedEndOfInput();
}

void JsonParser::throwUnexpectedExtraInput() const {
    throw cxx::exception("Failed to parse JSON: unexpected extra input");
}

void JsonParser::throwUnexpectedEndOfInput() const {
    throw cxx::exception("Failed to parse JSON: unexpected end of input");
}

void JsonParser::throwUnexpectedInput(int pos) const {
    throw cxx::exception(
                std::string("Failed to parse JSON: unexpected input '") +
                m_json.mid(pos).toStdString() + "'");
}
