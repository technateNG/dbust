#include "unit_pool.hpp"

dbust::UnitPool::UnitPool(std::size_t size)
{
    units.reserve(size);
    polls.reserve(size);
}

::pollfd* dbust::UnitPool::get_polls()
{
    return polls.data();
}

std::vector<dbust::Unit, std::allocator<dbust::Unit>>::iterator dbust::UnitPool::begin()
{
    return units.begin();
}

std::vector<dbust::Unit, std::allocator<dbust::Unit>>::iterator dbust::UnitPool::end()
{
    return units.end();
}

std::vector<dbust::Unit, std::allocator<dbust::Unit>>::const_iterator dbust::UnitPool::begin() const
{
    return units.begin();
}

std::vector<dbust::Unit, std::allocator<dbust::Unit>>::const_iterator dbust::UnitPool::end() const
{
    return units.end();
}

dbust::Unit dbust::UnitPool::operator[](std::size_t pos) const
{
    return units[pos];
}

dbust::Unit& dbust::UnitPool::operator[](std::size_t pos)
{
    return units[pos];
}

std::size_t dbust::UnitPool::size()
{
    return units.size();
}

void dbust::UnitPool::initialize_unit(const dbust::ConnectionFlavour& flavour)
{
    Unit unit(flavour, polls[units.size()]);
    units.emplace_back(unit);
}
