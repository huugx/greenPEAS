<!-- language: lang-none -->
                               ____  _____    _    ____                  
      __ _ _ __ ___  ___ _ __ |  _ \| ____|  / \  / ___|                 
     / _` | '__/ _ \/ _ \ '_ \| |_) |  _|   / _ \ \___ \                 
    | (_| | | |  __/  __/ | | |  __/| |___ / ___ \ ___) |                
     \__, |_|  \___|\___|_| |_|_|   |_____/_/   \_\____/                 
     |___/                                                               

# greenPEAS [![greenPEAS](https://img.shields.io/badge/green-PEAS-success)](https://github.com/hfenaux/greenPEAS) [![Latest Github release](https://img.shields.io/badge/build-development-informational)](https://github.com/hfenaux/greenPEAS)


An indoor environmental air quality sensor built on the Arduino framework.
* Evaluate local values for temperature, humidity, VOC, CO2, PM2.5, light, and sound
* Display live sensor readings
* Identify levels that exceed recommended ranges
* Wirelessly upload data to Arduino IOT Cloud

### Sensors
* [VEML 7700](https://www.adafruit.com/product/4162) - Lux sensor
* [SGP30](https://www.adafruit.com/product/3709) - Gas sensor
* [SHTC3](https://www.adafruit.com/product/4636) - Temperature and Humidity
* [Waveshare Dust Sensor](https://www.waveshare.com/dust-sensor.htm) - PM2.5 particulate sensor
* [Grove Sound Sensor](https://wiki.seeedstudio.com/Grove-Sound_Sensor/) - Analogue sound sensor

### Components
* [1.8" Color TFT](https://www.adafruit.com/product/358) - 128x128 px
* [Fan](https://www.adafruit.com/product/4468) - 5V cooling fan
* 3V-5V Step Up Converter
* 350mAh Lipo Battery

### Reference Links
* [Image to Byte Array](https://javl.github.io/image2cpp/)
* [TFT Reference](https://learn.adafruit.com/adafruit-gfx-graphics-library/coordinate-system-and-units)
* [16 Bit Color Generator](https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html)
* [Arduino JSON Assistant](https://arduinojson.org/v6/assistant/)
* [ASCII Text Generator](https://patorjk.com/software/taag/#p=testall&f=Alpha&t=greenPEAS)



## Adafruit IO Cloud

1. Generate username and active key by clicking **My Key**
   ![Screen Shot 2021-05-18 at 12 38 08 PM](https://user-images.githubusercontent.com/28346089/118690965-95db6580-b7d6-11eb-9efd-bc173032b1ea.png)
2. Update username and active key values in `Config.h`
3. Add feeds to match variables in `Config.h` (see "Adafruit IO Feeds/feeds.csv")
    ```
    feed1["key"] = "temperature";
    feed2["key"] = "humidity";
    feed3["key"] = "co2";
    feed4["key"] = "voc";
    feed5["key"] = "dust";
    feed6["key"] = "light";
    feed7["key"] = "sound";
    feed8["key"] = "index";
    ```
4. Create Dashboard


