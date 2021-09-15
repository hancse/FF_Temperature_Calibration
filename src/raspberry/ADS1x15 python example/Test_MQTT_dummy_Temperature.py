import os
import time
import sys
#import Adafruit_DHT as dht
import paho.mqtt.client as mqtt
import json
import Adafruit_ADS1x15
import math
import numpy as np

# Create an ADS1115 ADC (16-bit) instance.
adc = Adafruit_ADS1x15.ADS1115()

#GAIN = 1
GAIN = 1 # 1.2412

# Steinhart–Hart_coefficient
c1 = 1.009249522e-03
c2 = 2.378405444e-04
c3 = 2.019202697e-07



broker="broker.hivemq.com"
port = 1883
#broker="broker.mqttdashboard.com"
#port = 8000
#broker="127.0.0.1"

# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=5

sensor_data = {'temperature': 0, 'humidity': 0}

next_reading = time.time() 

client = mqtt.Client()

# Set access token
#client.username_pw_set(ACCESS_TOKEN)

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect(broker, port, 60)

client.loop_start()
readings = np.array([])

try:
    while True:
        
        # Read all the ADC channel values in a list.
        values = [0]*4
        
        for i in range(4):
            # Read the specified ADC channel using the previously set gain value.
            values[i] = adc.read_adc(i, gain=GAIN)
            #adc_raw_max = values[3]
            adc_raw_max = (2**16/2 -1)*3.3/4.096
            #convert = (adc_raw_max*4.096)/((2**16/2) - 1)
            #v3out =  (values[2]*4.096)/((2**16/2) - 1)
            # Thermistor
            if values[2] == 0:
               
               values[2] =  26376
                
            #Steinhart–Hart_equation
            T_ohm = 10000 * ((adc_raw_max /values[2]) - 1.0)
            # Avoid ln of number < 0, can change to previous values
            if T_ohm <= 0:
                
                T_ohm = 1
            
            LnT_ohm = math.log(T_ohm)
            T = (1.0 / (c1 + c2*LnT_ohm + c3*(LnT_ohm)**3));
            T = T - 273.15;
                
            #print(T_ohm)    
            #math.log(T_ohm)
            # Note you can also pass in an optional data_rate parameter that controls
            # the ADC conversion time (in samples/second). Each chip has a different
            # set of allowed data rate values, see datasheet Table 9 config register
            # DR bit values.
            #values[i] = adc.read_adc(i, gain=GAIN, data_rate=128)
            # Each value will be a 12 or 16 bit signed integer value depending on the
            # ADC (ADS1015 = 12-bit, ADS1115 = 16-bit).
        # Print the ADC values.
        print('| {0:>6} | {1:>6} | {2:>6} | {3:>6} |'.format(*values))
        
        
        reading = round(T,2)
        max_samples = 5

        readings = np.append(readings, reading)
        print(readings)
        avg = np.mean(readings)
        
        if len(readings) == max_samples:
            readings = np.delete(readings, 0)
        
        print(round(avg,1))
        
        print(round(T,1))
        print(T_ohm)
        #humidity,temperature = dht.read_retry(dht.DHT22, 4)
        #humidity = round(humidity, 2)
        #temperature = round(temperature, 2)
        #print(u"Temperature: {:g}\u00b0C, Humidity: {:g}%".format(temperature, humidity))
        #sensor_data['temperature'] = round(T,1) #temperature
        sensor_data['temperature'] = round(avg,2) #temperature
        sensor_data['humidity'] = 75 #humidity

        # Sending humidity and temperature data to ThingsBoard
        client.publish('FZ/Sensor', json.dumps(sensor_data), 1)
        time.sleep(4)
        
        client.subscribe('FZ/Sensor')#subscribe
        time.sleep(2)
        print("subscribe ")
        

        next_reading += INTERVAL
        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()