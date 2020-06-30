/* 
 * sensors.h - A helper class that stores and handles
 * data collected from a sensor. Can be used for
 * calculating deviations from a running mean. 
 */

#define RECORDING_DELAY_MS 64
#define RECORDING_SAMPLE_RATE 1000.0/RECORDING_DELAY_MS

// A helper class that stores and handles data collected from a sensor
class SensorData {
  public:
    bool use_log; /* If true, take log base 10 of the data before handling it. Should not be true if sensor can give negative data. */
    float min_activation; /* The sensor value above the sensor's running mean that should correspond to no motor activation. */
    float max_activation; /* The sensor value above the sensor's running mean that should correspond to maximum motor activation. */
    String data_name; /* Name of data, for logging purposes. */
    
    SensorData(bool _use_log, float _smoothing_constant_duration, float _min_activation, float _max_activation, float _sample_rate, String _data_name) {
      /* Constructor for SensorData class
       * _use_log: If true, take log base 10 of the data before handling it. Should not be true if sensor can give negative data.
       * _smoothing_constant_duration: A constant which defines how the running mean behaves. 
       *        The smaller this number, the more quickly the running mean will approach the newest values.
       *        This value should be greater than 1/_sample_rate. If this number is equal to 1/_sample_rate, 
       *        the running mean will represent the newest value only.
       * _min_activation: The sensor value above the sensor's running mean that should correspond to no motor activation.
       * _max_activation: The sensor value above the sensor's running mean that should correspond to maximum motor activation.
       * _sample_rate: Sampling rate of updating this sensor data. Used to normalize _smoothing_constant_duration.
       * _data_name: Name of data, for logging purposes.
       */
      use_log = _use_log;
      min_activation = _min_activation;
      max_activation = _max_activation;
      smoothing_constant = 1 / (_sample_rate * _smoothing_constant_duration);
      data_name = _data_name;
      running_mean = 0;
      data_points_count = 0;
      has_enough_data_points = false;
    }

    // Update the sensor's running mean with a new data point. Also store this latest data point.
    void add_new_sensor_data(float new_data) {
      if (use_log && new_data <= 0) {
        return;
      }
      if (use_log) {
        new_data = log10(new_data);
      }
      float _smoothing_constant = get_smoothing_constant();
      running_mean = new_data * _smoothing_constant + running_mean * (1 - _smoothing_constant);
      latest_data = new_data;
    }

    // Returns a float between [0,1] that represents the appropriate motor activation level (on a linear scale) based on the delta between the given data and the running mean
    float get_activation(float data) {
      data = data - running_mean;
      if (abs(data) < min_activation) {
        return 0;
      }
      if (abs(data) > max_activation) {
        if(data > 0) {
          return 1;
        } else {
          return -1;
        }
      }
      if (data > 0) {
        return (data - min_activation) / (max_activation - min_activation);
      } else {
        return (abs(data) - min_activation) / (max_activation - min_activation) * -1;
      }
    }

    // Get activation value for the latest data
    float get_activation() {
      return get_activation(latest_data);
    }

    // Get the current running mean
    float get_running_mean() {
      return running_mean;
    }

    // Get the latest data
    float get_latest_data() {
      return latest_data;
    }


    // Print newest data, running mean, and activation values to Serial port
    void print_info() {
      Serial.print("Data: "); Serial.println(data_name);
      Serial.print("\tRunning mean: "); Serial.println(running_mean, 5);
      Serial.print("\t Newest data: "); Serial.println(latest_data, 5);
      Serial.print("\tNewest delta: "); Serial.println(latest_data - running_mean, 5);
      Serial.print("\t  Activation: "); Serial.println(get_activation(), 5);
    }

  private:
    float running_mean = 0;
    int data_points_count = 0;
    bool has_enough_data_points = false;
    float smoothing_constant;
    float latest_data;
    float get_smoothing_constant() {
      if (!has_enough_data_points) {
        data_points_count++;
        if (1.0 / data_points_count < smoothing_constant) {
          has_enough_data_points = true;
        }
      }
      if (!has_enough_data_points) {
        return 1.0 / data_points_count;
      }
      return smoothing_constant;
    }
    bool has_skipped_first_sample = false;
};
