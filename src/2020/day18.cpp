#include "day18.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <charconv>
#include <fmt/format.h>
#include <string_view>

using Int = int64_t;

static void Apply(Int &left, char op, Int right)
{
    if (op == '+')
        left += right;
    else if (op == '*')
        left *= right;
    else
        left = right;
}

static Int Eval1(std::string_view expression, char const *&parentC)
{
    Int value = 0;
    char op = '=';
    auto const *const first = &expression.front();
    auto const *const last = std::next(&expression.back());

    for (char const *c = first; c < last; std::advance(c, 1))
    {
        if (*c == ' ')
            continue;

        if (*c == '(')
        {
            auto const offset = static_cast<size_t>(std::distance(first, c) + 1);
            Apply(value, op, Eval1(expression.substr(offset), c));
        }
        else if (*c == ')')
        {
            parentC = c;
            return value;
        }
        else if (*c == '*' || *c == '+')
        {
            op = *c;
        }
        else
        {
            Int v = 0;
            if (auto result = std::from_chars(c, last, v); result.ec == std::errc{})
            {
                Apply(value, op, v);
            }
        }
    }

    return value;
}

static Int Eval1(std::string_view expression)
{
    char const *dummy = nullptr;
    return Eval1(expression, dummy);
}

enum class TokenType
{
    Int,
    Plus,
    Mul,
    LParen,
    RParen,
    End,
};

struct Token
{
    TokenType type;
    Int value;

    Token(TokenType type_)
        : type(type_)
        , value(0)
    {
    }

    Token(TokenType type_, Int value_)
        : type(type_)
        , value(value_)
    {
    }
};

class Lexer
{
    std::string_view text;
    int currentChar;
    size_t offset = 0;

public:
    explicit Lexer(std::string_view text_)
        : text(text_)
        , currentChar(text_[0])
    {
    }

private:
    void Advance()
    {
        ++offset;
        if (offset >= text.size())
            currentChar = EOF;
        else
            currentChar = text[offset];
    }

    bool SkipWhitespace()
    {
        bool skip = false;

        while (currentChar != EOF && std::isspace(currentChar) != 0)
        {
            Advance();
            skip = true;
        }

        return skip;
    }

    Int Integer()
    {
        size_t start = offset;
        while (currentChar != EOF && std::isdigit(currentChar) != 0)
            Advance();

        Int value = 0;
        auto const *const numStart = &text[start];
        auto const *const numEnd = std::next(numStart, static_cast<ptrdiff_t>(offset - start));
        auto result = std::from_chars(numStart, numEnd, value);
        if (result.ec == std::errc{})
            return value;

        throw std::runtime_error("not an integer");
    }

public:
    Token GetNextToken()
    {
        while (currentChar != EOF)
        {
            if (SkipWhitespace())
                continue;

            if (std::isdigit(currentChar) != 0)
                return {TokenType::Int, Integer()};

            if (currentChar == '+')
            {
                Advance();
                return TokenType::Plus;
            }

            if (currentChar == '*')
            {
                Advance();
                return TokenType::Mul;
            }

            if (currentChar == '(')
            {
                Advance();
                return TokenType::LParen;
            }

            if (currentChar == ')')
            {
                Advance();
                return TokenType::RParen;
            }

            throw std::runtime_error("Invalid");
        }

        return TokenType::End;
    }
};

struct AST
{
    AST() = default;
    AST(AST const &) = delete;
    AST(AST &&) = delete;
    AST &operator=(AST const &) = delete;
    AST &operator=(AST &&) = delete;
    virtual ~AST() noexcept = default;
    virtual Int Eval() = 0;
};

struct BinaryOperation : public AST
{
    std::shared_ptr<AST> left;
    char op;
    std::shared_ptr<AST> right;

    BinaryOperation(std::shared_ptr<AST> left_, char op_, std::shared_ptr<AST> right_)
        : left(std::move(left_))
        , op(op_)
        , right(std::move(right_))
    {
    }

    Int Eval() override
    {
        if (op == '*')
            return left->Eval() * right->Eval();

        return left->Eval() + right->Eval();
    }
};

struct Number : public AST
{
    Int value;

    explicit Number(Int n)
        : value(n)
    {
    }

    Int Eval() override
    {
        return value;
    }
};

class Parser
{
    Lexer lex;
    Token currentToken;

public:
    Parser(std::string_view text)
        : lex(text)
        , currentToken(lex.GetNextToken())
    {
    }

private:
    [[noreturn]] static void Error()
    {
        throw std::runtime_error("Invalid syntax");
    }

    void Eat(TokenType type)
    {
        if (currentToken.type == type)
            currentToken = lex.GetNextToken();
        else
            Error();
    }

    std::shared_ptr<AST> Factor()
    {
        auto token = currentToken;
        if (token.type == TokenType::Int)
        {
            Eat(TokenType::Int);
            return std::make_shared<Number>(token.value);
        }

        if (token.type == TokenType::LParen)
        {
            Eat(TokenType::LParen);
            auto node = Expr();
            Eat(TokenType::RParen);
            return node;
        }

        Error();
    }

    std::shared_ptr<AST> Term()
    {
        auto node = Factor();
        while (currentToken.type == TokenType::Plus)
        {
            Eat(TokenType::Plus);
            node = std::make_shared<BinaryOperation>(node, '+', Factor());
        }

        return node;
    }

    std::shared_ptr<AST> Expr()
    {
        auto node = Term();
        while (currentToken.type == TokenType::Mul)
        {
            Eat(TokenType::Mul);
            node = std::make_shared<BinaryOperation>(node, '*', Term());
        }

        return node;
    }

public:
    std::shared_ptr<AST> Parse()
    {
        return Expr();
    }
};

static Int Eval2(std::string_view expression)
{
    Parser p(expression);
    auto ast = p.Parse();
    return ast->Eval();
}

static Int Part1()
{
    Int result = 0;
    for (auto &&expr : input::equations)
    {
        result += Eval1(expr);
    }

    return result;
}

static Int Part2()
{
    Int result = 0;
    for (auto &&expr : input::equations)
    {
        result += Eval2(expr);
    }

    return result;
}

int main()
{
    // https://adventofcode.com/2020/day/18
    fmt::print("Day 18, 2020: Operation Order\n");

    Assert(51 == Eval1("1 + (2 * 3) + (4 * (5 + 6))"));
    Assert(26 == Eval1("2 * 3 + (4 * 5)"));
    Assert(437 == Eval1("5 + (8 * 3 + 9 + 3 * 4 * 3)"));
    Assert(12240 == Eval1("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"));
    Assert(13632 == Eval1("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"));

    Assert(51 == Eval2("1 + (2 * 3) + (4 * (5 + 6))"));
    Assert(46 == Eval2("2 * 3 + (4 * 5)"));
    Assert(1445 == Eval2("5 + (8 * 3 + 9 + 3 * 4 * 3)"));
    Assert(669060 == Eval2("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"));
    Assert(23340 == Eval2("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(7147789965219 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(136824720421264 == part2);
}
