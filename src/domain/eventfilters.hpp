#ifndef DOMAIN_EVENTFILTERS_HPP_INCLUDED
#define DOMAIN_EVENTFILTERS_HPP_INCLUDED

#include <functional>
#include <map>

namespace domain
{

using EventFilterId = unsigned int;

template <typename Event> class EventFilters
{
  public:
    EventFilterId installEventFilter(std::function<bool(const Event &)> event_filter)
    {
        EventFilterId id = m_next_id++;
        m_event_filters[id] = std::move(event_filter);
        return id;
    }

    void removeEventFilter(EventFilterId id)
    {
        m_event_filters.erase(id);
    }

    bool isFiltered(const Event &event)
    {
        auto copy = m_event_filters;
        for (auto &[id, filter] : copy)
        {
            if (filter(event))
            {
                return true;
            }
        }
        return false;
    }

  private:
    EventFilterId m_next_id = 0;
    std::map<EventFilterId, std::function<bool(const Event &)>> m_event_filters;
};

} // namespace domain

#endif
