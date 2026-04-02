#ifndef DOMAIN_OBSERVERLIST_HPP_INCLUDED
#define DOMAIN_OBSERVERLIST_HPP_INCLUDED

#include <functional>
#include <map>

namespace domain
{

using ObserverId = int;

template <typename... Args> class ObserverList
{
  public:
    ObserverId addObserver(std::function<void(Args...)> callback)
    {
        ObserverId id = m_next_id++;
        m_observers[id] = std::move(callback);
        return id;
    }

    void removeObserver(ObserverId id)
    {
        m_observers.erase(id);
    }

    void notify(Args... args) const
    {
        auto copy = m_observers;
        for (auto &[id, cb] : copy)
        {
            cb(args...);
        }
    }

  private:
    ObserverId m_next_id = 0;
    std::map<ObserverId, std::function<void(Args...)>> m_observers;
};

} // namespace domain

#endif
