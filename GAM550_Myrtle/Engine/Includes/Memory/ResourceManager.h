/*!****************************************************************************
 * file  : ResoruceManager.h
 * brief : 
 * Resource Manager, load all asset and save it in map to prevent reallocation
 * author: Mykyta Skiba,Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <map>
#include <string>
#include <memory>

using namespace std;

template <typename Resource>
class ResourceManager {

private:

    typedef std::shared_ptr<Resource> ResourcePTR;

    static map<string, ResourcePTR> _resources;
    static std::mutex _mutex;

public:

    static ResourcePTR retrieve(string const& key) {
        std::lock_guard<std::mutex> lock(_mutex);
        typename std::map<string, ResourcePTR>::iterator it = _resources.find(key);
        if (it == _resources.end()) return nullptr;

        return (*it).second;
    }

    static ResourcePTR create(string const& key, ResourcePTR resource) {
        std::lock_guard<std::mutex> lock(_mutex);
        _resources.insert(std::make_pair(key, resource));
        return resource;
    }

    template <typename Functor>
    static void execute(Functor func) {
        typename std::map<string, ResourcePTR>::iterator it = _resources.begin();
        while (it != _resources.end()) {
            func((*it).second);
            ++it;
        }
    }


};

template <typename Resource>
map<string, typename ResourceManager<Resource>::ResourcePTR> ResourceManager<Resource>::_resources = map<string, ResourcePTR>();

template <typename Resource>
std::mutex ResourceManager<Resource>::_mutex;
