/*
 * argx.h
 * A simple command line argument parser
 * Created by: Eiko Sun
 * Version: 0.2.0
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
        [[nodiscard]] size_t arg_size() const { return _args.size(); }
        /**
         * Get the argument at the index or return the default value
         * @param index : index of the argument
         * @param def : default value
         * @return argument at the index or default value
         */
        [[nodiscard]] std::string arg_or_def(const int index, const std::string& def) {
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
        [[nodiscard]] size_t option_size() const { return _opts.size(); }
        /**
         * Get the option value of the key or return the default value
         * @param key : key of the option
         * @param def : default value
         * @return option value of the key or default value
         */
        [[nodiscard]] std::string option_or_def(const std::string& key, const std::string& def) {
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
        [[nodiscard]] std::string option_or_def(const string_il keys, const std::string& def) {
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
        [[nodiscard]] size_t flag_size() const { return _flags.size(); }
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

    inline ParseResult parse(const int argc, char **argv) {
        string_list arguments = {};
        options_map options = {};
        string_list flags = {};

        std::optional<std::string> previous = std::nullopt;

        for (int i = 0; i < argc; i++) {
            std::string target = argv[i];

            const size_t prefix = target.find_first_not_of('-');
            if (prefix == target.length()) continue;
            target.erase(0, prefix);

            if (prefix>=2) { // Flag
                previous = std::nullopt;
                flags.push_back(target);
            } else if (prefix==1) { // Option
                if (!options.contains(target))
                    options[target] = {};
                previous = target;
            } else { // Argument
                if (previous.has_value()) {
                    options[previous.value()].push_back(target);
                    previous = std::nullopt;
                }else {
                    arguments.push_back(target);
                }
            }
        }

        return {arguments, options, flags};
    }
}
