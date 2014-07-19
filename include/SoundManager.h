#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Audio.hpp>
#include <cmath>
#include <deque>
#include <vector>
#include <map>
#include <iostream>

#include "Filter.h"

class SoundManager
{
    public:

        SoundManager();
        void setRaw(std::vector<sf::Int16> & raw, int AMPLITUDE, double freq);
        void setRaw(std::vector<sf::Int16> & raw, int AMPLITUDE, double freq, double (*function) (double));
        void setADSR(std::vector<sf::Int16> & raw);
        std::vector<sf::Int16> repeat(std::vector<sf::Int16> & raw, double freq);
        virtual ~SoundManager();
        void update();
        void addNote(double frequence);
        void addNote(double frequence, double (*function) (double));
        int getSumRaw(int pos);
        int getLastRaw(int pos);
        int getRaw(unsigned int note, unsigned int pos);
        static const unsigned AMPLITUDE = 20000;
        static const unsigned SAMPLE_RATE = 44100;
    protected:

        static const unsigned SAMPLES = 44100;

        std::deque<std::vector<sf::Int16> > raw;
        std::map<double,std::vector<sf::Int16> > precomputedRaw;
        std::deque<sf::Sound> sound;
        std::deque<sf::SoundBuffer> buffer;
    private:
};

#endif // SOUNDMANAGER_H
