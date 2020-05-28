/*
 * plot.ino - Handles plotting signal data
 * via Devin Conley's Arduino Plotter library.
 */

#include <Plotter.h>
Plotter plot;
double uvPlotRunningMean;
double uvplot_data;
double uvPlotMinPos;
double uvPlotMaxPos;
double uvPlotMinNeg;
double uvPlotMaxNeg;
double irPlotRunningMean;
double irplot_data;
double irPlotMinPos;
double irPlotMaxPos;
double irPlotMinNeg;
double irPlotMaxNeg;

void setup_plot() {
  plot.Begin();
  plot.AddTimeGraph("UV Data", 200, "Data", uvplot_data, "Running Mean", uvPlotRunningMean, "Min", uvPlotMinPos, "Max", uvPlotMaxPos, "Min", uvPlotMinNeg, "Max", uvPlotMaxNeg);
  plot.AddTimeGraph("IR Data", 200, "Data", irplot_data, "Running Mean", irPlotRunningMean, "Min", irPlotMinPos, "Max", irPlotMaxPos, "Min", irPlotMinNeg, "Max", irPlotMaxNeg);
}

void plot_data() {
    uvplot_data = uvData.get_latest_data();
    uvPlotRunningMean = uvData.get_running_mean();
    uvPlotMinPos = uvPlotRunningMean + uvData.min_activation;
    uvPlotMaxPos = uvPlotRunningMean + uvData.max_activation;
    uvPlotMinNeg = uvPlotRunningMean - uvData.min_activation;
    uvPlotMaxNeg = uvPlotRunningMean - uvData.max_activation;
 
    irplot_data = irData.get_latest_data();
    irPlotRunningMean = irData.get_running_mean();
    irPlotMinPos = irPlotRunningMean + irData.min_activation;
    irPlotMaxPos = irPlotRunningMean + irData.max_activation;
    irPlotMinNeg = irPlotRunningMean - irData.min_activation;
    irPlotMaxNeg = irPlotRunningMean - irData.max_activation;
 
    plot.Plot();
}
