#ifndef CALLABLE_HPP
#define CALLABLE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

template<typename R, typename... Args>
class callable {
public:
    using VarMap  = std::unordered_map<std::string, R>;
    using FuncMap = std::unordered_map<
        std::string,
        std::function<R(const std::vector<R>&)>
    >;

    callable(std::initializer_list<std::string> arg_names,
             const std::string& expr)
        : names(arg_names), expression(expr)
    {
        if (names.size() != sizeof...(Args))
            throw std::runtime_error("cantidad de argumentos incorrecta");
    }

    R operator()(Args... args) const {
        VarMap vars;
        R values[] = { static_cast<R>(args)... };

        for (size_t i = 0; i < names.size(); ++i)
            vars[names[i]] = values[i];

        return eval(vars);
    }

    static void register_func(
        const std::string& name,
        std::function<R(const std::vector<R>&)> fn
    ) {
        functions()[name] = fn;
    }

private:
    std::vector<std::string> names;
    std::string expression;

    static FuncMap& functions() {
        static FuncMap fmap;
        return fmap;
    }

    static R parse_expr(const std::string& expr, const VarMap& vars) {
        auto p1 = expr.find('(');
        auto p2 = expr.find(')');

        // llamada a función: f(a,b)
        if (p1 != std::string::npos && p2 != std::string::npos) {
            std::string fname = expr.substr(0, p1);
            std::string args  = expr.substr(p1 + 1, p2 - p1 - 1);

            std::vector<R> values;
            std::stringstream ss(args);
            std::string tok;

            while (std::getline(ss, tok, ',')) {
                tok.erase(
                    std::remove_if(tok.begin(), tok.end(), ::isspace),
                    tok.end()
                );
                values.push_back(vars.at(tok));
            }

            return functions().at(fname)(values);
        }

        // expresión binaria simple: x + y
        std::stringstream ss(expr);
        std::string a, b;
        char op;
        ss >> a >> op >> b;

        R L = vars.at(a);
        R Rv = vars.at(b);

        switch (op) {
            case '+': return L + Rv;
            case '-': return L - Rv;
            case '*': return L * Rv;
            case '/': return L / Rv;
            default: throw std::runtime_error("expresión inválida");
        }
    }

    R eval(const VarMap& vars) const {
        return parse_expr(expression, vars);
    }
};

#endif
