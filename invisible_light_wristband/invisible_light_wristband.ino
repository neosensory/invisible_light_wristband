/*
 * Copyright 2020 Neosensory, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Please note that while this Neosensory SDK has an Apache 2.0 license, 
 * usage of the Neosensory API to interface with Neosensory products is 
 * still  subject to the Neosensory developer terms of service located at:
 * https://neosensory.com/legal/dev-terms-service/
 * 
 * invisible_light_wristband.ino - Collects data from a
 * near-infrared sensor and an ultraviolet sensor and sends
 * that data to Buzz as vibrations.
 * 
 */

#include <neosensory_bluefruit.h>
#include <Wire.h>
#include "Adafruit_SI1145.h"
#include "sensors.h"

NeosensoryBluefruit NeoBluefruit;

#define UV_MOTOR 0
#define IR_MOTOR 3

Adafruit_SI1145 uv = Adafruit_SI1145();
long last_read_time;

SensorData uvData(true, 6, 0.4, 4, RECORDING_SAMPLE_RATE, "UV");
SensorData irData(true, 3, 0.025, 0.25, RECORDING_SAMPLE_RATE, "IR");

bool do_print_buzz_messages = false;
bool do_print_sensor_data = false;
bool do_plot_data = true;

int num_frames_to_send;

void setup() {
  Serial.begin(9600);
  NeoBluefruit.begin();
  NeoBluefruit.setConnectedCallback(onConnected);
  NeoBluefruit.setDisconnectedCallback(onDisconnected);
  NeoBluefruit.setReadNotifyCallback(onReadNotify);
  NeoBluefruit.startScan();
  setup_sensors();
  setup_plot();
  num_frames_to_send = RECORDING_DELAY_MS / NeoBluefruit.firmware_frame_duration();
}

void loop () {
  bool has_new_data = read_sensors();
  
  if (has_new_data) {
    if (do_print_sensor_data) {
      uvData.print_info();
      irData.print_info();
    }
    
    if (do_plot_data) {
      plot_data();
    }
    
    if (NeoBluefruit.isConnected() & NeoBluefruit.isAuthorized()) {
      float** motor_frames = get_motor_frames(uvData.get_activation(), irData.get_activation());
      NeoBluefruit.vibrateMotors(motor_frames, num_frames_to_send);
    }
  }
}

// Get the frames that we should send to the wristband, based on our data
float** get_motor_frames(float uvActivation, float irActivation) {
  float **motor_frames = new float*[num_frames_to_send];
  for (int i = 0; i < num_frames_to_send; i++) {
    motor_frames[i] = new float[NeoBluefruit.num_motors()];
    for (int j = 0; j < NeoBluefruit.num_motors(); j++) {
      motor_frames[i][j] = 0;
    }
    motor_frames[i][UV_MOTOR] = (i % 2 & uvActivation < 0) ? 0 : abs(uvActivation);
    motor_frames[i][IR_MOTOR] = (i % 2 & irActivation < 0) ? 0 : abs(irActivation);
  }
  return motor_frames;
}

// Setup sensors so that data can be collected
void setup_sensors() {
  uv.begin();
}

// Update the data in our sensor objects. Will do nothing if called before RECORDING_DELAY_MS since the last update.
// Returns true if we received new data, false if we did nothing.
bool read_sensors() {
  if (millis() - last_read_time < RECORDING_DELAY_MS) {
    return false;
  }
  last_read_time = millis();

  float ir_reading = uv.readIR();
  float uv_reading = uv.readUV();
  
  irData.add_new_sensor_data(ir_reading);
  uvData.add_new_sensor_data(uv_reading);

  return true;
}

/* Buzz Callbacks */

void onConnected(bool success) {
  if (!success) {
    Serial.println("Attempted connection to Buzz but failed.");
    return;
  }
  Serial.println("Connected to Buzz!");
  NeoBluefruit.authorizeDeveloper();
  NeoBluefruit.acceptTermsAndConditions();
  NeoBluefruit.stopAlgorithm();
}

void onDisconnected(uint16_t conn_handle, uint8_t reason) {
  Serial.println("\nDisconnected from Buzz.");
}

void onReadNotify(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  if (!do_print_buzz_messages) return;
  for (int i = 0; i < len; i++) {
    Serial.write(data[i]);
  }
}
