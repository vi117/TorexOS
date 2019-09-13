#include<process/process.h>
#include<stringutil.h>
#include<util/interrupt_lock.h>

ps::Task::Builder::Builder()
:ret(createKernelTaskTemplate())
{
    auto & context = ret->cpu_context;
    context.cs = 0x8;   //gdt offset
    context.ds = 0x10;  //fix later.
    context.es = 0x10;
    context.fs = 0x10;
    context.gs = 0x10;
    context.ss = 0x10;
    context.RFLAG = 0x0200; // interrupt on.

    ret->stack_size = 4096;//defualt value.
    ret->stack_start = nullptr;
}

ps::Task::Builder::Builder(void (*fn)())
:Builder()
{
    ret->cpu_context.rip = reinterpret_cast<void *>(fn);
}
ps::Task::Builder::Builder(int (*fn)(int argc,const char * argv[])
        ,int argc,const char * argv[])
:Builder()
{
    ret->cpu_context.rip = reinterpret_cast<void *>(fn);
    ret->cpu_context.rdi = argc;
    ret->cpu_context.rsi = reinterpret_cast<uint64_t>(argv);
}
ps::Task::Builder & ps::Task::Builder::name(util::string_view n)
{
    ret->name = n;
    name_set = true;
    return *this;
}

ps::Task::Builder & ps::Task::Builder::stack(size_t sz)
{
    ret->stack_size = sz;
    return *this;
}
ps::Task::Builder & ps::Task::Builder::stack(void * ptr,size_t sz)
{
    ret->stack_start = (uint64_t *)ptr;
    ret->stack_size = sz;
    return *this;
}
ps::Task * ps::Task::Builder::build()
{
    if(ret->stack_start == nullptr)
    {
       ret->stack_start = reinterpret_cast<uint64_t *>(new char[ret->stack_size]);//...
    }
    ret->cpu_context.rbp = ret->stack_start 
                + ret->stack_size - sizeof(uintptr_t)*2; //16 bytes align.
    ret->cpu_context.rsp = ret->cpu_context.rbp;
    //*ret->cpu_context.rbp = exit;
    /*
    if(!name_set) ret-> name = generate_name();...
    */
    return ret;
}
void ps::Task::switch_to_in_irq(Task *after)
{
    change_state(StateFlag::ready);
    after->change_state(StateFlag::running);

    auto cur = x86_64::getContextInException();
    text::memcpy(&cpu_context,cur,sizeof(x86_64::cpuContext));
    //To do : load fpu context
    /*
    if(fpu_used){
        fsave load;
    }
    */
    text::memcpy(cur,&after->cpu_context,sizeof(x86_64::cpuContext));
}
void ps::Task::switch_to(Task * other)
{
    interrupt_lock l;
    l.lock();
    change_state(StateFlag::ready);
    other->change_state(StateFlag::running);
    x86_64::switch_to(&cpu_context,&other->cpu_context);
    l.unlock();
}
void ps::Task::context_switch_to(Task * t)
{
    x86_64::switch_to(nullptr,&t->cpu_context);
}
void ps::Task::change_state(StateFlag sf)
{
    get_queue(state).pop_node(&state_node);
    state = sf;
    get_queue(state).push_back_node(&state_node);
}

ps::Task * ps::Task::createKernelTaskTemplate()
{
    auto ret = new ps::Task;//... task pool
    ret->id = pid_alloc();
    ret->state = StateFlag::created;
    get_queue(StateFlag::created).push_back_node(&ret->state_node);
    return ret;
}


ps::TaskList & ps::get_queue(StateFlag sf)
{
    static TaskList queue[(size_t)StateFlag::state_end];
    return queue[(size_t)sf];
}
