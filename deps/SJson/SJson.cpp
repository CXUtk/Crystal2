#include "SJson.h"
#include <cassert>

static std::string::const_iterator stringEnd;
static std::shared_ptr<SJsonNode> parse_number(std::string::const_iterator& it, SJsonErrorCode& code);
static std::shared_ptr<SJsonNode> parse_string(std::string::const_iterator& it, SJsonErrorCode& code);
static bool parse_null(std::string::const_iterator& it);
static std::shared_ptr<SJsonNode> parse_true(std::string::const_iterator& it, SJsonErrorCode& code);
static std::shared_ptr<SJsonNode> parse_false(std::string::const_iterator& it, SJsonErrorCode& code);
static std::shared_ptr<SJsonNode> parse_array(std::string::const_iterator& it, SJsonErrorCode& code);
static std::shared_ptr<SJsonNode> parse_object(std::string::const_iterator& it, SJsonErrorCode& code);
static std::shared_ptr<SJsonNode> parse_value(std::string::const_iterator& it, SJsonErrorCode& code);


static bool isEnd(std::string::const_iterator& it) {
    return stringEnd == it;
}

static void expect(std::string::const_iterator& it, char ch) {
    assert(it != stringEnd &&  *it == ch);
    it++;
}

static void strip_white_space(std::string::const_iterator& it) {
    while (it != stringEnd && (*it == ' ' || *it == '\n' || *it == '\r' || *it == '\t')) {
        it++;
    }
}

static std::shared_ptr<SJsonNode> parse_number(std::string::const_iterator& it, SJsonErrorCode& code) {
    // [-] [INT] (.) (FRAC) (EXP)
    code = SJsonErrorCode::SJSON_OK;
    int stage = 0;
    bool neg = false;

    if (it != stringEnd && *it == '-') {
        neg = true;
        it++;
    }

    static char intStr[30];
    int intIndex = 0;

    static char fracStr[30];
    int fracIndex = 0;

    static char expStr[30];
    int expIndex = 0;
    bool expNeg = false;
    bool signChecked = false;


    while (it != stringEnd) {
        if (stage == 0) {
            if (isdigit(*it)) {
                intStr[intIndex++] = *it;
            }
            else if (*it == '.') {
                stage = 1;
            }
            else if (tolower(*it) == 'e') {
                stage = 2;
            }
            else {
                break;
            }
            it++;
        }
        else if(stage == 1) {
            if (isdigit(*it)) {
                fracStr[fracIndex++] = *it;
            }
            else if (*it == '.') {
                code = SJsonErrorCode::SJOSN_INVALID_VALUE;
                return std::make_shared<SJsonNullNode>();
            }
            else if (tolower(*it) == 'e') {
                stage = 2;
            }
            else {
                break;
            }
            it++;
        }
        else {
            if (!signChecked) {
                if (it != stringEnd ) {
                    if (*it == '-') {
                        expNeg = true;
                        it++;
                    }
                    else if (*it == '+') {
                        expNeg = false;
                        it++;
                    }
                }
                signChecked = true;
            }
            if (isdigit(*it)) {
                expStr[expIndex++] = *it;
            }
            else if (*it == '.') {
                code = SJsonErrorCode::SJOSN_INVALID_VALUE;
                return std::make_shared<SJsonNullNode>();
            }
            else if (tolower(*it) == 'e') {
                code = SJsonErrorCode::SJOSN_INVALID_VALUE;
                return std::make_shared<SJsonNullNode>();
            }
            else {
                break;
            }
            it++;
        }
    }
    if (!intIndex && !fracIndex && !expIndex) {
        code = SJsonErrorCode::SJOSN_INVALID_VALUE;
        return std::make_shared<SJsonNullNode>();
    }
    if (intStr[0] == '0' && intIndex > 1) {
        code = SJsonErrorCode::SJOSN_INVALID_VALUE;
        return std::make_shared<SJsonNullNode>();
    }
    if (intIndex && !fracIndex && !expIndex) {
        ll value = 0;
        for (int i = 0; i < intIndex; i++) {
            value *= 10;
            value += intStr[i] - '0';
        }
        return std::make_shared<SJsonIntNode>(neg ? -value : value);
    }
    else {
        double value = 0;
        for (int i = 0; i < intIndex; i++) {
            value *= 10.0;
            value += intStr[i] - '0';
        }

        if (fracIndex) {
            double t = 10;
            for (int i = 0; i < fracIndex; i++) {
                value += (fracStr[i] - '0') / t;
                t *= 10;
            }
        }
        if (expIndex) {
            ll expV = 0;
            for (int i = 0; i < expIndex; i++) {
                expV *= 10;
                expV += expStr[i] - '0';
            }
            value *= std::pow(10.0, expNeg ? -expV : expV);
        }
        return std::make_shared<SJsonFloatNode>(neg ? -value : value);
    }
}

