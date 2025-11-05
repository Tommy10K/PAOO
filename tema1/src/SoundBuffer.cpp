#include "SoundBuffer.h"
#include <iostream>
#include <algorithm>
#include <utility>

SoundBuffer::SoundBuffer(const std::string& name, size_t sampleCount)
    : m_assetName(name),
      m_samples(nullptr),
      m_sampleCount(sampleCount)
{
    if (m_sampleCount > 0) {
        m_samples = new int16_t[m_sampleCount];
        std::cout<<"   [CTOR] Alocat "<<m_sampleCount<<" eșantioane pentru "<<m_assetName<<"\n";
    }
}

SoundBuffer::~SoundBuffer()
{
    if (m_samples) {
        std::cout << "   [DTOR] Eliberat buffer-ul pentru " << m_assetName << "\n";
        delete[] m_samples;
    } else {
        std::cout << "   [DTOR] Obiect golit (post-move)\n";
    }
}

SoundBuffer::SoundBuffer(const SoundBuffer& other)
    : m_assetName(other.m_assetName + "_Copie"),
      m_samples(nullptr),
      m_sampleCount(other.m_sampleCount)
{
    if (m_sampleCount > 0) {
        m_samples = new int16_t[m_sampleCount];
        
        std::copy(other.m_samples, other.m_samples + m_sampleCount, m_samples);
    }
    std::cout<<"   [COPY CTOR] Creată copie în profunzime (deep copy) a "<<other.m_assetName<<"\n";
}

SoundBuffer::SoundBuffer(SoundBuffer&& other) noexcept
    : m_assetName(std::move(other.m_assetName)),
      m_samples(other.m_samples),
      m_sampleCount(other.m_sampleCount)
{
    other.m_samples = nullptr;
    other.m_sampleCount = 0;
    
    std::cout<<"   [MOVE CTOR] Resurse mutate de la "<<m_assetName<<"\n";
}

void SoundBuffer::play() const
{
    if (m_samples) {
        std::cout << "PLAY: Se redă " << m_assetName << " (" 
                  << m_sampleCount << " eșantioane)\n";
    } else {
        std::cout << "PLAY: Eroare, buffer-ul pentru " << m_assetName 
                  << " este gol (probabil a fost mutat).\n";
    }
}

void SoundBuffer::simulateProcessing(const std::string& effect)
{
    m_assetName += " (" + effect + ")";
    std::cout << "   ...Procesare audio: s-a aplicat efectul '" 
              << effect << "'...\n";
}