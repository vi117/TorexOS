#include<process/scheduler.h>
#include<process/idletask.h>
#include<drivers/timer/pit.h>
#include<memory/address.h>
#include<algo/new.h>

static void do_nothing(ps::Scheduler *){}
static ps::Scheduler * cur = nullptr;
static ps::Scheduler_Deleter del = do_nothing;
ps::Scheduler & ps::current_scheduler()
{
    return *cur;
}

static ps::NullScheduler null_sched;

void ps::init_scheduler()
{
    cur = &null_sched;
    auto idle = createIdleTask();
    idle->change_state(StateFlag::ready);
}
#include<debug/debug.h>
#include<process/simple_scheduler.h>
void ps::change_scheduler(ps::Scheduler * sched, ps::Scheduler_Deleter d)
{
    del(cur);
    cur = sched;
    del = d;
    for (auto& it : get_queue(StateFlag::ready))
    {
        sched->enqueue(&it);
    }
}
extern char STACK_START;
void ps::start_scheduling(void (*init_entry)())
{
    init_scheduler();
    auto pit_timer = PIT::get_PIT();
    pit_timer->expired_after(5_ms);
    debug << sizeof(RoundRobinScheduler) <<" size\n";
    auto rrs = new ps::RoundRobinScheduler(pit_timer);
    change_scheduler(rrs,[](Scheduler * sched){delete sched;});
    auto t = Task::Builder(init_entry)
                .name("main")
                .stack(&STACK_START,1_MB)
                .build();
    current_scheduler().enqueue(t);
    current_scheduler().current(t);
    t->change_state(StateFlag::running);
    Task::context_switch_to(t);
}