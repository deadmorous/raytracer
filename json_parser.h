// json_parser.h

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QVariant>

class JsonParser
{
public:
    explicit JsonParser(const QString& json);
    QVariant toVariant() const;
    QVariant operator()() const;

private:
    QString m_json;
    QVariant m_parsedJson;
    QVariant parse(int& pos) const;
    QVariantMap parseObject(int& pos) const;
    QVariantList parseArray(int& pos) const;
    QString parseString(int& pos) const;
    QString parseName(int& pos) const;

    void skipWhitespace(int& pos) const;
    void checkHasInput(int pos) const;
    void throwUnexpectedExtraInput() const;
    void throwUnexpectedEndOfInput() const;
    void throwUnexpectedInput(int pos) const;
};

inline QVariant parseJson(const QString& json) {
    return JsonParser(json)();
}

#endif // JSONPARSER_H
