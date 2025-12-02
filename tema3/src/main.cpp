#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <set>
#include <memory>

struct GraphicsCard {
    std::mutex gpuMutex;
    std::set<int> loadedTextureIDs;

    void deleteTexture(int id) {
        std::lock_guard<std::mutex> lock(gpuMutex);
        if (loadedTextureIDs.find(id) != loadedTextureIDs.end()) {
            loadedTextureIDs.erase(id);
            std::cout << "[GPU] Texture " << id << " deleted from VRAM.\n";
        } else {
            std::cout << "[GPU] ERROR: Double-free or Invalid delete on Texture " << id << "!\n";
        }
    }

    void drawTexture(int id) {
        std::lock_guard<std::mutex> lock(gpuMutex);
        if (loadedTextureIDs.find(id) != loadedTextureIDs.end()) {
            std::cout << "[GPU] Drawing with Texture " << id << " (Success).\n";
        } else {
            std::cout << "[GPU] CRITICAL ERROR: Segfault! Texture " << id << " is not in VRAM!\n";
        }
    }

    void allocateTexture(int id) {
        std::lock_guard<std::mutex> lock(gpuMutex);
        loadedTextureIDs.insert(id);
        std::cout << "[GPU] Texture " << id << " uploaded to VRAM.\n";
    }
};

GraphicsCard g_GPU;

class ManualAssetLoader {
public:
    void uploadBatch(int batchID) {
        std::cout << "  (Bad-Item13) Waiting for GPU Lock...\n";
        g_GPU.gpuMutex.lock();
        std::cout << "  (Bad-Item13) GPU Locked. Uploading batch " << batchID << "...\n";

        if (batchID < 0) {
            std::cout << "  (Bad-Item13) EXCEPTION: Corrupt asset data!\n";
            throw std::runtime_error("Asset Load Failed");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        g_GPU.gpuMutex.unlock();
        std::cout << "  (Bad-Item13) GPU Unlocked.\n";
    }
};

class ScopedGpuLock {
    std::mutex& m_mutex;
public:
    explicit ScopedGpuLock(std::mutex& m) : m_mutex(m) {
        m_mutex.lock();
        std::cout << "  (Good-Item13) RAII GPU Lock acquired.\n";
    }

    ~ScopedGpuLock() {
        m_mutex.unlock();
        std::cout << "  (Good-Item13) RAII GPU Lock released.\n";
    }
};

class UnsafeTextureHandle {
public:
    int texID;

    UnsafeTextureHandle(int id) : texID(id) {
        g_GPU.allocateTexture(texID);
    }

    ~UnsafeTextureHandle() {
        std::cout << "  (Bad-Item14) Destructor calling delete for " << texID << "...\n";
        g_GPU.deleteTexture(texID);
    }
};

class SmartTextureHandle {
    std::shared_ptr<int> texturePtr; 
public:
    SmartTextureHandle(int id) {
        g_GPU.allocateTexture(id);

        texturePtr = std::shared_ptr<int>(new int(id), [](int* p) {
            std::cout << "  (Good-Item14) Ref Count is 0. Deleting Texture " << *p << " from VRAM.\n";
            g_GPU.deleteTexture(*p);
            delete p;
        });
    }

    int getID() const { return *texturePtr; }
};

void runBadLoader() {
    ManualAssetLoader loader;
    try {
        loader.uploadBatch(-1); 
    } catch (...) {
        std::cout << "  (Main) Caught exception from BadLoader.\n";
    }
}

void testItem13() {
    std::cout << "\n=== TEST ITEM 13: GPU Locking (Deadlock Scenario) ===\n";

    std::thread t1(runBadLoader);
    t1.join();

    std::cout << "  (Main) Attempting to acquire GPU lock for next frame...\n";
    if (g_GPU.gpuMutex.try_lock()) {
        std::cout << "  (Main) SUCCESS: GPU was unlocked.\n";
        g_GPU.gpuMutex.unlock();
    } else {
        std::cout << "  (Main) FAILURE: GPU is deadlocked! (Item 13 Violation)\n";
        g_GPU.gpuMutex.unlock();
    }

    std::cout << "\n--- Retesting with RAII ---\n";
    try {
        ScopedGpuLock lock(g_GPU.gpuMutex);
        std::cout << "  (Main) Throwing exception inside RAII scope...\n";
        throw std::runtime_error("OOM");
    } catch (...) {
        std::cout << "  (Main) Caught exception.\n";
    }
    
    if (g_GPU.gpuMutex.try_lock()) {
        std::cout << "  (Main) SUCCESS: RAII correctly released the lock.\n";
        g_GPU.gpuMutex.unlock();
    }
}

template <typename T>
void backgroundRender(T tex) {
    std::cout << "    (Thread) Rendering with texture...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "    (Thread) Render complete. Helper object dying.\n";
}

void testItem14() {
    std::cout << "\n=== TEST ITEM 14: Texture Handles (Copy Behavior) ===\n";
    
    std::cout << "\n[Bad Implementation Scenario]\n";
    {
        UnsafeTextureHandle badTex(100);
        std::thread t1(backgroundRender<UnsafeTextureHandle>, badTex);
        t1.join(); 
        
        std::cout << "  (Main) Trying to draw original texture 100...\n";
        g_GPU.drawTexture(100);
    }

    std::cout << "\n[Good Implementation Scenario]\n";
    {
        SmartTextureHandle goodTex(200);
        std::thread t2(backgroundRender<SmartTextureHandle>, goodTex);
        t2.join();

        std::cout << "  (Main) Trying to draw original texture 200...\n";
        g_GPU.drawTexture(200);
    }
}

int main() {
    testItem13();
    testItem14();
    return 0;
}