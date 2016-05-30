/*
 * Gravity simulation engine
 * Copyright (C) 2016  Michał Walenciak <MichalWalenciak@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef SIMULATIONENGINE_HPP
#define SIMULATIONENGINE_HPP


#include <cmath>
#include <iostream>
#include <vector>


struct XY
{
    double x;
    double y;

    XY(double _x, double _y): x(_x), y(_y) {}
    XY(): x(0.0), y(0.0) {}

    XY operator-(const XY& other) const
    {
        XY result = *this;
        result.x -= other.x;
        result.y -= other.y;

        return result;
    }

    XY& operator*=(double v)
    {
        x *= v;
        y *= v;

        return *this;
    }

    XY operator*(double v) const
    {
        XY result(x, y);
        result *= v;

        return result;
    }

    XY& operator+=(const XY& other)
    {
        x += other.x;
        y += other.y;

        return *this;
    }

    XY operator+(const XY& other) const
    {
        XY result(x, y);
        result += other;

        return result;
    }

    XY operator-() const
    {
        XY result(-x, -y);

        return result;
    }

    XY operator/(double v) const
    {
        XY result = *this;
        result.x /= v;
        result.y /= v;

        return result;
    }

};


class Object
{
        XY m_pos;
        XY m_v;
        double m_mass;
        double m_radius;

        int m_id;

    public:
        Object(double x, double y, double m, double r, double v_x = 0.0, double v_y = 0.0):
            m_pos(x, y),
            m_v(v_x, v_y),
            m_mass(m),
            m_radius(r),
            m_id(0)
        {

        }

        double mass() const
        {
            return m_mass;
        }

        double radius() const
        {
            return m_radius;
        }

        const XY& pos() const
        {
            return m_pos;
        }

        const XY& velocity() const
        {
            return m_v;
        }

        int id() const
        {
            return m_id;
        }

        void setId(int id)
        {
            m_id = id;
        }

        void setMass(double m)
        {
            m_mass = m;
        }

        void setRadius(double r)
        {
            m_radius = r;
        }

        void setVelocity(const XY& v)
        {
            m_v = v;
        }

        void setPos(const XY& p)
        {
            m_pos = p;
        }
};


struct ISimulationEvents
{
    virtual ~ISimulationEvents() {}

    virtual void objectsColided(const Object &, const Object &) = 0;        // first id is for object which became bigger, second id is for object that was annihilated
    virtual void objectCreated(int id, const Object &) = 0;
    virtual void objectAnnihilated(const Object &) = 0;
    virtual void objectUpdated(int id, const Object &) = 0;
};


class SimulationEngine
{
    public:
        SimulationEngine();
        SimulationEngine(const SimulationEngine &) = delete;
        ~SimulationEngine();

        SimulationEngine& operator=(const SimulationEngine &) = delete;

        void addEventsObserver(ISimulationEvents *);

        int addObject(const Object &);
        void stepBy(double);
        double step();

        const std::vector<Object>& objects() const;

    private:
        std::vector<Object> m_objects;
        std::vector<ISimulationEvents *> m_eventObservers;
        double m_dt;
        int m_nextId;

        std::size_t collide(std::size_t, std::size_t);
        void checkForCollisions();

        std::vector<XY> calculateForces() const;
        std::vector<XY> calculateVelocities(const std::vector<XY> &, double) const;
};

#endif // SIMULATIONENGINE_HPP
