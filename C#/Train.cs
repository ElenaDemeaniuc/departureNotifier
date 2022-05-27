using System;
using System.Collections.Generic;

namespace WebAPIClient
{
    public class Train
    {
        public string? startCity { get; set; }
        public string? endCity { get; set; }
        public string? startStation { get; set; }
        public string? endStation { get; set; }
        public string? startTime { get; set; }
        public string? endTime { get; set; }
        public string? timeOnRoad { get; set; }
        public int timeToLeaveHome { get; set; }
        public Train() { }
        public Train(TimetableItem input, int leavetime, string citystart, string cityend)
        {
            startStation = input.start;
            endStation = input.destination;
            timeOnRoad = input.totaltime;
            startTime = input.starttime;
            endTime = input.destinationtime;
            startCity = citystart;
            endCity = cityend;
            timeToLeaveHome = leavetime;
        }

    }
}