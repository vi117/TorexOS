#pragma once
#include <process/scheduler.h>
#include <datastruc/list.h>
#include <process/sched_timer.h>
#include <util/interrupt_lock.h>

namespace ps
{
    //simple round robin scheduler
    class RoundRobinScheduler   : public Scheduler
    {
    public:
        RoundRobinScheduler(SchedulerTimer * t);
        virtual void tick() override;
        virtual void yield() override;
        virtual void enqueue(Task * t) override;
        virtual void dequeue(Task * t) override;
        virtual Task * current() override {return *cur;}
        virtual void current(Task * t) override
        {cur = list_type::iterator((list_type::node_type *)(getTaskNode(t)));}

        using list_type = util::list_base<Task *,util::list_detail::list_default_node<Task *>>;
    private:
        list_type::iterator next();
    private:
        list_type task_list;
        list_type::iterator cur;
        SchedulerTimer * timer;
        interrupt_lock task_list_lock;
    };
} // namespace ps
