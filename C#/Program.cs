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
            DateTime UnixTimeBegin = new DateTime(1970, 1, 1);
            DateTime CurrentTime = DateTime.Now;
            string DateToday = CurrentTime.ToString("yyyy.MM.dd");
            //Console.WriteLine(CurrentTime);
            //Console.WriteLine("Debug time: " + CurrentTime.ToString("yyyy.MM.dd"));

            long StringToUnix(string InputDateToday, string inputTime)
            {
                //     if (inputTime is null)
                //     {
                //         return 0;
                //        // throw new ArgumentNullException(nameof(inputTime));
                //     }
                string valueString = $"{InputDateToday} {inputTime}";
                DateTimeOffset valueDateTime = Convert.ToDateTime(valueString); //ofset because GMT     
                TimeSpan PassedTime = valueDateTime.Subtract(UnixTimeBegin);
                long valueInMillisecons = (long)PassedTime.TotalMilliseconds;
                return valueInMillisecons;
            }


            var query = new Dictionary<string, string>()
            { //%2A
                ["from"] = "Erd*",
                ["to"] = "Budapest",
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
                long starting = StringToUnix(DateToday, tr.starttime);
                long ending = StringToUnix(DateToday, tr.destinationtime);
                listOfTrains.Add(new Train(tr, starting, ending));
            }
            Train[] arrayOfTrains = listOfTrains.ToArray();
            foreach (var trainn in arrayOfTrains)
                Console.WriteLine($"Start: {trainn.startStation} at {trainn.StartTimeUnix}, End: {trainn.endStation} at {trainn.EndTimeUnix}, {trainn.TimeTillStationMinutes}");


            // string valueString = $"{DateToday} 18:40";
            // DateTimeOffset valueDateTime = Convert.ToDateTime(valueString); //ofset because GMT     
            // TimeSpan PassedTime = valueDateTime.Subtract(UnixTimeBegin);
            // long valueInMillisecons = (long)PassedTime.TotalMilliseconds;
            // Console.WriteLine(valueInMillisecons);
            //Console.WriteLine($"Debug:  {valueDateTime}");
            //Console.WriteLine($"OldTime:  {UnixTimeBegin}");

        }

        static async Task Main(string[] args)
        {
            await ProcessRepositories();

        }
    }
}

