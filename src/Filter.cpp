#include "../include/Filter.h"
#include "../src/utils.cpp"

template<typename T>
Complex<T> FilterTool::mul(const Complex<double> & asignal, const Complex<double> & bfilter)
{
	Complex<T> result;
	result.resize(asignal.size());
	for (unsigned int f = 0; f < result.size(); ++f) {
		result.re[f] = (T)(asignal.re[f] * bfilter.re[f] - 1.*asignal.im[f] * bfilter.im[f]);
		result.im[f] = (T)(asignal.re[f] * bfilter.im[f] + 1.*asignal.im[f] * bfilter.re[f]);
	}
	return result;
}

template<typename T>
Complex<double> FilterTool::getDFTsignal(std::vector<T> const& input)
{
	Complex<double> nsignal;
	nsignal.resize(input.size() / 2);
	for (unsigned int f = 0; f < nsignal.size(); ++f) {
		for (unsigned int i = 0; i < input.size(); ++i) {
			double w = TWO_PI*static_cast<double>(f*i) / input.size();
			nsignal.re[f] += input[i] * cos(w);
			nsignal.im[f] -= input[i] * sin(w);
		}
		nsignal.re[f] /= nsignal.size();
		nsignal.im[f] /= nsignal.size();
	}
	return nsignal;
}

template<typename T>
std::vector<T> FilterTool::getDFTinverse(const Complex<double> & sfiltered)
{
	std::vector<T> result;
	result.resize(sfiltered.size() * 2, 0);
	for (unsigned int f = 0; f < sfiltered.size(); ++f) {
		for (unsigned int i = 0; i < result.size(); ++i) {
			double w = TWO_PI*(double)(f*i)*1. / result.size();
			result[i] += (T)(sfiltered.re[f] * cos(w) - sfiltered.im[f] * sin(w));
		}
	}
	return result;
}

// ----------------------------------------------------------------------------

template<class Tin>
Filter<Tin>::Filter(std::vector<Tin>* raw, double LPcoef) :
	cutoff(LPcoef), input(raw)
{
}

template<class Tin>
Filter<Tin>::Filter(std::vector<Tin>* raw) :
	input(raw)
{
}

template<class Tin>
Filter<Tin>::Filter() :
	cutoff(150.f)
{
}

template<class Tin>
Filter<Tin>::~Filter()
{
}

template<class Tin>
void Filter<Tin>::compute()
{
	const Complex<double> filter(getDFTfilterLP1(cutoff));
	const Complex<double> inputC(FilterTool::getDFTsignal(*input));
	const Complex<double> filtered = FilterTool::mul<double>(inputC, filter);
	output = FilterTool::getDFTinverse<Tin>(filtered);
}

template<class Tin>
void Filter<Tin>::compute1()
{
	double fc = cutoff;
	int Fe = SoundManager::SAMPLE_RATE;
	Complex<double> filter;
	filter.resize(input->size() / 2);
	for (unsigned int f = 0; f < filter.size(); ++f) {
		double w = TWO_PI*f;
		double wc = (w*Fe) / (fc*input->size());
		double gain_squared = 1.0 / (1. + pow(wc, 8));
		filter.re[f] = gain_squared;
		filter.im[f] = 0;
	}

	std::vector<double> filt = FilterTool::getDFTinverse<double>(filter);
	output.resize(input->size(), 0);
	for (unsigned int i = 0; i < output.size(); ++i) {
		for (unsigned int f = 0; f < output.size(); ++f) {
			output[i] += (Tin)(1.*(*input)[(input->size() + i - f) % input->size()] * filt[f]);
		}
	}
}

template<class Tin>
std::vector<Tin> Filter<Tin>::getOutput()
{
	if (output.size() <= 0) {
		compute();
	}
	return output;
}

template<class Tin>
void Filter<Tin>::setInput(std::vector<Tin>* raw)
{
	input = raw;
}

/** compute the kernel of a lowpass filter */
template<class Tin>
Complex<double> Filter<Tin>::getDFTfilterLP1(double alpha)
{
	Complex<double> filter;
	filter.resize(input->size());
	double w;
	for (unsigned int f = 0; f < filter.size(); ++f) {
		w = TWO_PI*f;
		filter.re[f] = alpha*alpha / (alpha*alpha + w*w);
		filter.im[f] = -w*alpha / (alpha*alpha + w*w);
	}
	return filter;
}

template class Filter<sf::Int16>;
