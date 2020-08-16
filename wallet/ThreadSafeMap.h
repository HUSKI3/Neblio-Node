#ifndef THREADSAFEMAP_H
#define THREADSAFEMAP_H

#include <boost/optional.hpp>
#include <boost/thread.hpp>
#include <map>

template <typename K, typename V>
class ThreadSafeMap
{
    std::map<K, V>              theMap;
    mutable boost::shared_mutex mtx;

public:
    using MapType     = std::map<K, V>;
    using key_type    = K;
    using mapped_type = V;
    using value_type  = std::pair<K, V>;

    ThreadSafeMap();
    ThreadSafeMap(const std::map<K, V>& rhs);
    ThreadSafeMap(const ThreadSafeMap<K, V>& rhs);
    ThreadSafeMap<K, V>&      operator=(const ThreadSafeMap<K, V>& rhs);
    std::size_t               erase(const K& key);
    void                      set(const K& key, const V& value);
    [[nodiscard]] bool        exists(const K& key) const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool        empty() const;
    template <typename K_, typename V_>
    friend bool operator==(const ThreadSafeMap<K_, V_>& lhs, const ThreadSafeMap<K_, V_>& rhs);
    [[nodiscard]] boost::optional<V> get(const K& key) const;
    [[nodiscard]] boost::optional<V> front() const;
    [[nodiscard]] boost::optional<V> back() const;
    void                             clear();
    [[nodiscard]] std::map<K, V>     getInternalMap() const;
    void                             setInternalMap(const std::map<K, V>& TheMap);
};

template <typename K, typename V>
ThreadSafeMap<K, V>::ThreadSafeMap(const ThreadSafeMap<K, V>& rhs)
{
    boost::unique_lock<boost::shared_mutex> lock1(mtx);
    boost::shared_lock<boost::shared_mutex> lock2(rhs.mtx);
    this->theMap = rhs.theMap;
}

template <typename K, typename V>
ThreadSafeMap<K, V>& ThreadSafeMap<K, V>::operator=(const ThreadSafeMap<K, V>& rhs)
{
    boost::unique_lock<boost::shared_mutex> lock1(mtx);
    boost::shared_lock<boost::shared_mutex> lock2(rhs.mtx);
    this->theMap = rhs.theMap;
    return *this;
}

template <typename K_, typename V_>
bool operator==(const ThreadSafeMap<K_, V_>& lhs, const ThreadSafeMap<K_, V_>& rhs)
{
    if (&lhs == &rhs) {
        return true;
    }
    boost::shared_lock<boost::shared_mutex> lock1(lhs.mtx);
    boost::shared_lock<boost::shared_mutex> lock2(rhs.mtx);
    return (lhs.theMap == rhs.theMap);
}

template <typename K, typename V>
void ThreadSafeMap<K, V>::set(const K& key, const V& value)
{
    boost::unique_lock<boost::shared_mutex> lock(mtx);
    theMap[key] = value;
}

template <typename K, typename V>
boost::optional<V> ThreadSafeMap<K, V>::get(const K& key) const
{
    boost::shared_lock<boost::shared_mutex> lock(mtx);
    typename std::map<K, V>::const_iterator it = theMap.find(key);
    if (it == theMap.cend()) {
        return boost::none;
    } else {
        return boost::make_optional(it->second);
    }
}

template <typename K, typename V>
boost::optional<V> ThreadSafeMap<K, V>::front() const
{
    boost::shared_lock<boost::shared_mutex> lock(mtx);
    if (theMap.size() == 0) {
        return boost::none;
    }
    return boost::make_optional(*theMap.cbegin());
}

template <typename K, typename V>
boost::optional<V> ThreadSafeMap<K, V>::back() const
{
    boost::shared_lock<boost::shared_mutex> lock(mtx);
    if (theMap.size() == 0) {
        return boost::none;
    }
    return boost::make_optional(*theMap.crbegin());
}

template <typename K, typename V>
void ThreadSafeMap<K, V>::clear()
{
    boost::unique_lock<boost::shared_mutex> lock(mtx);
    theMap.clear();
}

template <typename K, typename V>
bool ThreadSafeMap<K, V>::exists(const K& key) const
{
    boost::shared_lock<boost::shared_mutex> lock(mtx);
    return (theMap.find(key) != theMap.end());
}

template <typename K, typename V>
std::size_t ThreadSafeMap<K, V>::size() const
{
    boost::shared_lock<boost::shared_mutex> lock(mtx);
    return theMap.size();
}

template <typename K, typename V>
bool ThreadSafeMap<K, V>::empty() const
{
    boost::shared_lock<boost::shared_mutex> lock(mtx);
    return theMap.empty();
}

template <typename K, typename V>
ThreadSafeMap<K, V>::ThreadSafeMap()
{
}

template <typename K, typename V>
ThreadSafeMap<K, V>::ThreadSafeMap(const std::map<K, V>& rhs)
{
    boost::unique_lock<boost::shared_mutex> lock(mtx);
    theMap = rhs;
}

template <typename K, typename V>
std::size_t ThreadSafeMap<K, V>::erase(const K& key)
{
    boost::unique_lock<boost::shared_mutex> lock(mtx);
    return theMap.erase(key);
}

template <typename K, typename V>
std::map<K, V> ThreadSafeMap<K, V>::getInternalMap() const
{
    boost::unique_lock<boost::shared_mutex> lock(mtx);
    std::map<K, V>                          safeCopy = theMap;
    return safeCopy;
}

template <typename K, typename V>
void ThreadSafeMap<K, V>::setInternalMap(const std::map<K, V>& TheMap)
{
    boost::unique_lock<boost::shared_mutex> lock(mtx);
    theMap = TheMap;
}

#endif // THREADSAFEMAP_H
