#include "../include/SoundManager.h"
#include "../src/utils.cpp"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
    for(unsigned int i=0;i<sound.size();i++){
        sound[i].stop();
    }
}

void SoundManager::setRaw(std::vector<sf::Int16> & raw, int AMPLITUDE, double freq)
{
    if(precomputedRaw.count(freq) > 0){
        raw = precomputedRaw[freq];
        return;
    }

    std::vector<sf::Int16> base((sf::Int16)(2*SAMPLE_RATE/freq), 0);
	const double increment = freq/SAMPLE_RATE;
	double x = 0;

	for (unsigned i = 0; i < base.size(); i++) {
	    base[i] = 0;
	    // different wave shape:
        //base[i] = AMPLITUDE * sin(x*TWO_PI);
		//base[i] += AMPLITUDE * (mod(static_cast<int>(x),2)-0.5);
        base[i] += (sf::Int16)(AMPLITUDE/2 *sin(x*TWO_PI/2)*sin(x*TWO_PI));
		//base[i] += rand()%(AMPLITUDE/2)-AMPLITUDE/4;
        /*if(raw[i] > AMPLITUDE*0.25*sin(x*TWO_PI) )
            raw[i] = AMPLITUDE*0.2;*/
		x += increment;
	}

	Filter<sf::Int16> f(&base, 100.0);base = f.getOutput();
    raw = repeat(base,freq);
	setADSR(raw);

	precomputedRaw[freq] = raw;
}

// you can pass a function defining the wave shape with "function"
void SoundManager::setRaw(std::vector<sf::Int16> & raw, int AMPLITUDE, double freq, double (*function) (double))
{
    if(precomputedRaw.count(freq) > 0){
        raw = precomputedRaw[freq];
        return;
    }

    std::vector<sf::Int16> base((sf::Int16)(2*SAMPLE_RATE/freq),0);
	const double increment = freq/SAMPLE_RATE;
	double x = 0;
	for (unsigned i = 0; i < base.size(); i++) {
		base[i] = (sf::Int16)(AMPLITUDE * function(x));
		x += increment;
	}

	const double filterCutoff = 50.0;

	Filter<sf::Int16> f(&base, filterCutoff);
	base = f.getOutput();
    raw = repeat(base,freq);
	setADSR(raw);

	precomputedRaw[freq] = raw;
}

std::vector<sf::Int16> SoundManager::repeat(std::vector<sf::Int16> & raw, double freq){
    std::vector<sf::Int16> result(SAMPLES);
    unsigned int nb = (unsigned int)(result.size() / raw.size());
    for (unsigned i = 0; i < raw.size(); i++) {
        for (unsigned int j = 0; j < nb; j++) {
            result[i+j*raw.size()] = raw[i];
        }
	}
	/*for (unsigned i = 0; i < result.size(); i++) {
        result[i] = raw[i%raw.size()];
	}*/
	return result;
}

void SoundManager::setADSR(std::vector<sf::Int16> & samp){
    // todo
	const int delay = 400;
	double ampModulation = 1.;
	for (unsigned i = 0; i < samp.size(); i++) {
	    if(i > SAMPLE_RATE*delay/1000.){ // D
	        ampModulation /= 1.0002;
	        if(ampModulation < 0.001)
                ampModulation = 0.;
	    }
		samp[i] *= (sf::Int16) ampModulation;
	}
}

void SoundManager::addNote(double freq)
{
    addNote(freq, s_square);
}

void SoundManager::addNote(double freq, double (*function) (double))
{
    raw.push_back(std::vector<sf::Int16>(SAMPLES) );
    buffer.push_back(sf::SoundBuffer());
    sound.push_back(sf::Sound());

    setRaw(raw.back(), AMPLITUDE, freq, function); // put the function here
	if (!buffer.back().loadFromSamples(&raw.back()[0], SAMPLES, 1, SAMPLE_RATE)) {
		std::cout << "Loading failed!" << std::endl;
		return;
	}
	sound.back().setBuffer(buffer.back());
	sound.back().play();
}

void SoundManager::update()
{
    for(unsigned int i=0;i<buffer.size();i++){
        if(sound[i].getStatus() == 0){
            raw.erase(raw.begin()+i);
            sound.erase(sound.begin()+i);
            buffer.erase(buffer.begin()+i);
        }
    }
}

// gettes / setters ------------------------------------------------------------

int SoundManager::getSumRaw(int pos)
{
    double t = 0.0;
    for(unsigned int i=0;i<buffer.size();i++){
        t += static_cast<double>(getRaw(i,pos));
    }
    t /= buffer.size();
    return static_cast<int>(t);
}

int SoundManager::getRaw(unsigned int note, unsigned int i)
{
    if(sound[note].getStatus() != 2)
        return 0;
    sf::Uint64 pos = buffer[note].getSampleCount()
                *sound[note].getPlayingOffset().asMilliseconds()/1000+i*1;
    if(pos < 0 || pos >= buffer[note].getSampleCount() )
        return 0;
    return raw.back()[pos];
}

int SoundManager::getLastRaw(int i)
{
    if(sound.size() == 0)
        return 0;
    if(sound.back().getStatus() != 2)
        return 0;
    sf::Uint64 pos = buffer.back().getSampleCount()
                *sound.back().getPlayingOffset().asMilliseconds()/1000+i*1;
    if(pos < 0 || pos >= buffer.back().getSampleCount() )
        return 0;
    return raw.back()[pos];
}

