/***********************************************
* Author: David peer                           *
* Reviewer: Zohar                              *
* File: thread_pool.cpp                        *
* Date: 18.05.2022                             *
* Description: thread pool source file         *
***********************************************/

#include <iostream>         // for std::cout

#include "thread_pool.hpp"  // for thread pool class and functions

namespace ilrd_rd1145
{

class ThreadPool::State: private boost::noncopyable
{
public:
    virtual bool ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                               boost::mutex *mutex,
                               boost::condition_variable *condVar) = 0;
};

class ThreadPool::RunState: public ThreadPool::State
{
public:
    static ThreadPool::State *GetInstance();
    bool ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                       boost::mutex *mutex,
                       boost::condition_variable *condVar);
};

class ThreadPool::PauseState: public ThreadPool::State
{
public:
    static ThreadPool::State *GetInstance();
    bool ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                       boost::mutex *mutex,
                       boost::condition_variable *condVar);
};

class ThreadPool::StopState: public ThreadPool::State
{
public:
    static ThreadPool::State *GetInstance();
    bool ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                       boost::mutex *mutex,
                       boost::condition_variable *condVar);
};

class ThreadPool::ForceStopState: public ThreadPool::State
{
public:
    static ThreadPool::State *GetInstance();
    bool ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                       boost::mutex *mutex,
                       boost::condition_variable *condVar);
};

void ThreadFunc(ThreadPool *pool);

ThreadPool::ThreadPool(std::size_t numOfThread) : m_numThreads(0), m_state(RUN)
{
    CreateThreads(numOfThread);
}

ThreadPool::~ThreadPool()
{
    Stop(boost::chrono::milliseconds(0));
}

int ThreadPool::AddTask(boost::function<void(void)> task, size_t niceness)
{
    int result = FAILURE;

    if (STOP != m_state && FORCE_STOP != m_state)
    {
        m_waitQueue.Push(Task(task, niceness));
        result = SUCCESS;
    }

    return result;
}

void ThreadPool::Stop()
{
    m_state = STOP;
    m_condVar.notify_all();

    while (m_numThreads)
    {
        sleep(0);
    }
}

void ThreadPool::Stop(boost::chrono::milliseconds timeout)
{
    m_state = STOP;
    m_condVar.notify_all();

    boost::this_thread::sleep_for(timeout);

    m_state = FORCE_STOP;
}

void ThreadPool::Resume()
{
    m_state = RUN;
    m_condVar.notify_all();
}

void ThreadPool::Pause()
{
    m_state = PAUSE;
}

void ThreadPool::SetNumOfThreads(std::size_t numOfThreads)
{
    if (numOfThreads > m_numThreads)
    {
        size_t delta = numOfThreads - m_numThreads;
        CreateThreads(delta);
    }
    else
    {
        size_t delta = m_numThreads - numOfThreads;
        DestroyThreads(delta);
    }

    m_numThreads = numOfThreads;
}

void ThreadPool::CreateThreads(size_t numOfThreads)
{
    for (size_t i = 0; i < numOfThreads; ++i)
    {
        boost::thread thread(ThreadFunc, this);
        ++m_numThreads;
        thread.detach();
    }
}

void DestroyTask()
{
    throw boost::thread_interrupted();
}

void ThreadPool::DestroyThreads(size_t numOfThreads)
{
    boost::function<void(void)> destTask = DestroyTask;

    for (size_t i = 0; i < numOfThreads; ++i)
    {
        --m_numThreads;
        AddTask(destTask, 0);
    }
}

void ThreadPool::ThreadFunc(ThreadPool *pool)
{
    bool alive = true;
    ThreadPool::State *obj = NULL;

    while (alive)
    {
        obj = ThreadPool::GetState(&pool->m_state);
        alive = obj->ActivateState(&pool->m_waitQueue, &pool->m_mut, &pool->m_condVar);
    }

    --pool->m_numThreads;
}

//States
ThreadPool::State *ThreadPool::GetState(boost::atomic<states> *state)
{
    static ThreadPool::State *(* objects[SIZE])() =
            {
                ThreadPool::RunState::GetInstance,
                ThreadPool::PauseState::GetInstance,
                ThreadPool::StopState::GetInstance,
                ThreadPool::ForceStopState::GetInstance
            };

    return objects[*state]();
}

ThreadPool::State *ThreadPool::RunState::GetInstance()
{
    static RunState obj;
    return &obj;
}

bool ThreadPool::RunState::ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                                         boost::mutex *mutex,
                                         boost::condition_variable *condVar)
{
    Task to_run(NULL, 0);

    (void)mutex;
    (void)condVar;

    if (true == wq->Pop(to_run, boost::chrono::milliseconds(500)))
    {
        try
        {
            to_run();
        }
        catch (...)
        {
            std::cout << "exception thrown from task\n";
        }
    }

    return true;
}

ThreadPool::State *ThreadPool::PauseState::GetInstance()
{
    static PauseState obj;
    return &obj;
}

bool ThreadPool::PauseState::ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                                           boost::mutex *mutex,
                                           boost::condition_variable *condVar)
{
    (void)wq;

    boost::unique_lock<boost::mutex> lock(*mutex);
    condVar->wait(lock);

    return true;
}

ThreadPool::State *ThreadPool::StopState::GetInstance()
{
    static StopState obj;
    return &obj;
}

bool ThreadPool::StopState::ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                                          boost::mutex *mutex,
                                          boost::condition_variable *condVar)
{
    ThreadPool::State *obj = ThreadPool::RunState::GetInstance();

    if (!wq->IsEmpty())
    {
        obj->ActivateState(wq, mutex, condVar);

        return true;
    }

    return false;
}

ThreadPool::State *ThreadPool::ForceStopState::GetInstance()
{
    static ForceStopState obj;
    return &obj;
}

bool ThreadPool::ForceStopState::ActivateState(WaitableQueue<std::priority_queue<Task> > *wq,
                                           boost::mutex *mutex,
                                           boost::condition_variable *condVar)
{
    (void)wq;
    (void)mutex;
    (void)condVar;

    return false;
}
} // ilrd_rd1145
