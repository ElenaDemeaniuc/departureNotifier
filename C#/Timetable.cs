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
        public string? date { get; set; }
        public string? route { get; set; }
        public TimetableItem[] timetable { get; set; }

    }

    public class TimetableItem
    {
        public TimetableItem()
        {
        }
        public string? starttime { get; set; }
        public string? start { get; set; }
        public string? destinationtime { get; set; }
        public string? destination { get; set; }
        public string? totaltime { get; set; }

    }
}