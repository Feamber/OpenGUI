#include <atomic>
#include <memory>
#include <thread>
#include <iostream>
#include <mutex>
#include <stdlib.h>
#include <vector>
#include <filesystem>
#include <chrono>

struct MemoryResource
{
    void* data = nullptr;
    size_t size_in_byte = 0;
};

MemoryResource load_file(const char* url)
{
    MemoryResource result = {};
    FILE* f = fopen(url, "rb");
    fseek(f, 0l, SEEK_END);
    result.size_in_byte = ftell(f);
    result.data = malloc(result.size_in_byte);
    rewind(f);
    fread(result.data, 1, result.size_in_byte, f);
    fclose(f);
    return result;
}

struct AsyncFile
{
    friend struct IOThread;
    virtual ~AsyncFile() {}
    virtual void finalize() = 0;
    virtual void initialize(const char* path) = 0;
    virtual size_t size() const = 0;
    virtual bool valid() const
    {
        return is_ready;
    }
protected:
    inline void __initialize(const char* path)
    {
        this->initialize(path);
        is_ready = true;
    }
    std::atomic_bool is_ready = false;
};  

struct IOThread
{
    template<typename T = AsyncFile>
    std::shared_ptr<T> Load(const char* path,
        const std::function<void(std::shared_ptr<AsyncFile>)>& completeCallback = [](std::shared_ptr<AsyncFile> tex) {})
    {
        FileLoaderTask task;
        task.path = path;
        task.file = std::shared_ptr<T>(new T(), [](T* file){
            file->finalize();
        });
        task.complete_callback = completeCallback;
        load_queue_mutex.lock();
        load_queue.push_back(task);
        load_queue_mutex.unlock();
        return std::static_pointer_cast<T>(task.file);
    }
    IOThread()
        :is_running(true)
    {
        loader_thread = std::thread(&IOThread::loaderThreadFunction, this);
    }
    ~IOThread()
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
                task.file->__initialize(task.path);
                assert(task.file != NULL && "Load Error!");
                task.complete_callback(task.file);
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
};

struct AsyncTexture final : public AsyncFile
{
    void initialize(const char* path) final
    {
        memory_resource = load_file(path);
    }
    void finalize() final
    {
        free(memory_resource.data);
        is_ready = false;
        std::cout << "free texture" << std::endl;
    }
    size_t size() const final {return memory_resource.size_in_byte;}
    const void* data() const {return memory_resource.data;}
    void* data() {return memory_resource.data;}

    MemoryResource memory_resource;
};  

#include <unordered_map>
struct FileManager 
{
    template<typename T>
    std::shared_ptr<T> Require(const std::string& url)
    {
        if(files.find(url) == files.end())
        {
            files[url] = loader.Load<AsyncTexture>(url.c_str());
        } 
        return std::static_pointer_cast<T>(files[url].lock());
    }
protected:
    IOThread loader;
    std::unordered_map<std::string, std::weak_ptr<AsyncFile>> files;
};

int main(void)
{
    FileManager manager;
    auto pth = std::filesystem::current_path();
    auto filePth = (pth/"cmake_install.cmake").string();
    
    /*
    IOThread loader;
    std::shared_ptr<AsyncTexture> file = 
        loader.Load<AsyncTexture>(filePth.c_str(), 
            [&](std::shared_ptr<AsyncFile> loaded){
                
        });
    while(!file->valid())
    {

    }
    std::cout << file->size() << "\n";
    */
    
    auto holder = manager.Require<AsyncTexture>(filePth.c_str());
    while(!holder->valid())
    {

    }
    std::cout << holder->size() << std::endl;
    

    return 0;
}