static std::shared_ptr<SJsonNode> parse_string(std::string::const_iterator& it, SJsonErrorCode& code) {
    code = SJsonErrorCode::SJSON_OK;
    std::string str;
    expect(it, '"');
    while (it != stringEnd && *it != '"') {
        if (*it == '\\') {
            it++;
            if (it == stringEnd) break;
            if (*it == '"') {
                str.push_back('\"');
            }
            else if (*it == '\\') {
                str.push_back('\\');
            }
            else if (*it == 'b') {
                str.push_back('\b');
            }
            else if (*it == 'f') {
                str.push_back('\f');
            }
            else if (*it == 'n') {
                str.push_back('\n');
            }
            else if (*it == 'r') {
                str.push_back('\r');
            }
            else if (*it == 't') {
                str.push_back('\t');
            }
            else {
                code = SJsonErrorCode::SJOSN_INVALID_VALUE;
                return std::make_shared<SJsonNullNode>();
            }
        }
        else {
            str.push_back(*it);
        }
        it++;
    }
    expect(it, '"');
    return std::make_shared<SJsonStringNode>(str);
}

static bool parse_null(std::string::const_iterator& it) {
    static const char nstr[5] = "null";
    for (int i = 0; i < 4; i++) {
        if (it == stringEnd || *it != nstr[i]) {
            return false;
        }
        it++;
    }
    return true;
}


static std::shared_ptr<SJsonNode> parse_true(std::string::const_iterator& it, SJsonErrorCode& code) {
    static const char trueStr[5] = "true";
    for (int i = 0; i < 4; i++) {
        if (it == stringEnd || *it != trueStr[i]) {
            code = SJsonErrorCode::SJOSN_INVALID_VALUE;
            return std::make_shared<SJsonNullNode>();
        }
        it++;
    }
    code = SJsonErrorCode::SJSON_OK;
    return std::make_shared<SJsonBoolNode>(true);
}

static std::shared_ptr<SJsonNode> parse_false(std::string::const_iterator& it, SJsonErrorCode& code) {
    static const char trueStr[6] = "false";
    for (int i = 0; i < 5; i++) {
        if (it == stringEnd || *it != trueStr[i]) {
            code = SJsonErrorCode::SJOSN_INVALID_VALUE;
            return std::make_shared<SJsonNullNode>();
        }
        it++;
    }
    code = SJsonErrorCode::SJSON_OK;
    return std::make_shared<SJsonBoolNode>(false);
}

static std::shared_ptr<SJsonNode> parse_array(std::string::const_iterator& it, SJsonErrorCode& code) {
    std::vector<std::shared_ptr<SJsonNode>> elements;
    code = SJsonErrorCode::SJSON_OK;
    expect(it, '[');
    strip_white_space(it);
    if (it != stringEnd && *it == ']') {
        it++;
        return std::make_shared<SJsonArrayNode>(elements);
    }
    while (it != stringEnd) {
        strip_white_space(it);
        auto e = parse_value(it, code);
        if (code != SJsonErrorCode::SJSON_OK) {
            return std::make_shared<SJsonNullNode>();
        }
        elements.push_back(e);
        strip_white_space(it);
        if (*it == ',') {
            it++;
        }
        else if (*it == ']') {
            it++;
            break;
        }
        else {
            code = SJsonErrorCode::SJSON_ROOT_NOT_SINGULAR;
            return std::make_shared<SJsonNullNode>();
        }
    }
    return std::make_shared<SJsonArrayNode>(elements);
}

