using System;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using Microsoft.AspNetCore.WebUtilities;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;


namespace WebAPIClient
{
    class Program
    {
        private static readonly HttpClient client = new HttpClient();


        private static async Task ProcessRepositories()
        {
            DateTime CurrentTime = DateTime.Now;
            string DateToday = CurrentTime.ToString("yyyy.MM.dd");
            string TimeNow = CurrentTime.ToString("HH:mm:ss");
            int TimeNowMillis = StringToMillis(DateToday, TimeNow);

            int StringToMillis(string InputDateToday, string inputTime)
            {
                string valueString = $"{InputDateToday}";
                string valueString2 = $"{InputDateToday} {inputTime}";
                DateTimeOffset valueDateTime = Convert.ToDateTime(valueString); //ofset because GMT    
                DateTimeOffset valueDateTime2 = Convert.ToDateTime(valueString2);
                TimeSpan PassedTime = valueDateTime2.Subtract(valueDateTime);
                int valueInMillisecons = (int)PassedTime.TotalMilliseconds;
                return valueInMillisecons;
            }


            Dictionary<string, int> homeToStation = new Dictionary<string, int>();
            var array = File.ReadAllLines("StationText.txt");
            for (var i = 0; i < array.Length; i++)
            {
                string[] subs = array[i].Split(':');
                if (String.IsNullOrEmpty(subs[1]))
                    Console.WriteLine("File content error, time value is missing");
                try
                {
                    int a = int.Parse(subs[1]);
                    homeToStation.Add(subs[0], a); //subs[0] should be in hungarian letters. Copy-paste from website
                }
                catch (FormatException)
                {
                    Console.WriteLine($"Unable to parse '{subs[1]}' for '{subs[0]}'");
                }
            }

            // foreach (KeyValuePair<string, int> kvp in homeToStation)
            // {
            //     Console.WriteLine("Key = {0}, Value = {1}",
            //         kvp.Key, kvp.Value);
            // }

            var query = new Dictionary<string, string>()
            {
                ["from"] = "Budapest*",    //Can use %2A for asterisk
                ["to"] = "Erd*",
                ["date"] = DateToday       //["date"] = "2022.05.23" correct date format         
            };
            var uri = QueryHelpers.AddQueryString("https://apiv2.oroszi.net/elvira", query); //building uri

            var streamTask = client.GetStreamAsync(uri);
            var schedule = await JsonSerializer.DeserializeAsync<Schedule>(await streamTask); //deserialize Json

            // foreach (var tr in schedule.timetable)
            //     Console.WriteLine($"Start> {tr.start} Dest> {tr.destination} start time> {tr.starttime} arrival> {tr.destinationtime}");

            List<Train> listOfTrains = new List<Train>();

            foreach (var tr in schedule.timetable)
            {
                int starting = StringToMillis(DateToday, tr.starttime);
                int ending = StringToMillis(DateToday, tr.destinationtime);
                if (starting > TimeNowMillis + 60000 && starting < TimeNowMillis + 60000 * 360) //current time plus 1 minute
                    listOfTrains.Add(new Train(tr, starting, ending));
            }

            Train[] arrayOfTrains = listOfTrains.ToArray();
            foreach (var trainn in arrayOfTrains)
            {
                int minutetemp;
                bool hasValue = homeToStation.TryGetValue(trainn.startStation, out minutetemp);
                if (hasValue)
                {
                    trainn.TimeTillStationMinutes = minutetemp;
                    trainn.TimeToLeaveHome = trainn.StartTimeMillis - minutetemp * 60000;
                }
                else
                {
                    Console.WriteLine("Key not present");
                }

                Console.WriteLine($"Start: {trainn.startStation} at {trainn.StartTimeMillis}, End: {trainn.endStation} at {trainn.EndTimeMillis}, {trainn.TimeTillStationMinutes}, Leave home at: {trainn.TimeToLeaveHome}");
                // Console.WriteLine($"{trainn.TimeTillStationMinutes}, {trainn.TimeToLeaveHome}");
            }
        }

        static async Task Main(string[] args)
        {
            await ProcessRepositories();

        }
    }
}

