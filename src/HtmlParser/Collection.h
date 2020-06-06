#pragma once

#include <vector>

namespace HtmlParser
{

template <typename T>
 class Collection
 {
protected:
    std::vector<T> m_collection;
public:
    using c_it = typename std::vector<T>::iterator;
    using c_cit = typename std::vector<T>::const_iterator;

    Collection() = default;
    Collection(std::vector<T>&& collection) : m_collection(std::move(collection)) { }
    virtual ~Collection() = default;

    virtual c_it begin() { return m_collection.begin(); }
    [[nodiscard]] virtual c_cit begin() const { return m_collection.cbegin(); }
    virtual c_it end() { return m_collection.end(); }
    [[nodiscard]] virtual c_cit end() const { return m_collection.cend(); }

    virtual void clear() { m_collection.clear(); }
    [[nodiscard]] virtual bool empty() const { return m_collection.empty(); }

    [[nodiscard]] virtual const T& front() const { return m_collection.front(); }
    [[nodiscard]] virtual T& front() { return m_collection.front(); }
    [[nodiscard]] virtual const T& back() const { return m_collection.back(); }
    [[nodiscard]] virtual T& back() { return m_collection.back(); }

    virtual void insert(c_cit position, T item) { m_collection.insert(position, item); insert_trigger();  }
    virtual void insert(std::size_t position, T item) { m_collection.insert(m_collection.begin() + position, item); insert_trigger(); }
    virtual void insert(c_cit position, const std::vector<T>& items) { m_collection.insert(position, items.begin(), items.end()); insert_trigger(); }
    virtual void insert(c_cit position, const Collection<T>& items) { m_collection.insert(position, items.begin(), items.end()); insert_trigger(); }
    virtual void insert(std::size_t position, const std::vector<T>& items) { m_collection.insert(m_collection.begin() + position, items.begin(), items.end()); insert_trigger(); }

    virtual void push_back(const T& item) { m_collection.push_back(item);  insert_trigger(); }
    [[nodiscard]] virtual std::size_t size() const { return m_collection.size(); }
protected:
    virtual void insert_trigger() {}
 };
}