static std::shared_ptr<SJsonNode> parse_object(std::string::const_iterator& it, SJsonErrorCode& code) {
    std::map<std::string, std::shared_ptr<SJsonNode>> objMap;
    code = SJsonErrorCode::SJSON_OK;
    expect(it, '{');
    strip_white_space(it);
    if (it != stringEnd && *it == '}') {
        it++;
        return std::make_shared<SJsonObjectNode>(objMap);
    }
    while (it != stringEnd) {
        strip_white_space(it);
        auto name = parse_string(it, code);
        strip_white_space(it);
        expect(it, ':');
        strip_white_space(it);
        auto e = parse_value(it, code);
        if (code != SJsonErrorCode::SJSON_OK) {
            return std::make_shared<SJsonNullNode>();
        }
        objMap.insert(std::pair<std::string, std::shared_ptr<SJsonNode>>(name->GetString(), e));
        strip_white_space(it);
        if (*it == ',') {
            it++;
        }
        else if (*it == '}') {
            it++;
            break;
        }
        else {
            code = SJsonErrorCode::SJSON_ROOT_NOT_SINGULAR;
            return std::make_shared<SJsonNullNode>();
        }
    }
    return std::make_shared<SJsonObjectNode>(objMap);
}


static std::shared_ptr<SJsonNode> parse_value(std::string::const_iterator& it, SJsonErrorCode& code) {
    code = SJsonErrorCode::SJSON_OK;
    if (it == stringEnd) {
        code = SJsonErrorCode::SJSON_EXPECT_VALUE;
        return std::make_shared<SJsonNullNode>();
    }
    auto ch = *it;
    if (ch == 'n') {
        code = parse_null(it) ? SJsonErrorCode::SJSON_OK : SJsonErrorCode::SJOSN_INVALID_VALUE;
        return std::make_shared<SJsonNullNode>();
    }
    else if (ch == 't') {
        return parse_true(it, code);
    }
    else if (ch == 'f') {
        return parse_false(it, code);
    }
    else if (ch == '"') {
        return parse_string(it, code);
    }
    else if (ch == '[') {
        return parse_array(it, code);
    }
    else if (ch == '{') {
        return parse_object(it, code);
    }
    else {
        return parse_number(it, code);
    }
}


std::shared_ptr<SJsonNode> SJsonParse(const std::string& text, SJsonErrorCode& err) {
    auto ptr = text.begin();
    stringEnd = text.end();
    strip_white_space(ptr);
    auto result = parse_value(ptr, err);
    strip_white_space(ptr);
    if (!isEnd(ptr) && err == SJsonErrorCode::SJSON_OK) {
        err = SJsonErrorCode::SJSON_ROOT_NOT_SINGULAR;
        return std::make_shared<SJsonNullNode>();
    }
    return result;
}

void SJsonFree(SJsonNode* node) {
    delete node;
}

std::string SJsonGetNodeEnumName(SJsonNodeType type) {
    switch (type) 	{
    case SJsonNodeType::JSON_NULL:
        return "NullNode";
    case SJsonNodeType::JSON_BOOL:
        return "BoolNode";
    case SJsonNodeType::JSON_INT:
        return "IntNode";
    case SJsonNodeType::JSON_FLOAT:
        return "FloatNode";
    case SJsonNodeType::JSON_STRING:
        return "StringNode";
    case SJsonNodeType::JSON_ARRAY:
        return "ArrayNode";
    case SJsonNodeType::JSON_OBJECT:
        return "ObjectNode";
    default:
        return "Unknown";
    }
}

std::string SJsonGetErrEnumName(SJsonErrorCode code) {
    switch (code) 	{
    case SJsonErrorCode::SJSON_OK:
        return "OK";
    case SJsonErrorCode::SJOSN_INVALID_VALUE:
        return "INVALID_VALUE";
    case SJsonErrorCode::SJSON_EXPECT_VALUE:
        return "EXPECT_VALUE";
    case SJsonErrorCode::SJSON_ROOT_NOT_SINGULAR:
        return "ROOT_NOT_SINGULAR";
    default:
        return "UNKNOWN";
    }
}
