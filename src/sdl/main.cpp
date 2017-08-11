#include <iostream>
#include "specrend.h"
#include <SDL/SDL.h>

#define START_WAVE_LENGTH 200
#define STOP_WAVE_LENGTH 800
#define WAVE_LENGTH_STEP 5
#define ABSORPTION_N 121

#define H   6.626192e-34    // Plancks constant
#define C   2.99792458e8    // speed of light
#define Kb  1.380649e-23    // Boltzmanns constant
#define PI  3.14159265      // PI

#define SUNLIGHT_TEMP   5778

float ambient_spectrum[ABSORPTION_N];

float ambient_spec(float wavelength){
    if (wavelength > START_WAVE_LENGTH && wavelength < STOP_WAVE_LENGTH)
        return ambient_spectrum[((int)wavelength - START_WAVE_LENGTH) / WAVE_LENGTH_STEP];
    else
        return 0;
};

float sun_spec(float wavelength)
{
    float wlm = wavelength * 1e-9;   // Wavelength in meters

    // Planck's law integrated over solid angles
    return (2 * H * C * C * pow(wlm, -5.0)) /
           (exp(H * C / (wlm * Kb * SUNLIGHT_TEMP)) - 1.0);
}

void spectrum_to_rgb(float (*spec_intens)(float wavelength), float& r, float& g, float& b){
    float x, y, z;
    struct colourSystem *cs = &SMPTEsystem;

    spectrum_to_xyz(spec_intens, &x, &y, &z);
    std::cout << "x y z: " << x << " " << y << " " << z << "\n";
    xyz_to_rgb(cs, x, y, z, &r, &g, &b);
    constrain_rgb(&r, &g, &b);
    norm_rgb(&r, &g, &b);
}

int main(){

    // absorption of pure water for different wavelengths as per: http://omlc.org/spectra/water/data/smith81.dat
    // absorptions were interpolated so that they have initial wavelength 200 nm with 5 nm steps.
    float absorptions[] = {
        3.070000, 2.53000, 1.99000, 1.65000, 1.31000, 1.11850, 0.92700, 0.82350, 0.72000, 0.63950, 0.55900, 0.50800, 0.45700, 0.41500, 0.37300, 0.33050, 0.28800, 0.25150, 0.21500, 0.17800, 0.14100, 0.12300, 0.10500, 0.09470, 0.08440, 0.07610, 0.06780, 0.06195, 0.05610, 0.05120, 0.04630, 0.04210, 0.03790, 0.03395, 0.03000, 0.02600, 0.02200, 0.02055, 0.01910, 0.01810, 0.01710, 0.01665, 0.01620, 0.01575, 0.01530, 0.01485, 0.01440, 0.01445, 0.01450, 0.01450, 0.01450, 0.01505, 0.01560, 0.01560, 0.01560, 0.01660, 0.01760, 0.01860, 0.01960, 0.02265, 0.02570, 0.03070, 0.03570, 0.04170, 0.04770, 0.04920, 0.05070, 0.05325, 0.05580, 0.05980, 0.06380, 0.06730, 0.07080, 0.07535, 0.07990, 0.09395, 0.10800, 0.13250, 0.15700, 0.20050, 0.24400, 0.26650, 0.28900, 0.29900, 0.30900, 0.31400, 0.31900, 0.32400, 0.32900, 0.33900, 0.34900, 0.37450, 0.40000, 0.41500, 0.43000, 0.44000, 0.45000, 0.47500, 0.50000, 0.57500, 0.65000, 0.74450, 0.83900, 1.00400, 1.16900, 1.48400, 1.79900, 2.08950, 2.38000, 2.42500, 2.47000, 2.51000, 2.55000, 2.53000, 2.51000, 2.43500, 2.36000, 2.26000, 2.16000, 2.11500, 2.07000
    };





    float h = 0;

    //init SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      std::cout << "error, SDL video init failed" << std::endl;
      return -1;
    }
    SDL_Surface* scr = SDL_SetVideoMode(1080, 720, 30, SDL_HWSURFACE);
    if(!scr)
    {
      std::cout << "error, no SDL screen" << std::endl;
      return -1;
    }
    SDL_WM_SetCaption("color", NULL); //set window caption

    SDL_Event event;
    bool done = 0;
    while(!done){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) done = 1;
            else if(SDL_GetKeyState(NULL)[SDLK_ESCAPE]) done = 2;
            else if(event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_DOWN)
                    h += 1;
                else if (event.key.keysym.sym == SDLK_UP){
                    h -= 1;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                    h += .1;
                else if (event.key.keysym.sym == SDLK_LEFT){
                    h -= .1;
                }
            }
        }

// LOGIC
        int ir, ig, ib;
        if (h > 50){
            ib = 255; ir = ig = 0;
        }
        else{
            float r, g, b;
            for (unsigned i = 0; i < ABSORPTION_N; ++i)
                ambient_spectrum[i] = sun_spec(WAVE_LENGTH_STEP * i+START_WAVE_LENGTH) * pow(10, -absorptions[i] * h);
                //ambient_spectrum[i] = pow(10, -absorptions[i] * h);

            spectrum_to_rgb(ambient_spec, r, g, b);
            ir = 255*r; ig = 255*g; ib = 255*b;
        }
        int color = (ir << 16) | (ig << 8) | ib;
// END LOGIC

        std::cout << "color: " << std::hex << color << "\n";
        std::cout << "depth: " << h << "\n";

        SDL_FillRect(scr, NULL, color);

        SDL_UpdateRect(scr, 0, 0, 0, 0);
        SDL_Delay(5);
    }

    

    return 0;

}
