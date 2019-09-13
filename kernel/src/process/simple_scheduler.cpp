#include <process/simple_scheduler.h>
#include <memory/mm.h>
#include <util/interrupt_lock.h>
#include <debug/debug.h>

using namespace ps;

ps::RoundRobinScheduler::RoundRobinScheduler(SchedulerTimer * t):timer(t)
{}

void ps::RoundRobinScheduler::tick()
{
    auto prev = next();
    (*prev)->switch_to_in_irq(*cur);
    timer->expired_after(5_ms);
    //debug << (*prev)->get_name() << (*cur)->get_name()<<" ";
}
void RoundRobinScheduler::yield()
{
    interrupt_lock lock;
    lock.lock();
    timer->expired_after(5_ms);
    auto prev = next();
    (*prev)->switch_to(*cur);
    lock.unlock();
}
void RoundRobinScheduler::enqueue(Task * t)
{
    auto n = new list_type::node_type;
    n->data = t;
    setTaskNode(t,(void *)n);
    task_list_lock.lock();
    task_list.push_front_node(n);
    task_list_lock.unlock();
}
void RoundRobinScheduler::dequeue(Task * t)
{
    auto n = (list_type::node_type *)getTaskNode(t);
    task_list_lock.lock();
    task_list.pop_node(n);
    task_list_lock.unlock();
    delete n;
}
RoundRobinScheduler::list_type::iterator RoundRobinScheduler::next()
{
    auto ret = cur;
    cur++;
    if(cur == task_list.end()){
        cur = task_list.begin();
    }
    return ret;
}