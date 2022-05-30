#include "RemoteDictionary.h"

    
RemoteDictionary::RemoteDictionary(size_t nrOfBuckets):MAX_NR_BUCKETS(nrOfBuckets) 
{
    buckets.resize(MAX_NR_BUCKETS);
}

RemoteDictionary::RemoteDictionary(): MAX_NR_BUCKETS(1024) 
{
    buckets.resize(MAX_NR_BUCKETS);
}

communication::Response RemoteDictionary::set(const std::string& key, std::string&& value)
{            
    std::lock_guard<std::mutex> lockGuard(mutex_protection);
    size_t position = hashFunction(key);
    communication::Response response;
    response.set_responsetype(communication::Response::TypeOfResponse::Response_TypeOfResponse_SET_RESPONSE);
    communication::Response::SetResponse* setResponse = new communication::Response::SetResponse;
    setResponse->set_success(true);

    try
    {
        buckets[position].emplace_back(key, move(value));        
        
    }
    catch (std::exception const& e) 
    {
        setResponse->set_success(false);
        setResponse->set_failure_message(e.what());        
    }    

    response.set_allocated_setresp(setResponse);
    return response;
}


communication::Response RemoteDictionary::set(const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> lockGuard(mutex_protection);
    size_t position = hashFunction(key);    
    communication::Response response;
    response.set_responsetype(communication::Response::TypeOfResponse::Response_TypeOfResponse_SET_RESPONSE);
    communication::Response::SetResponse* setResponse = new communication::Response::SetResponse;
    setResponse->set_success(true);

    try
    {
        buckets[position].emplace_back(key, value);
    }
    catch (std::exception const& e) 
    {
        setResponse->set_success(false);
        setResponse->set_failure_message(e.what());        
    }

    response.set_allocated_setresp(setResponse);
    return response;
}


// // std::string RemoteDictionary::RemoteDictionary::get(const std::string& key)
communication::Response RemoteDictionary::get(const std::string& key)
{
    communication::Response response;
    response.set_responsetype(communication::Response::TypeOfResponse::Response_TypeOfResponse_GET_RESPONSE);
    communication::Response::GetResponse* getResp = new communication::Response::GetResponse;
    size_t position = hashFunction(key);            
    auto it = std::find_if(buckets[position].begin(), buckets[position].end(), [&key](const std::pair<std::string, std::string>& arg) {
        return std::get<0>(arg) == key;
    });

    getOperations++;
    if(it != std::end(buckets[position]))
    {
        std::cout << "[RemoteDictionary] Key found , value " << std::get<1>(*it) << " " << std::get<0>(*it) << std::endl;
        getResp->set_success(true);
        getResp->set_value(std::get<1>(*it));
        successfulGetOperations++;
    }
    else
    {
        getResp->set_success(false);
        getResp->set_value("[Error] Key not found !");
        failedGetOperations++;
    }

    response.set_allocated_getresp(getResp);
    return response;
}

communication::Response RemoteDictionary::stats()
{    
    communication::Response resp;
    resp.set_responsetype(communication::Response::TypeOfResponse::Response_TypeOfResponse_STATS_RESPONSE);
    communication::Response::StatsResponse* statResp = new communication::Response::StatsResponse;
    statResp->set_nr_of_get_operations(getOperations);
    statResp->set_failed_get_operations(failedGetOperations);
    statResp->set_successful_get_operations(successfulGetOperations);
    resp.set_allocated_statresp(statResp);
    return resp;
}

size_t RemoteDictionary::hashFunction(const std::string& s)
{
    const char* ptr = s.c_str();
    size_t length = strlen(ptr);
    size_t sum = 0;

    for(int idx=0; idx < length; idx++)
    {
        sum += ptr[idx];
    }
    return sum % MAX_NR_BUCKETS;
}