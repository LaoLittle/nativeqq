#include <iostream>
#include "tup/tup.h"
#include "oicq/oicq.h"
#include "uvw.hpp"
#include "crypt/hmac_sha256.h"

#include <uv.h>

#include "cppcoro/task.hpp"
#include "cppcoro/sync_wait.hpp"
#include "cppcoro/async_latch.hpp"
#include <cppcoro/static_thread_pool.hpp>

using namespace tars;

cppcoro::task<int> count_lines()
{
    int lineCount = 0;

    std::cout << "line: ";
    for (int i = 0; i < 3; ++i) {
        lineCount++;
        std::cout << "thread_id: " << std::this_thread::get_id() << "\n";
        std::cout << lineCount << " \n"[i == 3 - 1];
        uv_sleep(1 * 1000);
        std::cout << "thread_id: " << std::this_thread::get_id() << "\n";
    }

    co_return lineCount;
}

void thread_entry() {
    cppcoro::static_thread_pool threadPool;
    auto initiatingThreadId = std::this_thread::get_id();

    cppcoro::sync([&]() -> cppcoro::task<void> {
        co_await threadPool.schedule();
        std::cout << "thread_id (after schedule): " << std::this_thread::get_id() << "\n";

        if (std::this_thread::get_id() == initiatingThreadId) {
            std::cout << ("schedule() did not switch threads");
        }
    }());
    /*
    cppcoro::sync_wait([&]() -> cppcoro::task<> {
        std::cout << "thread_id: " << std::this_thread::get_id() << "\n";
        cppcoro::task<int> countTask = count_lines();
        // co_await t;
        //int line = co_await countTask;
        //std::cout << "out_line: " << line << "\n";
        std::cout << "thread_id: " << std::this_thread::get_id() << "\n";

        for (int i = 0; i < 10; ++i) {
            uv_sleep(1);
        }
    }());*/
}

int main() {
    try {
        std::cout << "thread_id: " << std::this_thread::get_id() << "\n";
        thread_entry();
        std::cout << "thread_id (after thread_entry): " << std::this_thread::get_id() << "\n";

        oicq::Oicq oicq;
        oicq.getStByPwd(1372362033, "123456");
    } catch (exception& ex) {
        printf("err: %s\n", ex.what());
    }
    return 0;
}