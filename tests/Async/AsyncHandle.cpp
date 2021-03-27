#include <atomic>
#include <memory>
#include <thread>
#include <iostream>
#include <mutex>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <chrono>
#include <cassert>
#include <functional>

struct MemoryResource
{
    void* data = nullptr;
    size_t size_in_bytes = 0;
};

MemoryResource load_file(const char* url)
{
    MemoryResource result = {};
    FILE* f = fopen(url, "rb");
    fseek(f, 0l, SEEK_END);
    result.size_in_bytes = ftell(f);
    result.data = malloc(result.size_in_bytes);
    rewind(f);
    fread(result.data, 1, result.size_in_bytes, f);
    return result;
}

struct AsyncFile
{
    friend struct AsyncFileLoader;

protected:
    void __initialize(MemoryResource resource)
    {
        memory_resource = resource;
        is_ready = true;
    }
    MemoryResource memory_resource;
    std::atomic_bool is_ready = false;
};  

struct AsyncFileLoader
{
    void loadTextureAsync(const char* path,
        const std::function<void(std::shared_ptr<AsyncFile>)>& completeCallback = [](std::shared_ptr<AsyncFile> tex) {})
    {
        FileLoaderTask task;
        task.path = path;
        task.file = NULL;
        task.complete_callback = completeCallback;
        load_queue_mutex.lock();
        load_queue.push_back(task);
        load_queue_mutex.unlock();
    }
    AsyncFileLoader()
        :is_running(true)
    {
        loader_thread = std::thread(&AsyncFileLoader::loaderThreadFunction, this);
    }
    ~AsyncFileLoader()
    {
        is_running = false;
        if(loader_thread.joinable())
            loader_thread.join();
    }
protected:
    void loaderThreadFunction()
    {
        while(is_running)
        {
            load_queue_mutex.lock();
            std::vector<FileLoaderTask> tasks = load_queue;
            load_queue.clear();
            load_queue_mutex.unlock();

            for (FileLoaderTask& task : tasks) {
                task.file = std::make_shared<AsyncFile>();
                task.file->__initialize(load_file(task.path));
                assert(task.file != NULL && "Load Error!");
                std::cout << task.file->memory_resource.size_in_bytes << std::endl;

                complete_queue_mutex.lock();
                complete_queue.push_back(task);
                complete_queue_mutex.unlock();
            }
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1ms);
            }
        }
    }
    struct FileLoaderTask 
    {
		const char* path;
		std::shared_ptr<AsyncFile> file;
		std::function<void(std::shared_ptr<AsyncFile>)> complete_callback;
	};
    std::atomic_bool is_running;
    std::thread loader_thread;
	std::vector<FileLoaderTask> load_queue;
	std::mutex load_queue_mutex;
	std::vector<FileLoaderTask> complete_queue;
	std::mutex complete_queue_mutex;
};

int main(void)
{
    auto pth = std::filesystem::current_path();
    auto filePth = pth/"cmake_install.cmake";
    AsyncFileLoader loader;
    std::shared_ptr<AsyncFile> file;
    auto cpath = filePth.string();
    loader.loadTextureAsync(cpath.c_str(),
        [&](std::shared_ptr<AsyncFile> loaded){
            file = loaded;
    });

    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1500ms);
    }

    return 0;
}