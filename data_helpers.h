#ifndef VIRUSINDEX_H
#define VIRUSINDEX_H

//extern int virusIndex, tempIndex, humIndex, co2Index, dustIndex;
extern float currDust, currCo2, currVoc, currTemperature, currHumidity, currLight, currSound;
const int smoothDataInterval = 20;

//
//int safeIndex();
//int resetIndex();
void initSmoothSensorData();
void smoothSensorData(); 

#endif /* VIRUSINDEX_H */