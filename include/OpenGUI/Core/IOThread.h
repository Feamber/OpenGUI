#pragma once
#include "OpenGUI/Core/AsyncFile.h"
#include <atomic>
#include <mutex>
#include "OpenGUI/Core/Containers/vector.hpp"
#include <thread>
#include <chrono>
#include <memory>
#include <functional>

namespace OGUI
{
class OGUI_API IOThread
{
public:
    template<typename T = AsyncFile>
    std::shared_ptr<T> Load(const char* path,
        const std::function<void(std::shared_ptr<AsyncFile>)>& completeCallback = [](std::shared_ptr<AsyncFile> tex) {})
    {
        FileLoaderTask task;
        task.path = path;
        task.file = std::shared_ptr<T>(new T(), [](T* file){
            file->Finalize();
        });
        task.complete_callback = completeCallback;
        load_queue_mutex.lock();
        load_queue.push_back(task);
        load_queue_mutex.unlock();
        return std::static_pointer_cast<T>(task.file);
    }
    IOThread();
    ~IOThread();
protected:
    void loaderThreadFunction();

    struct FileLoaderTask 
    {
		std::string path;
		std::shared_ptr<AsyncFile> file;
		std::function<void(std::shared_ptr<AsyncFile>)> complete_callback;
	};
    std::atomic_bool is_running;
    std::thread loader_thread;
	std::vector<FileLoaderTask> load_queue;
	std::mutex load_queue_mutex;
};
}