#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h> // printf için
#define NOT_RANDOM false
//http://www.canliskor.com/futbol/turkiye/spor-toto-super-lig/arsiv/       Burdan kayıtları bul.
//http://tr.wikipedia.org/wiki/S%C3%BCper_Lig#ref_D.C3.BC.C5.9Fmeyenler       Ligden düşmeyenler

int IN_POP_COUNT = 8; // Daha büyük programlar için
int OUT_POP_COUNT= 3; // Daha büyük programlar için
int DATA_COUNT = 0; // Dosyadan sayılacak
int IN_NEURON_COUNT = 2;
int HID_NEURON_COUNT = 3;
int OUT_NEURON_COUNT = 1;
int ITERATION_COUNT = 1000;
float LAMBDA = 1.0f; // Öğrenme katsayısı
float ALPHA = .7f; // Momentum

float ** dataDeviations; // Datalara göre sapmalar      -- dataDeviations[ ITERATION_COUNT ][ DATA_COUNT ]
float * deviations; // Tüm datalara göre roundların sapmalları

float * nIns; // Giriş Nöronlar
float * nHids; // Gizli katman nöronları
float * nOuts; // Çıkış katmanı nöronları
float * nHids_Err; // Gizli katman hatalar
float * nOuts_Err;

float * hidNets; // Gizli katman Net değerleri
float * outNets; // Çıkış katmanı Net değerleri

float ** wHids; // Dentritler, yani ağırlıklar        -- wHids[ HID_NEURON_COUNT ][ IN_NEURON_COUNT ]
float ** dHids; // Delta, yani değişim katsayıları    -- dHids[ HID_NEURON_COUNT ][ IN_NEURON_COUNT ]
float ** wOuts; //                                    -- wOuts[ OUT_NEURON_COUNT ][ HID_NEURON_COUNT ]
float ** dOuts; //                                    -- dOuts[ OUT_NEURON_COUNT ][ HID_NEURON_COUNT ]

float * wHidB; // Sağlama nöronunun ağırlıkları ve
float * dHidB; // değişim katsayıları
float * wOutB;
float * dOutB;


float randomFloat(float start = .001, float end = .998)
{
    return ( start + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(end))) );
    // 0.001 ve 0.999 arasında rastgele sayılar.
}

void setupNetwork(bool random = true)
{
    dataDeviations = new float * [ITERATION_COUNT]; // Datalara göre sapmalar      -- dataDeviations[ ITERATION_COUNT ][ DATA_COUNT ]
    deviations = new float [ITERATION_COUNT]; // Tüm datalara göre roundların sapmalları

    nIns = new float [ IN_NEURON_COUNT ]; // Giriş Nöronlar
    nHids = new float [ HID_NEURON_COUNT ]; // Gizli katman nöronları
    nOuts = new float [ OUT_NEURON_COUNT ]; // Çıkış katmanı nöronları
    nHids_Err = new float [ HID_NEURON_COUNT ]; // Gizli katman hatalar
    nOuts_Err = new float [ OUT_NEURON_COUNT ];

    hidNets = new float [ HID_NEURON_COUNT ]; // Gizli katman Net değerleri
    outNets = new float [ OUT_NEURON_COUNT ]; // Çıkış katmanı Net değerleri

    wHids = new float * [ HID_NEURON_COUNT ]; // Dentritler, yani ağırlıklar        -- wHids[ HID_NEURON_COUNT ][ IN_NEURON_COUNT ]
    dHids = new float * [ HID_NEURON_COUNT ]; // Delta, yani değişim katsayıları    -- dHids[ HID_NEURON_COUNT ][ IN_NEURON_COUNT ]
    wOuts = new float * [ OUT_NEURON_COUNT ]; //                                    -- wOuts[ OUT_NEURON_COUNT ][ HID_NEURON_COUNT ]
    dOuts = new float * [ OUT_NEURON_COUNT ]; //                                    -- dOuts[ OUT_NEURON_COUNT ][ HID_NEURON_COUNT ]

    wHidB = new float [HID_NEURON_COUNT]; // Sağlama nöronunun ağırlıkları ve
    dHidB = new float [HID_NEURON_COUNT]; // değişim katsayıları
    wOutB = new float [OUT_NEURON_COUNT];
    dOutB = new float [OUT_NEURON_COUNT];

    // Dinamik dizileri tanımlıyoruz
    for (int i = 0; i < HID_NEURON_COUNT; i++)
    {
        wHids[i] = new float[ IN_NEURON_COUNT ];
        dHids[i] = new float[ IN_NEURON_COUNT ];
    }
    for (int i = 0; i < OUT_NEURON_COUNT; i++)
    {
        wOuts[i] = new float[ HID_NEURON_COUNT ];
        dOuts[i] = new float[ HID_NEURON_COUNT ];
    }
    for (int i = 0; i < ITERATION_COUNT; i++) dataDeviations[i] = new float [DATA_COUNT];

    // Dendritlere rastgele değerler veriyoruz ve değişimleri sıfırlıyoruz

    if (random)
    {
        for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
        {
            for (int inc = 0; inc < IN_NEURON_COUNT; inc++)
            {
                wHids[hidc][inc] = randomFloat();
                dHids[hidc][inc] = .0;
            }
            wHidB[hidc] = randomFloat();
            dHidB[hidc] = .0;
        }

        for (int outc = 0; outc < OUT_NEURON_COUNT; outc++)
        {
            for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
            {
                wOuts[outc][hidc] = randomFloat();
                dOuts[outc][hidc] = .0;
            }
            wOutB[outc] = randomFloat();
            dOutB[outc] = .0;
        }
    }
    else
    {
        for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
        {
            for (int inc = 0; inc < IN_NEURON_COUNT; inc++)
            {
                wHids[hidc][inc] = 0.5;
                dHids[hidc][inc] = .0;
            }
            wHidB[hidc] = 0.5;
            dHidB[hidc] = .0;
        }

        for (int outc = 0; outc < OUT_NEURON_COUNT; outc++)
        {
            for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
            {
                wOuts[outc][hidc] = 0.5;
                dOuts[outc][hidc] = .0;
            }
            wOutB[outc] = 0.5;
            dOutB[outc] = .0;
        }
    }
}

