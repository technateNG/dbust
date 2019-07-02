#pragma once

#include <vector>
#include <poll.h>
#include "unit.hpp"

namespace dbust
{
    class UnitPool
    {
        std::vector<Unit> units;

        std::vector<::pollfd> polls;

    public:
        explicit UnitPool(std::size_t size);

        ::pollfd* get_polls();

        std::size_t size();

        std::vector<Unit>::iterator begin();

        std::vector<Unit>::iterator end();

        std::vector<Unit>::const_iterator begin() const;

        std::vector<Unit>::const_iterator end() const;

        Unit operator[](std::size_t pos) const;

        Unit& operator[](std::size_t pos);

        void initialize_unit(const ConnectionFlavour& flavour);
    };
}