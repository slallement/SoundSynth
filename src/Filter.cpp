#include "../include/Filter.h"
#include "../src/utils.cpp"

Filter::Filter(std::vector<sf::Int16>* raw, double LPcoef) :
    cutoff(LPcoef)
{
    input = raw;
}

Filter::Filter(std::vector<sf::Int16>* raw)
{
    setSignal(raw);

}

Filter::Filter():
    cutoff(150.f)
{
}

Filter::~Filter()
{
}

Complex<double> Filter::getDFTsignal(std::vector<sf::Int16> const& input)
{
    Complex<double> nsignal;
    nsignal.resize(input.size()/2);
    for(unsigned int f=0; f<nsignal.size(); f++){
        for(unsigned int i=0;i<input.size();i++) {
            double w = TWO_PI*static_cast<double>(i)/input.size();
            nsignal.re[f]+=input[i]*cos(f*w);
            nsignal.im[f]-=input[i]*sin(f*w);
        }
        nsignal.re[f]/=nsignal.size();
        nsignal.im[f]/=nsignal.size();
    }
    return nsignal;
}

Complex<double> Filter::dconvolution(Complex<double> & asignal, Complex<double> & bfilter)
{
    Complex<double> result;
    result.resize(asignal.size());
    for(unsigned int f=0;f<result.size();f++) {
        result.re[f]=asignal.re[f]*bfilter.re[f]-1.*asignal.im[f]*bfilter.im[f];
        result.im[f]=asignal.re[f]*bfilter.im[f]+1.*asignal.im[f]*bfilter.re[f];
    }
    return result;
}

Complex<sf::Int16> Filter::iconvolution(Complex<double> & asignal, Complex<double> & bfilter)
{
    Complex<sf::Int16> result;
    result.resize(asignal.size());
    for(unsigned int f=0;f<result.size();f++) {
        result.re[f]=static_cast<sf::Int16>(asignal.re[f]*bfilter.re[f]-1.*asignal.im[f]*bfilter.im[f]);
        result.im[f]=static_cast<sf::Int16>(asignal.re[f]*bfilter.im[f]+1.*asignal.im[f]*bfilter.re[f]);
    }
    return result;
}

std::vector<sf::Int16> Filter::getDFTinverse(Complex<double> & sfiltered)
{
    std::vector<sf::Int16> result;
    result.resize(input->size()*2,0);
    double w;
    for(unsigned int f=0;f<sfiltered.size();f++) {
        for(unsigned int i=0;i<result.size();i++) {
            w = TWO_PI*static_cast<double>(i)*1./result.size();
            result[i]+=static_cast<sf::Int16>(sfiltered.re[f]*cos(f*w)-sfiltered.im[f]*sin(f*w));
        }
    }
    return result;
}

std::vector<double> Filter::getDFTinverseD(Complex<double> & sfiltered)
{
    std::vector<double> result;
    result.resize(sfiltered.size()*2,0.0);
    double w;
    for(unsigned int f=0;f<sfiltered.size();f++) {
        for(unsigned int i=0;i<result.size();i++) {
            w = TWO_PI*static_cast<double>(i)*1./result.size();
            result[i]+=sfiltered.re[f]*cos(f*w)-sfiltered.im[f]*sin(f*w);
        }
    }
    return result;
}

void Filter::compute1()
{
    Complex<double> filter( getDFTfilterLP1(cutoff) );
    Complex<double> inputC( getDFTsignal(*input) );
    Complex<double> filtered;

    filtered = dconvolution(inputC, filter);
    output = getDFTinverse(filtered);
}

void Filter::compute()
{
    double fc = cutoff;
    int Fe = SoundManager::SAMPLE_RATE;
    Complex<double> filter;
    filter.resize(input->size()/2);
    for (unsigned int f = 0; f < filter.size(); f++) {
        double w  = TWO_PI*f;
        double wc = (w*Fe)/(fc*input->size());
        double gain_squared = 1.0/(1.+pow(wc,8));
        filter.re[f]=gain_squared;
        filter.im[f]=0;
    }

    std::vector<double> filt;
    filt = getDFTinverseD(filter);
    output.resize(input->size(),0);
    for (unsigned int i = 0; i < output.size(); i++) {
        //output[i] = 0;
        for (unsigned int f = 0; f < output.size(); f++){
            output[i] += static_cast<sf::Int16>( 1.*(*input)[(input->size() + i - f) % input->size()] * filt[f]);
        }
    }

}

std::vector<sf::Int16> Filter::getOutput()
{
    if(output.size() <= 0){
        compute1();
    }
    return output;
}

void Filter::setSignal(std::vector<sf::Int16>* raw)
{
    input = raw;
}

Complex<double> Filter::getDFTfilterLP1(double alpha)
{
    alpha = alpha/TWO_PI;
    Complex<double> filter;
    filter.resize(input->size());
    double w;
    for(unsigned int f=0;f<filter.size();f++) {
        w = TWO_PI*f;
        filter.re[f]=alpha*alpha/(alpha*alpha+w*w);
        filter.im[f]=-w*alpha/(alpha*alpha+w*w);
    }
    return filter;
}

