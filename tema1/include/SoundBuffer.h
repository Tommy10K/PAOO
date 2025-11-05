#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H

#include <string>
#include <cstddef>
#include <cstdint>

class SoundBuffer
{
private:
    std::string m_assetName;
    int16_t* m_samples;
    size_t m_sampleCount;

public:
    SoundBuffer(const std::string& name, size_t sampleCount);
    ~SoundBuffer();

    SoundBuffer(const SoundBuffer& other);

    SoundBuffer(SoundBuffer&& other) noexcept;

    const std::string& getName() const { return m_assetName; }
    void play() const;
    void simulateProcessing(const std::string& effect);
};

#endif