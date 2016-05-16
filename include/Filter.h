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

	inline void resize(size_t size)
	{
		re.resize(size);
		im.resize(size);
	}
    inline size_t size() const
	{
        return re.size();
    }
};

namespace FilterTool
{
	template<typename T>
	static Complex<T> mul(const Complex<double> & a, const Complex<double> & b);
	/** Discrete fourier transform */
	template<typename T>
	static Complex<double> getDFTsignal(std::vector<T> const& input);
	/** Discrete fourier transform inverse */
	template<typename T>
	static std::vector<T> getDFTinverse(const Complex<double> & input);
}

/** A basic low pass filter filter a array of vector<T> 
 * the output is given by getOutput()
*/
template<typename Tin>
class Filter
{
    public:

        Filter();
        Filter(std::vector<Tin>* raw,  double LPcoef);
        Filter(std::vector<Tin>* raw);
        virtual ~Filter();
        void setInput(std::vector<Tin>* raw);
		std::vector<Tin> getOutput();
        
    protected:
		void compute();
		void compute1();
        Complex<double> getDFTfilterLP1(double alpha);

        double cutoff;
    private:
        std::vector<Tin>* input;
        std::vector<Tin> output;

};

#endif // FILTER_H
