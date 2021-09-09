# homebridge_DaikinAC
A project using homebridge to control my Daikin air conditioner via esp8266 micro controller

Start: August, 2021

## Changelog

1.0 add funtion of turning AC on or off

## Arduino esp8266

Dependency: [Arduino_DY_IRDaikin](https://github.com/danny-source/Arduino_DY_IRDaikin)

Download as zip file and add it to your arduino library

<img src="https://user-images.githubusercontent.com/45283397/132716147-66859e14-2b7a-4437-b692-345f9af63917.jpg" width="600" />

#### GPIO setup

CK-013 or you may use your own IR transmitter

DAT: D2 (GPIO 4)
VCC: 3.3v
GND: GND

## Homebridge

Dependency: [homebridge-http](https://github.com/rudders/homebridge-http#readme)

Append following in config:

```
"accessories": [
        {
            "accessory": "Http",
            "name": "AC",
            "on_url": "http://192.168.0.5/cmd?A=On&B=3",
            "off_url": "http://192.168.0.5/cmd?A=Off&B=3",
            "http_method": "POST"
        }
    ]
```

## ToDo

- [x] Control the power of AC
- [ ] Set specific temperature
- [ ] Control temperature and fan speed up and down




