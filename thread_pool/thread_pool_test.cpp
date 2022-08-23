/***********************************************
* Author: David peer                           *
* Reviewer: Zohar                              *
* File: thread_pool_test.cpp                   *
* Date: 18.05.2022                             *
* Description: thread pool test file           *
***********************************************/

#include <iostream> // for std::cout
#include <exception> // for std::exception

#include "thread_pool.hpp" // for thread pool class and functions

using namespace ilrd_rd1145;

namespace ilrd_rd1145
{
class Tests
{
public:
    void UnitTest1();
    void UnitTest2();
    void UnitTest3();
    void UnitTest4();
    void UnitTest5();
    void UnitTest6();
    void UnitTest7();
    void UnitTest8();
    void UnitTest9();
    void UnitTest10();
};

void Task1()
{
    std::cout << "10\n";
    sleep(1);
}

void Task2()
{
    std::cout << "-10\n";
}

void TaskThrow()
{
    const char *msg = "hello\n";

    try
    {
        throw msg;
    }
    catch (const char& msg)
    {
        std::cout << msg;
    }
}
}// ilrd_rd1145

int main()
{
    Tests test;

    //test.UnitTest1();
    //test.UnitTest2();
    //test.UnitTest3();
    //test.UnitTest4();
    //test.UnitTest5();
    //test.UnitTest6();
    //test.UnitTest7();
    //test.UnitTest8();
    test.UnitTest9();
    //test.UnitTest10();

    return 0;
}

namespace ilrd_rd1145
{
void Tests::UnitTest1()
{
    ThreadPool pool;
    std::cout << "\n***************Test 1***************\n";
    for (int i = 0; i < 4; ++i)
    {
        pool.AddTask(Task1, 5);
    }

    for (int i = 0; i < 8; ++i)
    {
        pool.AddTask(Task2, 1);
    }

    for (int i = 0; i < 4; ++i)
    {
        pool.AddTask(Task1, 5);
    }

    pool.Stop();
    std::cout << "\n************End of Test 1************\n";
}

void Tests::UnitTest2()
{
    ThreadPool pool;
    std::cout << "\n***************Test 2***************\n";

    for (int i = 0; i < 10; ++i)
    {
        pool.AddTask(Task2, 17);
    }

    pool.Stop();

    for (int i = 0; i < 10; ++i)
    {
        pool.AddTask(Task2, 17);
    }

    std::cout << "\n************End of Test 2************\n";
}

void Tests::UnitTest3()
{
    ThreadPool pool;
    std::cout << "\n***************Test 3***************\n";

    for (int i = 0; i < 10; ++i)
    {
        pool.AddTask(Task2, 17);
    }

    pool.Pause();
    sleep(1);
    std::cout << "adding higher priority tasks to queue and resuming\n";

    for (int i = 0; i < 10; ++i)
    {
        pool.AddTask(Task1, 10);
    }

    pool.Resume();

    pool.Stop();

    std::cout << "\n************End of Test 3************\n";
}
void Tests::UnitTest4()
{
    ThreadPool pool(10);
    std::cout << "\n***************Test 4***************\n";

    pool.Pause();
    for (int i = 0; i < 5; ++i)
    {
        pool.AddTask(Task1, 10);
    }
    pool.Resume();

    pool.SetNumOfThreads(1);
    for (int i = 0; i < 10; ++i)
    {
        pool.AddTask(Task2, 17);
    }

    pool.Stop();
    std::cout << "\n************End of Test 4************\n";
}

void Tests::UnitTest5()
{
    ThreadPool pool;
    std::cout << "\n***************Test 5***************\n";
    std::cout << "\npausing after stop\n";
    for (int i = 0; i < 15; ++i)
    {
        pool.AddTask(Task2, 17);
    }
    pool.Stop();
    pool.Pause();

    std::cout << "\n************End of Test 5************\n";
}

void Tests::UnitTest6()
{
    ThreadPool pool;
    std::cout << "\n***************Test 6***************\n";
    std::cout << "\npausing and stopping without resume\n";

    pool.Pause();
    for (int i = 0; i < 15; ++i)
    {
        pool.AddTask(Task1, 17);
    }
    pool.Stop();

    std::cout << "\n************End of Test 6************\n";
}

void Tests::UnitTest7()
{
    ThreadPool pool;
    std::cout << "\n***************Test 7***************\n";
    std::cout << "\ncalling double pause and then resume\n";

    pool.Pause();
    pool.Pause();
    for (int i = 0; i < 15; ++i)
    {
        pool.AddTask(Task2, 8);
    }
    pool.Resume();

    pool.Stop();

    std::cout << "\n************End of Test 7************\n";

}

void Tests::UnitTest8()
{
    ThreadPool pool;
    std::cout << "\n***************Test 8***************\n";
    std::cout << "\ncalling double resume\n";

    pool.Pause();
    for (int i = 0; i < 15; ++i)
    {
        pool.AddTask(Task1, 8);
    }
    pool.Resume();
    pool.Resume();

    pool.Stop();

    std::cout << "\n************End of Test 8************\n";
}

void Tests::UnitTest9()
{
    ThreadPool pool;
    std::cout << "\n***************Test 9***************\n";
    std::cout << "\nset threads to 0\n";

    pool.Pause();
    for (int i = 0; i < 15; ++i)
    {
        pool.AddTask(Task1, 9);
    }
    pool.SetNumOfThreads(0);
    pool.Resume();

    sleep(3);
    pool.SetNumOfThreads(1);
    std::cout << "after sleeping increasing threads above 0\n";

    pool.Stop();

    std::cout << "\n************End of Test 9************\n";
}

void Tests::UnitTest10()
{
    ThreadPool pool(1);
    std::cout << "\n***************Test 10***************\n";

    pool.Stop();
    std::cout << "\n************End of Test 10************\n";
}

}//ilrd_rd1145

