#ifndef ASHOKA_POOL_H_
#define ASHOKA_POOL_H_

#include <deque>
#include <exception>
#include <set>
#include <string>

#include <boost/log/trivial.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace ashoka
{
    namespace pool
    {
        class Connection
        {
        public:
            Connection(){};
            virtual ~Connection(){};
        };
        class Factory
        {
        public:
            virtual boost::shared_ptr<Connection> create() = 0;
            virtual std::string name() const = 0;
        };

        class Status
        {
        public:
            Status(size_t size, size_t used) : size(size), used(used) {}
            const size_t size;
            const size_t used;
        };

        template <typename T>
        class Pool
        {
        public:
            Pool(size_t size, boost::shared_ptr<Factory> factory);
            ~Pool() {}

            boost::shared_ptr<T> get()
            {
                boost::mutex::scoped_lock lock(this->locker);
                if (this->pool.size() == 0)
                {
                    for (std::set<boost::shared_ptr<Connection>>::iterator it = this->used.begin(); it != this->used.end(); ++it)
                    {

                        if ((*it).unique())
                        {

                            try
                            {
                                BOOST_LOG_TRIVIAL(debug) << "creating new connection to replace discarded connection";
                                boost::shared_ptr<Connection> con = this->factory->create();
                                this->used.erase(it);
                                this->used.insert(con);
                                return boost::static_pointer_cast<T>(con);
                            }
                            catch (std::exception &e)
                            {
                                BOOST_LOG_TRIVIAL(error) << e;
                                throw std::runtime_error("can't open new " + this->factory.name() + " connection");
                            }
                        }
                    }
                    throw std::out_of_range("pool " + this->factory.name() + " is full");
                }

                std::shared_ptr<Connection> con = this->pool.front();
                this->pool.pop_front();
                this->used.insert(con);
                return boost::static_pointer_cast<T>(con);
            }

            void release(boost::shared_ptr<T> con)
            {
                boost::mutex::scoped_lock lock(this->locker);
                this->pool.push_back(boost::static_pointer_cast<Connection>(con));
                this->borrowed.erase(con);
            }
            Status status()
            {
                boost::mutex::scoped_lock lock(this->locker);
                ashoka::pool::Status it(this->pool.size(), this->used.size());
                return it;
            }

        private:
            boost::shared_ptr<Factory> factory;
            size_t size;
            std::deque<boost::shared_ptr<Connection>> pool;
            std::set<boost::shared_ptr<Connection>> used;
            boost::mutex locker;
        };
    } // namespace pool
} // namespace ashoka
#endif