float activation(float x) // Sigmoid
{
    return (1.0 / (1.0 + exp(-x)));
    // return (x / (1.0 + abs(x))); //Fast Sigmoid... Sonra bak buna...
}

void calculateOuts(float * inValues) // İleri besleme
{
    for (int i = 0; i < IN_NEURON_COUNT; i++) nIns[i] = inValues[i];

    for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
    {
        hidNets[hidc] = .0;
        for (int inc = 0; inc < IN_NEURON_COUNT; inc++)
        {
            hidNets[hidc] += nIns[inc] * wHids[hidc][inc];
        }
        hidNets[hidc] += 1 * wHidB[hidc];
        nHids[hidc] = activation( hidNets[hidc] );
    }

    for (int outc = 0; outc < OUT_NEURON_COUNT; outc++)
    {
        outNets[outc] = .0;
        for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
        {
            outNets[outc] += nHids[hidc] * wOuts[outc][hidc];
        }
        outNets[outc] += 1 * wOutB[outc];
        nOuts[outc] = activation( outNets[outc] );
    }
}

void train( float * iVals, float * oVals)
{
    calculateOuts( iVals );
    for (int outc = 0; outc < OUT_NEURON_COUNT; outc++)
    {
        nOuts_Err[outc] = ( oVals[outc] - nOuts[outc] ) * nOuts[outc] * (1 - nOuts[outc]);
        for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
        {
            nHids_Err[hidc] = wOuts[outc][hidc] * nOuts_Err[outc] * nHids[hidc] * (1 - nHids[hidc]);
        }
        for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
        {
            for (int inc = 0; inc < IN_NEURON_COUNT; inc++)
            {
                dHids[hidc][inc] = LAMBDA * nHids_Err[hidc] * nIns[inc] + ALPHA * dHids[hidc][inc];
                wHids[hidc][inc] += dHids[hidc][inc];
            }
            dHidB[hidc] = LAMBDA * nHids_Err[hidc] * 1 + ALPHA * dHidB[hidc];
            wHidB[hidc] = wHidB[hidc] + dHidB[hidc];
        }

        for (int outc = 0; outc < OUT_NEURON_COUNT; outc++)
        {
            for (int hidc = 0; hidc < HID_NEURON_COUNT; hidc++)
            {
                dOuts[outc][hidc] = LAMBDA * nOuts_Err[outc] * nHids[hidc] + ALPHA * dOuts[outc][hidc];
                wOuts[outc][hidc] += dOuts[outc][hidc];
            }
            dOutB[outc] = LAMBDA * nOuts_Err[outc] * 1 + ALPHA * dOutB[outc];
            wOutB[outc] = wOutB[outc] + dOutB[outc];
        }
    }
}

float errorFactor(float target, float source)
{
    return (float)sqrt(pow(target - source, 2));
}

void calculateDeviations( float * iVals, float * oVals, int iteration_seq, int data_seq)
{
        calculateOuts(iVals);
        dataDeviations[iteration_seq][data_seq] = .0;
        for (int j = 0; j < OUT_NEURON_COUNT; j++)
        {
            dataDeviations[iteration_seq][data_seq] += errorFactor(oVals[j], nOuts[j]);
        }
        dataDeviations[iteration_seq][data_seq] = dataDeviations[iteration_seq][data_seq]/OUT_NEURON_COUNT;
}

#endif // NEURALNETWORK_H
