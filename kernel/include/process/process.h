#pragma once
#include <stdint.h>
#include <stddef.h>
#include <arch/context.h>
#include <process/pid.h>

#include <datastruc/string_view.h>
#include <datastruc/com_string.h>
#include <datastruc/list.h>

namespace ps
{
using priority_t = short;
enum class StateFlag
{
    created,
    suspended,
    running,
    asleep,
    ready,
    terminated,
    state_end,
};
using pid_t = int;

class Task
{
private:
    static Task *createKernelTaskTemplate();
public:
    class Builder
    {
    public:
    explicit Builder(void (*fn)());
    explicit Builder(int (*fn)(int argc,const char * argv[])
        ,int argc,const char * argv[]);
    Builder & name(util::string_view);
    Builder & stack(void * ptr , size_t sz);
    Builder & stack(size_t size);
    Task * build();
    private:
        explicit Builder();
        static uint64_t name_generator(){static uint64_t i = 0;return i++;};
    private:
        Task * ret;
        bool name_set = false;

    };
    friend class Scheduler;
    struct TaskNode : public util::list_detail::list_node_trait<TaskNode>
    {
        using elem = Task;
        Task * getTask();
        elem * getDataPtr() {return getTask();}
    };
    void switch_to_in_irq(Task *after);

    void switch_to(Task *other);
    static void context_switch_to(Task * t);

    void setPriority(priority_t p) { priority = p; }
    priority_t getPriority() const { return priority; }
    TaskNode & stateNode(){return state_node;}
    void change_state(StateFlag sf);

    const char * get_name()const{ return name.data();}
    pid_t get_pid()const {return id;}
    StateFlag get_state() const{return state;}
private:
    Task() :
        id(0),
        state(StateFlag::created),
        priority(0),
        use_fpu(false),
        fpu_context(nullptr) {}
private:
    pid_t id;
    util::com_string name;
    //either wait_queue, ready_queue, termniated_queue or suspended_queue
    TaskNode state_node;
    StateFlag state;
    uint64_t *stack_start;
    uint64_t stack_size;
    priority_t priority;
    void *node_in_scheduler;
    bool use_fpu;
    //if set, do not change page table mapping.
    bool kernelTask;
    x86_64::fpuContext *fpu_context;
    x86_64::cpuContext cpu_context;
};
inline Task * Task::TaskNode::getTask(){ 
    return reinterpret_cast<Task *>(
        reinterpret_cast<uint8_t *>(this) - offsetof(Task,state_node)
        );
}

using TaskList = typename util::list_detail::list_base<typename Task::Task ,typename Task::TaskNode >;

TaskList & get_queue(StateFlag);

} // namespace ps

namespace process = ps;