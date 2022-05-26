using System;
using System.Collections.Generic;

namespace WebAPIClient
{
    public class Train
    {
        public string? startStation { get; set; }
        public string? endStation { get; set; }
        public int StartTimeMillis { get; set; }
        public int EndTimeMillis { get; set; }
        public string? TimeOnRoad { get; set; }
        public int TimeTillStationMinutes { get; set; }
        public int TimeToLeaveHome { get; set; }
        public Train() { }
        public Train(TimetableItem input, int startinput, int endinput)
        {
            startStation = input.start;
            endStation = input.destination;
            TimeOnRoad = input.totaltime;
            StartTimeMillis = startinput;
            EndTimeMillis = endinput;
            TimeTillStationMinutes = 0;
            TimeToLeaveHome = 0;
        }

    }
}