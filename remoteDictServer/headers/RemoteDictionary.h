#ifndef REMOTE_DICTIONARY_H
#define REMOTE_DICTIONARY_H
#include<bits/stdc++.h>
#include <string.h>
#include <algorithm>
#include <utility>
#include <vector>
#include <mutex>
#include "communication.pb.h"

class RemoteDictionary
{
    using Dictionary = std::vector< std::vector<std::pair<std::string, std::string> > >;

    public:
        RemoteDictionary(size_t nrOfBuckets);   
        RemoteDictionary();     
       
        communication::Response set(const std::string& key, std::string&& value);
        communication::Response set(const std::string& key, const std::string& value);
        communication::Response get(const std::string& key);
        communication::Response stats();


    private:
        std::mutex mutex_protection;
        const size_t MAX_NR_BUCKETS;
        Dictionary buckets;
        std::atomic<size_t> getOperations = 0;
        std::atomic<size_t> successfulGetOperations = 0;
        std::atomic<size_t> failedGetOperations = 0;
        

        size_t hashFunction(const std::string& s);
};

#endif