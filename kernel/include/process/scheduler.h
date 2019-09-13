#pragma once
#include <stdint.h>
#include <stddef.h>
#include <process/process.h>
#include <algo/type_trait.h>

namespace ps
{
    class Scheduler{
    public:
        virtual void tick() = 0;
        virtual void yield() = 0;
        virtual void enqueue(Task * t) = 0;
        virtual void dequeue(Task * t) = 0;
        virtual Task * current() = 0;
        //the argument must be enqueued in scheduler before calling the method.
        virtual void current(Task *) = 0;
        virtual ~Scheduler() noexcept {}
    
        static inline void setTaskNode(Task * t,void * ptr)
        {
            t->node_in_scheduler = ptr;
        }
        static inline void * getTaskNode(Task * t)
        {
            return t->node_in_scheduler;
        }
    };
    class NullScheduler : public Scheduler
    {
    public:
        virtual void tick(){}
        virtual void yield(){}
        virtual void enqueue(Task *){}
        virtual void dequeue(Task *){}
        virtual Task * current()
        {
            return get_queue(StateFlag::running).front_node()->getTask();
        }
        virtual void current(Task * t){
            get_queue(t->get_state()).pop_node(&t->stateNode());
            get_queue(StateFlag::running).push_front_node(&t->stateNode());
        }
    };

    using Scheduler_Deleter = util::add_pointer<void(Scheduler *)>::type;
    void init_scheduler();
    void change_scheduler(Scheduler * l,Scheduler_Deleter d_fn);
    Scheduler & current_scheduler();
    /*Constraint
    Processing io event in 20ms.
    Fairness.
    프로세스는 마감시간을 다 썼거나, yield 로 스스로 나갔거나 둘 중 하나
    */
    void start_scheduling(void (*init_entry)());
} // namespace ps

namespace process = ps;