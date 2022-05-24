using System;
using System.Collections.Generic;

namespace WebAPIClient
{
    public class Train
    {
        public string? startStation { get; set; }
        public string? endStation { get; set; }
        public long StartTimeUnix { get; set; }
        public long EndTimeUnix { get; set; }
        // public string? StartTimeString { get; set; }
        // public string? EndTimeString { get; set; }
        public string? TimeOnRoad { get; set; }
        public int TimeTillStationMinutes { get; set; }
        public long TimeToLeaveHome { get; set; }
        public Train() { }
        public Train(TimetableItem input, long startinput, long endinput)
        {
            startStation = input.start;
            endStation = input.destination;
            TimeOnRoad = input.totaltime;
            StartTimeUnix = startinput;
            EndTimeUnix = endinput;
            TimeTillStationMinutes = 0;
            TimeToLeaveHome = 0;
        }


    }
}