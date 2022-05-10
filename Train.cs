using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace WebAPIClient
{
    public class Schedule
    {
        public Schedule()
        {
        }
        public string date { get; set; }
        public string route { get; set; }
        public Train[] timetable { get; set; }
        // public List<Train>? timetable {get; set;}
    }

    public class Train
    {
        public Train()
        {
        }
        public string? starttime { get; set; }
        public string? start { get; set; }
        public string? destinationtime { get; set; }
        public string? destination { get; set; }
        public string? totaltime { get; set; }

    }
}