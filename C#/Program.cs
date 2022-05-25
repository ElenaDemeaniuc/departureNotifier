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

            //  DateTimeOffset valueString4 = Convert.ToDateTime(DateToday);
            Console.WriteLine(CurrentTime);
            Console.WriteLine("Debug time: " + DateToday);
            Console.WriteLine(TimeNowMillis);
            // Console.WriteLine(valueString4);

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
            for (var i = 0; i < array.Length; i += 2)
            {
                int a = int.Parse(array[i + 1]);
                homeToStation.Add(array[i], a);
            }

            foreach (KeyValuePair<string, int> kvp in homeToStation)
            {
                Console.WriteLine("Key = {0}, Value = {1}",
                    kvp.Key, kvp.Value);
            }




            var query = new Dictionary<string, string>()
            { //%2A
                ["from"] = "Budapest*",
                ["to"] = "Wien*",
                ["date"] = DateToday
                //["date"] = "2022.05.23"
            };
            var uri = QueryHelpers.AddQueryString("https://apiv2.oroszi.net/elvira", query); //building uri

            var streamTask = client.GetStreamAsync(uri);
            var schedule = await JsonSerializer.DeserializeAsync<Schedule>(await streamTask); //deserialize Json

            //foreach (var tr in schedule.timetable)
            //    Console.WriteLine($"Start> {tr.start} Dest> {tr.destination} start time> {tr.starttime} arrival> {tr.destinationtime}");





            List<Train> listOfTrains = new List<Train>();

            foreach (var tr in schedule.timetable)
            {
                int starting = StringToMillis(DateToday, tr.starttime);
                int ending = StringToMillis(DateToday, tr.destinationtime);
                if (starting > TimeNowMillis + 60000) //current time plus 1 minute
                    listOfTrains.Add(new Train(tr, starting, ending));
            }
            Train[] arrayOfTrains = listOfTrains.ToArray();
            foreach (var trainn in arrayOfTrains)
                Console.WriteLine($"Start: {trainn.startStation} at {trainn.StartTimeUnix}, End: {trainn.endStation} at {trainn.EndTimeUnix}, {trainn.TimeTillStationMinutes}");

        }



        static async Task Main(string[] args)
        {
            await ProcessRepositories();

        }
    }
}

