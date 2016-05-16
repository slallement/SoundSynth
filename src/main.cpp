#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <map>

#include "../include/SoundManager.h"

using namespace std;

int main()
{
    const unsigned int WINDOW_WIDTH = 1000;
	const unsigned int WINDOW_HEIGHT = 400;
    const double inital_freq = 220.0; // Hz : the lowest note on the keyboard

    SoundManager music;

	// compute the note frequency
    double listNotes[30];
    for(int i=0; i<30; i++){
        listNotes[i] = inital_freq * pow(2,static_cast<double>(i)/12.0);
    }

	// KEYBOARD mapping for AZERTY keyboard
    std::map<unsigned, double> keyboard;
    keyboard[sf::Keyboard::W] = listNotes[0]; // C
    keyboard[sf::Keyboard::X] = listNotes[2];
    keyboard[sf::Keyboard::C] = listNotes[4];
    keyboard[sf::Keyboard::V] = listNotes[5];
    keyboard[sf::Keyboard::B] = listNotes[7];
    keyboard[sf::Keyboard::N] = listNotes[9]; // A
    keyboard[sf::Keyboard::Comma] = listNotes[11];

    keyboard[sf::Keyboard::Period] = listNotes[12];
    keyboard[sf::Keyboard::Slash] = listNotes[12+2];
    //keyboard[sf::Keyboard::Num1] = listNotes[12+4];

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

	keyboard[sf::Keyboard::S] = listNotes[0 + 1];
	keyboard[sf::Keyboard::D] = listNotes[0 + 3];

	keyboard[sf::Keyboard::G] = listNotes[0 + 6];
	keyboard[sf::Keyboard::H] = listNotes[0 + 8];
	keyboard[sf::Keyboard::J] = listNotes[0 + 10];

	keyboard[sf::Keyboard::L] = listNotes[12 + 1];
	keyboard[sf::Keyboard::M] = listNotes[12 + 3];

    // ---

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sound Synth");

    double tempfreq = -1;
    float val = 0.0f;

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

		// play the appropriate notes
        if(tempfreq > 0.){
            music.addNote(tempfreq);
            tempfreq = -1.;
        }

        window.clear();

        music.update();

		// draw the visualisation chart
        for(unsigned int i = 0; i<WINDOW_WIDTH; i++){
            int value = music.getSumRaw(i);
            if(value != 0){
                val = 200.0f*(1.0f-1.0f*value/(music.AMPLITUDE*2));
                if(val < 400.0f && val > 0.0f){
					float fi = (float)i;

					// green area
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(fi, 200.0f)),
                        sf::Vertex(sf::Vector2f(fi, val))
                    };
                    line[0].color = sf::Color(0,150,0);
                    line[1].color = sf::Color(0,205,25);
                    window.draw(line, 2, sf::Lines);

					// white line
                    float valAnt = (i>0)? 200.0f*(1.f-1.f*music.getSumRaw(i-1)/(music.AMPLITUDE*2)) : val;
                    sf::Vertex line2[] =
                    {
                        sf::Vertex(sf::Vector2f(fi, valAnt)),
                        sf::Vertex(sf::Vector2f(fi, val))
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
