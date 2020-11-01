#ifndef ASHOKA_POOL_H_
#define ASHOKA_POOL_H_

#include "common.h"

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
            virtual std::shared_ptr<Connection> create() = 0;
            virtual std::string name() const = 0;
        };

        class Status
        {
        public:
            Status(size_t size, size_t used) : size(size), used(used) {}
            const size_t size;
            const size_t used;
        };

        template <class T>
        class Pool
        {
        public:
            Pool(size_t size, std::shared_ptr<Factory> factory) : size(size), factory(factory)
            {

                while (this->pool.size() < this->size)
                {
                    this->pool.push_back(this->factory->create());
                }
            }
            ~Pool() {}

            std::shared_ptr<T> get()
            {
                std::lock_guard<std::mutex> lock(this->locker);
                if (this->pool.size() == 0)
                {
                    for (std::set<std::shared_ptr<Connection>>::iterator it = this->used.begin(); it != this->used.end(); ++it)
                    {

                        if ((*it).unique())
                        {

                            try
                            {
                                BOOST_LOG_TRIVIAL(debug) << "creating new connection to replace discarded connection";
                                std::shared_ptr<Connection> con = this->factory->create();
                                this->used.erase(it);
                                this->used.insert(con);
                                return std::static_pointer_cast<T>(con);
                            }
                            catch (std::exception &e)
                            {
                                BOOST_LOG_TRIVIAL(error) << e;
                                throw std::runtime_error("can't open new " + this->factory->name() + " connection");
                            }
                        }
                    }
                    throw std::out_of_range("pool " + this->factory->name() + " is full");
                }

                std::shared_ptr<Connection> con = this->pool.front();
                this->pool.pop_front();
                this->used.insert(con);
                return std::static_pointer_cast<T>(con);
            }

            void release(std::shared_ptr<T> con)
            {
                std::lock_guard<std::mutex> lock(this->locker);
                this->pool.push_back(std::static_pointer_cast<Connection>(con));
                this->used.erase(con);
            }
            Status status()
            {
                std::lock_guard<std::mutex> lock(this->locker);
                ashoka::pool::Status it(this->pool.size(), this->used.size());
                return it;
            }

        private:
            const std::shared_ptr<Factory> factory;
            const size_t size;
            std::deque<std::shared_ptr<Connection>> pool;
            std::set<std::shared_ptr<Connection>> used;
            std::mutex locker;
        };
    } // namespace pool
} // namespace ashoka
#endif