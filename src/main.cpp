#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <map>
#include <deque>
#include <vector>

#include "../include/SoundManager.h"

using namespace std;

int main()
{
    srand (time(NULL));
    const unsigned int SCREEN_WIDTH = 1000;

    SoundManager music;

    double listNotes[30];
    for(int i=0;i<30;i++){
        listNotes[i] = 220. * pow(2,static_cast<double>(i)/12.);
        //std::cout<<"note "<<i<<" fq: "<<listNotes[i]<<std::endl;
    }

    std::map<unsigned, double> keyboard;
    keyboard[sf::Keyboard::W] = listNotes[0]; // C
    keyboard[sf::Keyboard::X] = listNotes[2];
    keyboard[sf::Keyboard::C] = listNotes[4];
    keyboard[sf::Keyboard::V] = listNotes[5];
    keyboard[sf::Keyboard::B] = listNotes[7];
    keyboard[sf::Keyboard::N] = listNotes[9]; // A
    keyboard[sf::Keyboard::Comma] = listNotes[11];

    keyboard[sf::Keyboard::Comma] = listNotes[12];
    keyboard[sf::Keyboard::Period] = listNotes[12+2];
    keyboard[sf::Keyboard::Slash] = listNotes[12+4];
    //keyboard[sf::Keyboard::] = listNotes[11];

    keyboard[sf::Keyboard::A] = listNotes[12+0]; // C
    keyboard[sf::Keyboard::Z] = listNotes[12+2];
    keyboard[sf::Keyboard::E] = listNotes[12+4];
    keyboard[sf::Keyboard::R] = listNotes[12+5];
    keyboard[sf::Keyboard::T] = listNotes[12+7];
    keyboard[sf::Keyboard::Y] = listNotes[12+9]; // A
    keyboard[sf::Keyboard::U] = listNotes[12+11];

    keyboard[sf::Keyboard::I] = listNotes[24+0];
    keyboard[sf::Keyboard::O] = listNotes[24+2];
    keyboard[sf::Keyboard::P] = listNotes[24+4];

    // semitone:
    keyboard[sf::Keyboard::Num2] = listNotes[12+1];
    keyboard[sf::Keyboard::Num3] = listNotes[12+3];

    keyboard[sf::Keyboard::Num5] = listNotes[12+6];
    keyboard[sf::Keyboard::Num6] = listNotes[12+8];
    keyboard[sf::Keyboard::Num7] = listNotes[12+10];

    keyboard[sf::Keyboard::Num9] = listNotes[24+1];
    keyboard[sf::Keyboard::Num0] = listNotes[24+3];

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH,400), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    double tempfreq = -1;
    double val = 0.;

    window.setKeyRepeatEnabled(false);

    // main loop

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                music.update();
                window.close();
                break;
            }else if (event.type == sf::Event::KeyPressed) {
                if(keyboard.count(event.key.code) > 0 ) {
                    tempfreq = keyboard[event.key.code];
                }
            }
        }

        if(tempfreq > 0.){
            music.addNote(tempfreq);
            //std::cout<<"note "<<tempfreq<<std::endl;
            /*Sound.stop();
            setRaw1(raw[0],SAMPLE_RATE, AMPLITUDE, tempfreq);
            Buffer = sf::SoundBuffer();
            if (!Buffer.loadFromSamples(&raw[0][0], SAMPLES, 1, SAMPLE_RATE)) {
                std::cerr << "Loading failed!" << std::endl;
                return 1;
            }
            Sound.setBuffer(Buffer);
            Sound.play();*/
            tempfreq = -1.;
        }

        window.clear();
        /*for(int i = 0; i<400; i++){
            double val = 200.0+raw[i*SAMPLES/40000]*200./(2*AMPLITUDE);
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(i, 200)),
                sf::Vertex(sf::Vector2f(i, val))
            };
            line[0].color = sf::Color(255,0,0);
            window.draw(line, 2, sf::Lines);
        }*/

        music.update();

        for(unsigned int i = 0; i<SCREEN_WIDTH; i++){
            int value = music.getSumRaw(i);
            if(value != 0){
                val = 200.0*(1.-1.*value/(music.AMPLITUDE*2));
                if(val < 400. && val > 0.){

                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(i, 200)),
                        sf::Vertex(sf::Vector2f(i, val))
                    };
                    line[0].color = sf::Color(0,150,0);
                    line[1].color = sf::Color(0,205,25);
                    window.draw(line, 2, sf::Lines);

                    int valAnt;
                    valAnt = (i>0)? 200.0*(1.-1.*music.getSumRaw(i-1)/(music.AMPLITUDE*2)) : val;
                    sf::Vertex line2[] =
                    {
                        sf::Vertex(sf::Vector2f(i, valAnt)),
                        sf::Vertex(sf::Vector2f(i, val))
                    };
                    line2[0].color = sf::Color(255,255,255);
                    line2[1].color = sf::Color(255,255,255);
                    window.draw(line2, 2, sf::Lines);
                }
            }
        }

        //window.draw(shape);
        window.display();
    }
    return EXIT_SUCCESS;
}
