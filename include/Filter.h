#ifndef FILTER_H
#define FILTER_H

// class used to filter a signal in frequency

#include <SFML/Audio.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#include "SoundManager.h"

template <typename T>
struct Complex
{
    std::vector<T> re;
    std::vector<T> im;
    inline void resize(int size){
        re.resize(size);
        im.resize(size);
    }
    inline unsigned int size(){
        return re.size();
    }
};

class Filter
{
    public:
        Filter();
        Filter(std::vector<sf::Int16>* raw,  double LPcoef);
        Filter(std::vector<sf::Int16>* raw);
        virtual ~Filter();
        void setSignal(std::vector<sf::Int16>* raw);
        std::vector<sf::Int16> getOutput();

        static Complex<double> getDFTsignal(std::vector<sf::Int16> const& input);
        void compute();
        void compute1();
    protected:
        Complex<double> getDFTfilterLP1(double alpha);
        std::vector<sf::Int16> getDFTinverse(Complex<double> & sfiltered);
        std::vector<double> getDFTinverseD(Complex<double> & sfiltered);
        Complex<sf::Int16> iconvolution(Complex<double> & asignal, Complex<double> & bfilter);
        Complex<double> dconvolution(Complex<double> & asignal, Complex<double> & bfilter);
        double cutoff;
    private:
        std::vector<sf::Int16>* input;
        //Complex<double> input;
        /*Complex<double> filter;
        Complex<sf::Int16> filtered;*/
        std::vector<sf::Int16> output;


};

#endif // FILTER_H
