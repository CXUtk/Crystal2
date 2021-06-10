#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <map>
#include <vector>
#include <memory>

using ll = long long int;

enum class SJsonNodeType { JSON_NULL, JSON_BOOL, JSON_INT, JSON_FLOAT, JSON_STRING, JSON_ARRAY, JSON_OBJECT };
enum class SJsonErrorCode { SJSON_OK, SJOSN_INVALID_VALUE, SJSON_EXPECT_VALUE, SJSON_ROOT_NOT_SINGULAR };

/*

Context-free Grammar:

    JSON = <whitespace> <value> <whitespace>
    whitespace = ('\0' | '\n' | ' ' | '\r' | '\t')*
    value = <bool> | <null> | <number> | <string> | <array> | <object>
    bool = "true" | "false"
    null = "null"
    number = ([0-9]+)(.)?([0-9]*)? | (.)([0-9]+)
    string = \".*\"
    array = [ <whitespace> <items> <whitespace> ]
    items = <value> | (<value>, <whitespace> <items>)
    object = { <whitespace> <dicts> <whitespace> }
    dicts = (<string> <whitespace> ":" <JSON>) | ((<string> <whitespace> ":" <JSON>), <whitespace> <dicts>)

*/

class SJsonNode {
public:
    SJsonNode() = default;
    virtual ~SJsonNode() = 0 {}

    virtual SJsonNodeType GetType() const = 0;

    virtual ll GetInt() const { throw; }
    virtual double GetFloat() const { throw; }
    virtual bool GetBool() const { throw; }
    virtual std::string GetString() const { throw; }

    virtual std::vector<std::shared_ptr<SJsonNode>>::const_iterator begin() const { throw; }
    virtual std::vector<std::shared_ptr<SJsonNode>>::const_iterator end() const { throw; }
    virtual std::size_t arraySize() const { throw; }
    virtual std::shared_ptr<SJsonNode> GetArrayIndex(int i) const { throw; }

    virtual const std::shared_ptr<SJsonNode>& GetMember(const std::string& str) const { throw; }
};


class SJsonObjectNode : public SJsonNode {
public:
    SJsonObjectNode(const std::map<std::string, std::shared_ptr<SJsonNode>>& items) : _value(items) {}
    ~SJsonObjectNode() override {}

    SJsonNodeType GetType() const override { return SJsonNodeType::JSON_OBJECT; };
    const std::shared_ptr<SJsonNode>& GetMember(const std::string& str) const override {
        auto v = _value.find(str);
        if (v == _value.end()) throw;
        return v->second;
    }

private:
    std::map<std::string, std::shared_ptr<SJsonNode>> _value;
};


class SJsonArrayNode : public SJsonNode {
public:
    SJsonArrayNode(const std::vector<std::shared_ptr<SJsonNode>>& items) : _value(items) {}
    ~SJsonArrayNode() override {}

    SJsonNodeType GetType() const override { return SJsonNodeType::JSON_ARRAY; };

    std::size_t arraySize() const override { return _value.size(); }
    std::vector<std::shared_ptr<SJsonNode>>::const_iterator begin() const override {
        return _value.cbegin();
    }
    std::vector<std::shared_ptr<SJsonNode>>::const_iterator end() const override {
        return _value.cend();
    }

    std::shared_ptr<SJsonNode> GetArrayIndex(int i) const override {
        return _value[i];
    }

private:
    std::vector<std::shared_ptr<SJsonNode>> _value;
};

class SJsonNullNode : public SJsonNode {
public:
    SJsonNullNode() = default;
    ~SJsonNullNode() override {}

    SJsonNodeType GetType() const override { return SJsonNodeType::JSON_NULL; };
};

class SJsonBoolNode : public SJsonNode {
public:
    SJsonBoolNode(bool value) : SJsonNode(), _value(value) {}
    ~SJsonBoolNode() override {}

    SJsonNodeType GetType() const override { return SJsonNodeType::JSON_BOOL; };
    bool GetBool() const override { return _value; }

private:
    bool _value;
};

class SJsonIntNode : public SJsonNode {
public:
    SJsonIntNode(ll value) : SJsonNode(), _value(value) {}
    ~SJsonIntNode() override {}

    SJsonNodeType GetType() const override { return SJsonNodeType::JSON_INT; };
    ll GetInt() const override { return _value; }
    double GetFloat() const override { return (double)_value; }

private:
    ll _value;
};

class SJsonFloatNode : public SJsonNode {
public:
    SJsonFloatNode(double value) : SJsonNode(), _value(value) {}
    ~SJsonFloatNode() override {}

    SJsonNodeType GetType() const override { return SJsonNodeType::JSON_FLOAT; };
    double GetFloat() const override { return _value; }
    ll GetInt() const override { return (ll)_value; }

private:
    double _value;
};


class SJsonStringNode : public SJsonNode {
public:
    SJsonStringNode(const std::string& value) : SJsonNode(), _value(value) {}
    ~SJsonStringNode() override {}

    SJsonNodeType GetType() const override { return SJsonNodeType::JSON_STRING; };
    std::string GetString() const override { return _value; }

private:
    std::string _value;
};

std::shared_ptr<SJsonNode> SJsonParse(const std::string& text, SJsonErrorCode& err);

void SJsonFree(SJsonNode* node);

std::string SJsonGetNodeEnumName(SJsonNodeType type);

std::string SJsonGetErrEnumName(SJsonErrorCode code);