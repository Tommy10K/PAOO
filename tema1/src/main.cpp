#include <iostream>
#include <vector>

#include "SoundBuffer.h"

SoundBuffer loadGunshotSound()
{
    std::cout << "-> (Funcție) Se încarcă 'shotgun_blast.wav' de pe disc...\n";
    SoundBuffer tempSound("shotgun_blast.wav", 500000);
    std::cout << "-> (Funcție) Încărcare terminată. Se returnează 'tempSound'...\n";
    return std::move(tempSound);
}

SoundBuffer applyCaveEcho(const SoundBuffer& originalFootsteps)
{
    std::cout << "-> (Funcție) Se aplică ecou pe " << originalFootsteps.getName() << "\n";
    SoundBuffer echoCopy = originalFootsteps;
    echoCopy.simulateProcessing("Ecou de Peșteră");
    std::cout << "-> (Funcție) Returnare copie cu ecou...\n";
    return echoCopy;
}

int main()
{
    std::cout << "=== DEMO 1: MOVE CONSTRUCTOR (ÎMPUȘCĂTURĂ) ===\n";
    SoundBuffer myGunSound = loadGunshotSound();
    
    std::cout << "-> (Main) Am primit sunetul. Îl redăm:\n";
    myGunSound.play();
    
    std::cout << "\n-------------------------------------------------\n";
    
    std::cout << "\n=== DEMO 2: COPY CONSTRUCTOR (PAȘI) ===\n";
    
    SoundBuffer originalSteps("wood_steps.wav", 5000);
    
    SoundBuffer echoSteps = applyCaveEcho(originalSteps);
    
    std::cout << "\n-> (Main) Procesare terminată. Avem 2 sunete separate:\n";
    
    std::cout << "Originalul:";
    originalSteps.play();
    
    std::cout << "Copia:";
    echoSteps.play();

    std::cout << "\n=== DEMO 3: MOVE CONSTRUCTOR în std::vector ===\n";
    
    std::vector<SoundBuffer> soundLibrary;
    
    std::cout << "\n-> 1. Adăugare 'push_back' cu un temporar (R-Value)...\n";
    soundLibrary.push_back(SoundBuffer("pickup_item.wav", 20000));
    
    std::cout << "\n-> 2. Adăugare 'push_back' cu rezultatul unei funcții...\n";
    soundLibrary.push_back(loadGunshotSound());

    std::cout << "\n-> 3. Adăugare 'push_back' cu std::move (L-Value forțat)...\n";
    SoundBuffer jumpSound("jump.wav", 15000);
    soundLibrary.push_back(std::move(jumpSound));

    std::cout << "\n-> Verificare: 'jumpSound' este acum gol:\n";
    jumpSound.play();

    std::cout << "\n-------------------------------------------------\n";
    std::cout << "Programul se termină. Se apelează destructorii:\n";
    
    return 0;
}