/*
 * argx.h
 * A simple command line argument parser
 * Created by: Eiko Sun
 * Version: 0.1.0
 * Last modified: 2025-01-03
 *
 * Example:
 *     argx arg0 arg1 -key value -option --flag
 */
#pragma once

#include <string>
#include <list>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <optional>
#include <utility>

typedef std::list<std::string> string_list;
typedef std::map<std::string, string_list> options_map;
typedef std::initializer_list<std::string> string_il;

namespace argx {

    class ParseResult {
    public:
        ParseResult(string_list args, options_map options, string_list flags):
        _args(std::move(args)), _opts(std::move(options)), _flags(std::move(flags)) {}

        /**
         * Get the size of arguments
         * @return size of arguments
         */
        [[nodiscard]] size_t argSize() const { return _args.size(); }
        /**
         * Get the argument at the index or return the default value
         * @param index : index of the argument
         * @param def : default value
         * @return argument at the index or default value
         */
        [[nodiscard]] std::string argOrDef(const int index, const std::string& def) {
            if ( index >= _args.size() ) return def;
            auto it = _args.begin();
            std::advance(it, index);
            return *it;
        }
        /**
         * Get the argument at the index
         * @param index : index of the argument
         * @return argument at the index
         * @throw std::out_of_range if index is out of range
         */
        [[nodiscard]] std::string argument(const int index) {
            if ( index >= _args.size() ) throw std::out_of_range("argx:ParseResult:Index out of range:"+std::to_string(index));
            auto it = _args.begin();
            std::advance(it, index);
            return *it;
        }
        /**
         * Get the list of arguments
         * @return list of arguments
         */
        [[nodiscard]] string_list args() const { return _args; }

        /**
         * Get the size of options
         * @return size of options
         */
        [[nodiscard]] size_t optionSize() const { return _opts.size(); }
        /**
         * Get the option value of the key or return the default value
         * @param key : key of the option
         * @param def : default value
         * @return option value of the key or default value
         */
        [[nodiscard]] std::string optionOrDef(const std::string& key, const std::string& def) {
            if(_opts.contains(key)) {
                return _opts[key].front();
            }
            return def;
        }
        /**
         * Get the option value of the key
         * @param key : key of the option
         * @return option value of the key
         * @throw std::out_of_range if key is not found
         */
        [[nodiscard]] std::string option(std::string key) {
            return option({std::move(key)});
        }
        /**
         * Get the option value of the key or return the default value
         * @param keys : list of keys
         * @param def : default value
         * @return option value of the key or default value
         */
        [[nodiscard]] std::string optionOrDef(const string_il keys, const std::string& def) {
            for(const auto& key : keys) {
                if(_opts.contains(key)) {
                    return _opts[key].front();
                }
            }
            return def;
        }
        /**
         * Get the option value of the key
         * @param keys : list of keys
         * @return option value of the key
         * @throw std::out_of_range if key is not found
         */
        [[nodiscard]] std::string option(const string_il keys) {
            for(const auto& key : keys) {
                if(_opts.contains(key)) {
                    return _opts[key].front();
                }
            }
            throw std::out_of_range("argx:ParseResult:Key not found");
        }
        /**
         * Get the list of options
         * @param key : key of the option
         * @return list of options
         */
        [[nodiscard]] string_list options(const std::string& key) {
            if(_opts.contains(key)) {
                return _opts[key];
            }
            return {};
        }
        /**
         * Get the list of options
         * @param keys : list of keys
         * @return list of options
         */
        [[nodiscard]] string_list options(const string_il keys) {
            string_list result = {};
            for(const auto& key : keys) {
                if(_opts.contains(key)) {
                    result.insert(result.end(), _opts[key].begin(), _opts[key].end());
                }
            }
            return result;
        }
        /**
         * Get the map of options
         * @return map of options
         */
        [[nodiscard]] options_map options() const { return _opts; }
        /**
         * Get the size of flags
         * @return size of flags
         */
        [[nodiscard]] size_t flagSize() const { return _flags.size(); }
        /**
         * Check if the flag exists
         * @param flag : flag to check
         * @return true if the flag exists
         */
        [[nodiscard]] bool flag(const std::string& flag) {
            return std::ranges::find(_flags, flag) != _flags.end();
        }
        /**
         * Get the list of flags
         * @return list of flags
         */
        [[nodiscard]] string_list flags() const { return _flags; }
    private:
        string_list _args;
        options_map _opts;
        string_list _flags;
    };

    class ParseResultBuilder {
    public:
        ParseResultBuilder()= default;
        void arg(const std::string& arg) {
            args.push_back(arg);
        }
        void option(const std::string& key, const std::string& value) {
            if(options.contains(key)) {
                options[key].push_back(value);
            }else {
                options[key] = {value};
            }
        }
        void option(const std::string& key) {
            if (!options.contains(key)) {
                options[key] = {};
            }
        }
        void flag(const std::string& flag) {
            flags.push_back(flag);
        }
        [[nodiscard]]
        ParseResult build() const {
            return {args, options, flags};
        }
    private:
        string_list args = {};
        options_map options = {};
        string_list flags = {};
    };

    inline ParseResult parse(const int argc, char **argv) {
        auto builder = ParseResultBuilder();
        std::optional<std::string> previous = std::nullopt;
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg[0] == '-' && arg[1] == '-') { // Flag
                if (previous.has_value()) {
                    builder.option(previous.value());
                    previous = std::nullopt;
                }
                builder.flag(arg.substr(2));
            } else if (arg[0] == '-') { // Option
                if (previous.has_value()) {
                    builder.option(previous.value());
                }
                previous = arg.substr(1);
            } else { // Argument
                if (previous.has_value()) {
                    builder.option(previous.value(),arg);
                    previous = std::nullopt;
                }else {
                    builder.arg(arg);
                }
            }
        }
        if (previous.has_value()) {
            builder.option(previous.value());
        }
        return builder.build();
    }
}
